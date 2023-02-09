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
int32 ARoadSpline::GetMoveDirection(AActor* InActor)
{
	AAIBase* BaseAI = Cast<AAIBase>(InActor);
	if(BaseAI == nullptr) return 0;
	int32 Index = 0;
	FVector NextRoadFirstEndPoint = FVector::ZeroVector;
	FVector NextRoadSecondEndPoint = FVector::ZeroVector;
	float ClosestDistance = 100000.f;
	int32 OutDirection = 0;
	// Iterate over the road names
	for(const FName& SomeRoad : BaseAI->GetRoadNames())
	{
		if(SomeRoad == BaseAI->GetCurrentRoadName())
		{
			if(BaseAI->GetRoadNames().IsValidIndex(Index + 1))
			{
				const FName TempName = BaseAI->GetRoadNames()[Index+1];
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
					}
				}
			}
			else
			{
				//If BaseAI->GetRoadNames().IsValidIndex(Index + 1) == false, means that there isn't a next road.
				break;
			}
		}
		
		Index++;
	}

	//Find the move direction
	//TODO: current wont work well
	// TODO get the spline point that is closest to one of the end points of the next road.
	// TODO compare the spline point index with the current index.
	// TODO save the spline point index that was the closest and switch roads when current index is the same as the saved one
	
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
}

FVector ARoadSpline::FindClosestSplinePoint(AActor* InActor)
{
	AAIBase* BaseAI = Cast<AAIBase>(InActor);
	if(BaseAI == nullptr) return FVector();

	int32 ClosestPointIndex = 0;
	float ClosestDistance = 100000.f;
	
	for(const int32 ID : GetSplineComponent()->GetNumberOfSplinePoints())
	{
		if(const float TempDist = UKismetMathLibrary::Vector_Distance(BaseAI->GetActorLocation(), GetSplineComponent()->GetLocationAtSplinePoint(ID, ESplineCoordinateSpace::World)) < ClosestDistance)
		{
			ClosestDistance = TempDist;
			ClosestPointIndex = ID;
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
		bSuccess = false;
		return FVector();
	}

	if(!ActorSplineIndexMap.Contains(BaseAI) || !ActorMoveDirMap.Contains(BaseAI))
	{
		bSuccess = false;
		return FVector();
	}
	
	//Got to the end of the road
	if(GetSplineComponent()->GetNumberOfSplinePoints() == ActorSplineIndexMap[BaseAI])
	{
		bSuccess = true;
		return FVector();
	}
	
	return GetSplineComponent()->GetLocationAtSplinePoint(ActorSplineIndexMap[BaseAI] + ActorMoveDirMap[BaseAI], ESplineCoordinateSpace::World);
}


