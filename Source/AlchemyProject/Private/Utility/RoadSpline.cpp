// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/RoadSpline.h"

#include "AI/AIBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/RoadSplineComponent.h"


ARoadSpline::ARoadSpline()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<URoadSplineComponent>(TEXT("SplineComponent"));
	if(GetRootComponent()) SplineComponent->SetupAttachment(GetRootComponent());
	else SetRootComponent(SplineComponent);
	
}


void ARoadSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

//DEPRECATED Doesn't do anything anymore.
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
		FName TempName = NAME_None;
		if(BaseAI->GetRoadNames().IsValidIndex(BaseAI->GetCurrentRoadIndex())) TempName = BaseAI->GetRoadNames()[BaseAI->GetCurrentRoadIndex()];
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
	
	// Find the move direction
	// NOTE get the spline point that is closest to one of the end points of the next road.
	// NOTE compare the spline point index with the current index.
	// NOTE save the spline point index that was the closest and switch roads when current index is the same as the saved one
	
	
	//If next road exists
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
	if(bHasNextRoad)
	{
		if(NextRoadMoveDir == 1) NavPackage.NextRoadSplineIndex = 0;
		else if(NextRoadMoveDir == -1) NavPackage.NextRoadSplineIndex = NextRoadLastIndex;
	}
	else NavPackage.NextRoadSplineIndex = -1; 
	
	NavPackage.bFollowingRoad = true;
	ActorNavPackages.Emplace(InActor, NavPackage);
	
	return NavPackage;
}

FVector ARoadSpline::FindClosestSplinePoint(AActor* InActor)
{
	//NOTE: This function is called at the start of road movement behavior
	//NOTE: Returns the closest spline point location of the first road and saves the index

	
	AAIBase* BaseAI = Cast<AAIBase>(InActor);
	if(BaseAI == nullptr) return FVector(0.f);

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
	//NOTE: This function first checks if the AI should switch roads, then if it is at the end of the road
	//NOTE: If no, it gets the next spline point index and updates the ActorSplineIndexMap and returns the next location
	//NOTE: When switching roads, it calls GetNextRoad() and returns the start location of the new road
	
	AAIBase* BaseAI = Cast<AAIBase>(InActor);
	if(BaseAI == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseAI == nullptr"))
		bSuccess = false;
		return FVector(0.f);
	}
	
	FVector OutVector = FVector::ZeroVector;
	
	if(!ActorSplineIndexMap.Contains(BaseAI) || !ActorNavPackages.Contains(BaseAI))
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseAI not part of the ActorSplineIndexMap or the ActorNavPackages"))
		bSuccess = false;
		return FVector(0.f);
	}

	// Switch to the next road
	if(ActorSplineIndexMap[BaseAI] == ActorNavPackages[BaseAI].SwitchRoadSplineIndex)
	{
		
		bSuccess = true;
		bool bNextRoad = GetNextRoad(BaseAI);
		if(bNextRoad)
		{
			OutVector = BaseAI->GetCurrentRoad()->GetSplineComponent()->GetLocationAtSplinePoint(ActorNavPackages[BaseAI].NextRoadSplineIndex, ESplineCoordinateSpace::World);
			UE_LOG(LogTemp, Warning, TEXT("Switching Roads: %d"), bNextRoad)
			//Remove the actor from the maps of this road.
			ActorNavPackages.Remove(BaseAI);
			ActorSplineIndexMap.Remove(BaseAI);
			return OutVector;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Reached Destination"))
			ActorNavPackages.Remove(BaseAI);
			ActorSplineIndexMap.Remove(BaseAI);
			BaseAI->DestinationReached();
			return FVector(0.f);
		}
	}

	// Got to the end of the road
	// 2 checks to make sure we are at the end of the road and not at the start, because it depends on the AI move direction
	if(ActorNavPackages[BaseAI].MoveDir == 1 && (GetSplineComponent()->GetNumberOfSplinePoints() - 1) == ActorSplineIndexMap[BaseAI])
	{
		UE_LOG(LogTemp, Warning, TEXT("End of the road"))
		bSuccess = true;
		ActorNavPackages.Remove(BaseAI);
		ActorSplineIndexMap.Remove(BaseAI);
		BaseAI->DestinationReached();
		return FVector(0.f);
	}
	if(ActorNavPackages[BaseAI].MoveDir == -1 && 0 == ActorSplineIndexMap[BaseAI])
	{
		UE_LOG(LogTemp, Warning, TEXT("End of the road"))
		bSuccess = true;
		ActorNavPackages.Remove(BaseAI);
		ActorSplineIndexMap.Remove(BaseAI);
		BaseAI->DestinationReached();
		return FVector(0.f);
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
	// NOTE Takes an AActor* but only actually accepts BaseAI
	// NOTE This will be changed in the future to accept other actors too

	// NOTE: This function finds the next for the passed in actor
	// NOTE: Sets the current road to the next road
	// NOTE: Adds the actor to the ActorNavPackages map of the next road
	// NOTE: Lastly it calls GetActorNavPackage
	
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

FVector ARoadSpline::GetNearestRoadSplinePoint(const AActor* const InActor, const ARoadSpline* OutRoadSpline)
{
	if(InActor == nullptr) return FVector(0.f);
	FVector OutPosition = FVector(0.f);
	TArray<AActor*> OutActors;
	float DistanceToNearestActor;
	UGameplayStatics::GetAllActorsOfClass(InActor, ARoadSpline::StaticClass(), OutActors);
	AActor* NearestActor = UGameplayStatics::FindNearestActor(InActor->GetActorLocation(), OutActors, DistanceToNearestActor);
	ARoadSpline* NearestRoad = Cast<ARoadSpline>(NearestActor);
	if(NearestRoad == nullptr) return OutPosition;

	OutRoadSpline = NearestRoad;
	
	int32 ClosestPointIndex = 0;
	float ClosestDistance = 100000.f;
	
	for(int32 i = 0; i < NearestRoad->GetSplineComponent()->GetNumberOfSplinePoints(); i++)
	{
		const float TempDist = UKismetMathLibrary::Vector_Distance(InActor->GetActorLocation(), NearestRoad->GetSplineComponent()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
		
		if(TempDist < ClosestDistance)
		{
			ClosestDistance = TempDist;
			ClosestPointIndex = i;
		}
	}

	OutPosition = NearestRoad->GetSplineComponent()->GetLocationAtSplinePoint(ClosestPointIndex, ESplineCoordinateSpace::World);
	
	return OutPosition;
}

void ARoadSpline::ShowRoadConnections()
{
	/*
	for(const auto Something : RoadConnections)
	{
		if(IsValid(Something.Key)) UE_LOG(LogTemp, Warning, TEXT("RoadName: %s"), *Something.Key->GetName())
	}
	*/
}
