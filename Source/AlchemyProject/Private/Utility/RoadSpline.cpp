// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/RoadSpline.h"

#include "AI/AIBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/RoadSplineComponent.h"


ARoadSpline::ARoadSpline()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<URoadSplineComponent>(TEXT("SplineComponent"));

}


void ARoadSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

//Doesn't do anything anymore.
FVector ARoadSpline::GetSplinePointPosition() const
{
	return FVector();
}

// Called after FindClosestSplinePoint and before calling FindNextSplinePoint
FActorNavPackage ARoadSpline::GetActorNavPackage(AActor* InActor)
{
	AAIBase* BaseAI = Cast<AAIBase>(InActor);
	if(BaseAI == nullptr) return FActorNavPackage();
	int32 Index = 0;
	// The origin point of the next road
	FVector NextRoadFirstEndPoint = FVector::ZeroVector;
	// The end point of the next road
	FVector NextRoadSecondEndPoint = FVector::ZeroVector;
	// Will be used to select the closest point
	float ClosestDistance = 100000.f;
	// Road direction that the AI should move. Up or Down the spline point array. 1 and -1 are the only acceptable values.
	int32 OutDirection = 0;
	// Keeps track of the last index of the next road
	int32 NextRoadLastIndex = 0;
	// Keeps track of the index which is used to move to the next road or the point that is closest to the NavDestination
	int32 ClosestIndex = 0;
	//Road direction for the next road
	int32 NextRoadMoveDir = 0;
	bool bHasNextRoad = true;
	ARoadSpline* NextRoad = nullptr;
	
	// If there is a next road
	if(BaseAI->GetRoadNames().IsValidIndex(BaseAI->GetCurrentRoadIndex() + 1 ))
	{
		const FName TempName = BaseAI->GetRoadNames()[BaseAI->GetCurrentRoadIndex() + 1 ];
		const FString RoadInfoTablePath(TEXT("DataTable'/Game/Assets/Datatables/RoadInfoDataTable.RoadInfoDataTable'"));
		// ReSharper disable once CppTooWideScope
		const UDataTable* RoadInfoTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RoadInfoTablePath));
		if(RoadInfoTableObject)
		{
			// ReSharper disable once CppTooWideScope
			const FRoadInfo* TableRow = RoadInfoTableObject->FindRow<FRoadInfo>(TempName, TEXT(""));
			if(TableRow)
			{
				NextRoadFirstEndPoint = TableRow->RoadSpline.Get()->SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
				NextRoadSecondEndPoint = TableRow->RoadSpline.Get()->SplineComponent->GetLocationAtSplinePoint(TableRow->RoadSpline.Get()->SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World);
				NextRoadLastIndex = TableRow->RoadSpline.Get()->SplineComponent->GetNumberOfSplinePoints() - 1;
			}
		}
	}
	else if(BaseAI->GetNavDestination()) // If NavDestination is valid
	{
		bHasNextRoad = false;
		const FName TempName = BaseAI->GetRoadNames()[BaseAI->GetCurrentRoadIndex() + 1 ];
		const FString RoadInfoTablePath(TEXT("DataTable'/Game/Assets/Datatables/RoadInfoDataTable.RoadInfoDataTable'"));
		// ReSharper disable once CppTooWideScope
		const UDataTable* RoadInfoTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RoadInfoTablePath));
		if(RoadInfoTableObject)
		{
			// ReSharper disable once CppTooWideScope
			const FRoadInfo* TableRow = RoadInfoTableObject->FindRow<FRoadInfo>(TempName, TEXT(""));
			if(TableRow)
			{
				NextRoad = TableRow->RoadSpline.Get();
				if(NextRoad && NextRoad->GetSplineComponent())
				{
					for(int32 i = 0; i < NextRoad->GetSplineComponent()->GetNumberOfSplinePoints(); i++)
					{
						if(UKismetMathLibrary::Vector_Distance(BaseAI->GetNavDestination()->GetActorLocation(), GetSplineComponent()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World)) < ClosestDistance)
						{
							ClosestDistance = UKismetMathLibrary::Vector_Distance(BaseAI->GetNavDestination()->GetActorLocation(), GetSplineComponent()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
							ClosestIndex = i;
						}
					}
				}
			}
		}
	}
	else // If NavDestination doesn't exist
	{
		bHasNextRoad = false;
		if(ActorSplineIndexMap[BaseAI] == 0) ClosestIndex = GetSplineComponent()->GetNumberOfSplinePoints() - 1;
		else ClosestIndex = 0;
	}
	
	//Find the move direction
	// TODO: current wont work well
	// TODO get the spline point that is closest to one of the end points of the next road.
	// TODO compare the spline point index with the current index.
	// TODO save the spline point index that was the closest and switch roads when current index is the same as the saved one
	// TODO Might need to create a struct that holds data like MoveDir and Spline point goal index

	

	if(bHasNextRoad)
	{
		for(int32 i = 0; i < GetSplineComponent()->GetNumberOfSplinePoints(); i++)
		{
			if(UKismetMathLibrary::Vector_Distance(NextRoadFirstEndPoint, GetSplineComponent()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World)) < ClosestDistance)
			{
				ClosestDistance = UKismetMathLibrary::Vector_Distance(NextRoadFirstEndPoint, GetSplineComponent()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
				ClosestIndex = i;
				NextRoadMoveDir = 1;
			
			}
			if(UKismetMathLibrary::Vector_Distance(NextRoadSecondEndPoint, GetSplineComponent()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World)) < ClosestDistance)
			{
				ClosestDistance = UKismetMathLibrary::Vector_Distance(NextRoadFirstEndPoint, GetSplineComponent()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
				ClosestIndex = i;
				NextRoadMoveDir = -1;
			}
		}
	}
	
	if(ActorSplineIndexMap.Contains(BaseAI))
	{
		OutDirection = ActorSplineIndexMap[BaseAI] < ClosestIndex ? 1 : -1;
	}
	
	
	FActorNavPackage NavPackage = FActorNavPackage(OutDirection, ClosestIndex, NextRoadMoveDir);
	if(NextRoadMoveDir == 1) NavPackage.NextRoadSplineIndex = 0;
	else if(NextRoadMoveDir == -1) NavPackage.NextRoadSplineIndex = NextRoadLastIndex;
	NavPackage.bFollowingRoad = true;
	ActorNavPackages.Emplace(InActor, NavPackage);
	
	return NavPackage;

	
	/**
	int32 CurrentSplinePointIndex = 0;
	if(ActorSplineIndexMap.Contains(BaseAI)) CurrentSplinePointIndex = ActorSplineIndexMap[BaseAI];
	FVector TempPos = GetSplineComponent()->GetLocationAtSplinePoint(CurrentSplinePointIndex + 1, ESplineCoordinateSpace::World);
	float TempDist = UKismetMathLibrary::Vector_Distance(NextRoadFirstEndPoint, TempPos);
	float TempDist2 = UKismetMathLibrary::Vector_Distance(NextRoadSecondEndPoint, TempPos);
	if(TempDist < ClosestDistance)
	{
		ClosestDistance = TempDist;
		OutDirection = -1;
	}
	if(TempDist2 < ClosestDistance)
	{
		ClosestDistance = TempDist2;
		OutDirection = -1;
	}
	TempPos = GetSplineComponent()->GetLocationAtSplinePoint(CurrentSplinePointIndex - 1, ESplineCoordinateSpace::World);
	TempDist = UKismetMathLibrary::Vector_Distance(NextRoadFirstEndPoint, TempPos);
	TempDist2 = UKismetMathLibrary::Vector_Distance(NextRoadSecondEndPoint, TempPos);
	if(TempDist < ClosestDistance)
	{
		ClosestDistance = TempDist;
		OutDirection = 1;
	}
	if(TempDist2 < ClosestDistance)
	{
		ClosestDistance = TempDist2;
		OutDirection = 1;
	}
	ActorMoveDirMap.Emplace(BaseAI, OutDirection);
	return OutDirection;
	
	*/
}

FVector ARoadSpline::FindClosestSplinePoint(AActor* InActor)
{
	AAIBase* BaseAI = Cast<AAIBase>(InActor);
	if(BaseAI == nullptr) return FVector();

	int32 ClosestPointIndex = 0;
	float ClosestDistance = 100000.f;
	
	for(int32 i = 0; i < GetSplineComponent()->GetNumberOfSplinePoints(); i++)
	{
		const float TempDist = UKismetMathLibrary::Vector_Distance(BaseAI->GetActorLocation(), GetSplineComponent()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
		
		if(TempDist < ClosestDistance)
		{
			ClosestDistance = TempDist;
			ClosestPointIndex = i;
		}
	}

	const FVector OutPosition = GetSplineComponent()->GetLocationAtSplinePoint(ClosestPointIndex, ESplineCoordinateSpace::World);
	ActorSplineIndexMap.Emplace(BaseAI, ClosestPointIndex);
	return OutPosition;
}

FVector ARoadSpline::FindNextSplinePoint(AActor* InActor, bool& bSuccess)
{
	AAIBase* BaseAI = Cast<AAIBase>(InActor);
	if(BaseAI == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseAI == nullptr"))
		bSuccess = false;
		return FVector();
	}
	
	FVector OutVector = FVector::ZeroVector;
	
	if(!ActorSplineIndexMap.Contains(BaseAI) || !ActorNavPackages.Contains(BaseAI))
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseAI not part of the ActorSplineIndexMap or the ActorNavPackages"))
		bSuccess = false;
		return FVector();
	}
	
	if(ActorSplineIndexMap[BaseAI] == ActorNavPackages[BaseAI].SwitchRoadSplineIndex)
	{
		// Switch to the next road
		bSuccess = true;
		GetNextRoad(BaseAI);
		OutVector = BaseAI->GetCurrentRoad()->GetSplineComponent()->GetLocationAtSplinePoint(ActorNavPackages[BaseAI].NextRoadSplineIndex, ESplineCoordinateSpace::World);
		UE_LOG(LogTemp, Warning, TEXT("Switching Roads"))
		ActorNavPackages.Remove(BaseAI);
		ActorSplineIndexMap.Remove(BaseAI);
		return OutVector;
	}

	// Got to the end of the road
	if(ActorNavPackages[BaseAI].MoveDir == 1 && (GetSplineComponent()->GetNumberOfSplinePoints() - 1) == ActorSplineIndexMap[BaseAI])
	{
		UE_LOG(LogTemp, Warning, TEXT("end of the road"))
		bSuccess = true;
		return FVector();
	}
	if(ActorNavPackages[BaseAI].MoveDir == -1 && 0 == ActorSplineIndexMap[BaseAI])
	{
		UE_LOG(LogTemp, Warning, TEXT("end of the road"))
		bSuccess = true;
		return FVector();
	}
	
	bSuccess = true;
	
	if(ActorNavPackages[BaseAI].MoveDir == 1)
	{
		OutVector = GetSplineComponent()->GetLocationAtSplinePoint(ActorSplineIndexMap[BaseAI] + 1, ESplineCoordinateSpace::World);
		ActorSplineIndexMap.Emplace(BaseAI, ActorSplineIndexMap[BaseAI] + 1);
	}
	else if (ActorNavPackages[BaseAI].MoveDir == -1)
	{
		OutVector = GetSplineComponent()->GetLocationAtSplinePoint(ActorSplineIndexMap[BaseAI] - 1, ESplineCoordinateSpace::World);
		ActorSplineIndexMap.Emplace(BaseAI, ActorSplineIndexMap[BaseAI] - 1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Movement direction was invalid"))
	}

	return OutVector;
}

bool ARoadSpline::GetNextRoad(AActor* InActor)
{
	AAIBase* BaseAI = Cast<AAIBase>(InActor);
	if(BaseAI == nullptr) return false;
	FName NextRoad = NAME_None;
	if(BaseAI->GetRoadNames().IsValidIndex(BaseAI->GetCurrentRoadIndex() + 1))
	{
		NextRoad = BaseAI->GetRoadNames()[BaseAI->GetCurrentRoadIndex() + 1];
		const FString RoadInfoTablePath(TEXT("DataTable'/Game/Assets/Datatables/RoadInfoDataTable.RoadInfoDataTable'"));
		// ReSharper disable once CppTooWideScope
		const UDataTable* RoadInfoTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RoadInfoTablePath));
		if(RoadInfoTableObject)
		{
			// ReSharper disable once CppTooWideScope
			const FRoadInfo* TableRow = RoadInfoTableObject->FindRow<FRoadInfo>(NextRoad, TEXT(""));
			if(TableRow)
			{
				if(!TableRow->RoadSpline.IsValid()) return false;
				BaseAI->SetCurrentRoad(TableRow->RoadSpline.Get());
				TableRow->RoadSpline.Get()->ActorSplineIndexMap.Emplace(BaseAI, ActorNavPackages[BaseAI].NextRoadSplineIndex);
				TableRow->RoadSpline.Get()->GetActorNavPackage(BaseAI);
				
				return true;
			}
		}
	}
	
	return false;
}
