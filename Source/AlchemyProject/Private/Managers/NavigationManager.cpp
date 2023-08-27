// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/NavigationManager.h"

#include "AI/AIBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/RoadSpline.h"
#include "Utility/RoadSplineComponent.h"

bool UNavigationManager::CalculateRequiredRoads(AAIBase* InActor, ARoadSpline* StartRoad, ARoadSpline* EndRoad, TArray<FRoadInfo>& OutRoadInfos)
{
	AAIBase* BaseAI = Cast<AAIBase>(InActor);
	if(BaseAI == nullptr) return false;

	FRoadConnectionContainer RoadConnectionContainer;
	
	if(StartRoad == EndRoad)
	{
		RoadConnectionContainer.RoadSplines.Add(StartRoad);
		RoadsToFollowMap.Emplace(InActor, RoadConnectionContainer);
		for(FRoadInfo& RoadInfo : RoadInfos)
		{
			if (RoadInfo.RoadSpline.Get() == StartRoad)
			{
				OutRoadInfos.Emplace(RoadInfo);
				break;
			}
		}

		BaseAI->GetRoadNames().Empty();
		TArray<FName> RoadNames;
		for(const FRoadInfo& RoadInfo : OutRoadInfos)
		{
			RoadNames.AddUnique(RoadInfo.RoadSpline->RoadName);
		}
		BaseAI->SetRoadNames(RoadNames);
		
		return true;
	}
	
	FRoadInfo StartRoadInfo;
	FRoadInfo EndRoadInfo;
	for(FRoadInfo& RoadInfo : RoadInfos)
	{
		if (RoadInfo.RoadSpline.Get() == StartRoad)
		{
			//UE_LOG(LogTemp, Warning, TEXT("== StartRoad"))
			StartRoadInfo = RoadInfo;
		}
		if (RoadInfo.RoadSpline.Get() == EndRoad)
		{
			//UE_LOG(LogTemp, Warning, TEXT("== EndRoad"))
			EndRoadInfo = RoadInfo;
		}
		if (StartRoadInfo.RoadSpline != nullptr && EndRoadInfo.RoadSpline != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("StartRoadInfo != nullptr && EndRoadInfo != nullptr"))
			break;
		}
	}
	
	if (StartRoadInfo.RoadSpline == nullptr || EndRoadInfo.RoadSpline == nullptr) return false;
	
	// First FRoadInfo* is the road that was visited and the second one in the array is the FRoadInfo* that the road was visited from
	TMap<FRoadInfo, FRoadInfo> VisitedRoads;
	TQueue<FRoadInfo> RoadsToVisit;
	TArray<FRoadInfo> OutRoads;

	RoadsToVisit.Enqueue(StartRoadInfo);
	VisitedRoads.Add(StartRoadInfo, StartRoadInfo);
	bool bFoundRoute = false;
	for(auto& Entry : RoadConnectionMap)
	{
		//if(Entry.Key.RoadSpline.Get()) UE_LOG(LogTemp, Warning, TEXT("RoadConnectionMap: RoadName: %s"), *Entry.Key.RoadSpline.Get()->GetName())
	}
	while(!RoadsToVisit.IsEmpty())
	{
		FRoadInfo CurrentRoad;
		RoadsToVisit.Dequeue(CurrentRoad);
		//if(CurrentRoad.RoadSpline) UE_LOG(LogTemp, Warning, TEXT("CurrentRoad Name: %s"), *CurrentRoad.RoadSpline->GetName())
		
		bool bContains = false;
		for(const auto& Temp : RoadConnectionMap)
		{
			if(Temp.Key.RoadSpline.Get() == CurrentRoad.RoadSpline.Get())
			{
				bContains = true;
				break;
			}
		}
		
		if(bContains)
		{
			//UE_LOG(LogTemp, Warning, TEXT("bContains"))
			
			FRoadInfo FirstRoadInfo;
			FRoadInfoContainer FirstRoadInfoContainer;
			for(const auto& Some : RoadConnectionMap)
			{
				if(Some.Key.RoadSpline == CurrentRoad.RoadSpline)
				{
					FirstRoadInfo = Some.Key;
					FirstRoadInfoContainer = Some.Value;
				}
			}
			
			if(FirstRoadInfo.RoadSpline)
			{
				for(FRoadInfo& RoadInfo: FirstRoadInfoContainer.RoadInfos)
				{
					bool bVisited = false;
					for(const auto& VisitedRoad : VisitedRoads)
					{
						if(VisitedRoad.Key.RoadSpline.Get() == RoadInfo.RoadSpline.Get())
						{
							bVisited = true;
						}
					}
					if(!bVisited)
					{
						//UE_LOG(LogTemp, Warning, TEXT("!VisitedRoads: RoadInfo: %s, EndRoadInfo: %s"), *RoadInfo.RoadSpline.Get()->GetName(), *EndRoadInfo.RoadSpline.Get()->GetName())
						RoadsToVisit.Enqueue(RoadInfo);
						VisitedRoads.Add(RoadInfo, CurrentRoad);
						if(RoadInfo.RoadSpline.Get() == EndRoadInfo.RoadSpline.Get())
						{
							//UE_LOG(LogTemp, Warning, TEXT("RoadInfo.RoadSpline.Get() == EndRoadInfo.RoadSpline.Get()"))
							OutRoads.AddUnique(RoadInfo);
							FRoadInfo RoadToAdd = RoadInfo;
							while(RoadToAdd.RoadSpline.Get() != StartRoadInfo.RoadSpline.Get())
							{
								//UE_LOG(LogTemp, Warning, TEXT("RoadToAdd: %s, StartRoadInfo: %s"), *RoadToAdd.RoadSpline.Get()->GetName(), *StartRoadInfo.RoadSpline.Get()->GetName())

								OutRoads.AddUnique(VisitedRoads[RoadToAdd]); //NOTE This gave an error because GetTypeHash was using FCrc::MemCrc32
								RoadToAdd = VisitedRoads[RoadToAdd];
							}
							bFoundRoute = true;
						}//else UE_LOG(LogTemp, Warning, TEXT("RoadInfo != EndRoadInfo"))
					}//else UE_LOG(LogTemp, Warning, TEXT("Contains:  %s"), *RoadInfo.RoadSpline.Get()->GetName())
				}
			}
			
		}//else UE_LOG(LogTemp, Warning, TEXT("RoadConnectionMap.Contains(CurrentRoad) == false"))
	}

	for(const auto& Rood : OutRoads)
	{
		if(Rood.RoadSpline)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Road Name: %s"), *Rood.RoadSpline->RoadName.ToString())
		}
	}
	
	if(OutRoads.IsEmpty()) return false;
	
	Algo::Reverse(OutRoads);
	OutRoadInfos = OutRoads;
	
	BaseAI->GetRoadNames().Empty();
	TArray<FName> RoadNames;
	for(const FRoadInfo& RoadInfo : OutRoadInfos)
	{
		RoadNames.AddUnique(RoadInfo.RoadSpline->RoadName);
	}
	BaseAI->SetRoadNames(RoadNames);
	
	return true;
	
}

bool UNavigationManager::InitRoads()
{
	const FString RoadInfoTablePath(TEXT("DataTable'/Game/Assets/Datatables/RoadInfoDataTable.RoadInfoDataTable'"));
	// ReSharper disable once CppTooWideScope
	const UDataTable* RoadInfoTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RoadInfoTablePath));
	TArray<FRoadInfo*> TableRowArray;
	TArray<FRoadInfo> TempRoadInfos;
	if(RoadInfoTableObject)
	{
		const TCHAR* Context = TEXT("blablabla");
		RoadInfoTableObject->GetAllRows<FRoadInfo>(Context, TableRowArray);
		for(const auto& TempRoad : TableRowArray)
		{
			TempRoadInfos.Add(*TempRoad);
		}
		for(const FRoadInfo Temp : TempRoadInfos)
		{
			//Storing for later use
			//if(Temp.RoadSpline) UE_LOG(LogTemp, Warning, TEXT("Road to be added: %s"), *Temp.RoadSpline->GetName())
			//else UE_LOG(LogTemp, Warning, TEXT("Road spline was nullptr"))
			RoadInfos.Add(Temp);
		}
		for (FRoadInfo RoadInfo : TempRoadInfos)
		{
			
			TArray<FRoadInfo> ConnectedRoads;
			FRoadInfoContainer RoadInfoContainer;
			for (TSoftObjectPtr<ARoadSpline>& RoadSpline : RoadInfo.RoadConnections.RoadSplines)
			{
				//UE_LOG(LogTemp, Warning, TEXT("RoadInfo: %s, Connected Road: %s"), *RoadInfo.RoadSpline->GetName(), *RoadSpline.Get()->GetName())
				TArray<FRoadInfo>::ElementType* ConnectedRoadInfo = TempRoadInfos.FindByPredicate([&](const FRoadInfo Info)
				{
					return Info.RoadSpline == RoadSpline;
				});
				
				if (ConnectedRoadInfo != nullptr)
				{
					ConnectedRoads.Add(*ConnectedRoadInfo);
				}
			}
			RoadInfoContainer.RoadInfos = ConnectedRoads;
			RoadConnectionMap.Emplace(RoadInfo, RoadInfoContainer);
		}
	}
	else
	{
		return false;
	}

	for(auto& Entry : RoadConnectionMap)
	{
		//if(Entry.Key.RoadSpline.Get()) UE_LOG(LogTemp, Warning, TEXT("RoadConnectionMap: RoadName: %s"), *Entry.Key.RoadSpline.Get()->GetName())
	}
	
	return true;
	
}

void UNavigationManager::FillAllSplinePointsMap(const AActor* const ContextActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("FillAllSplinePointsMap"))
	AllSplinePointsMap.Empty();
	TArray<AActor*> RoadSplinesArray;
	
	UGameplayStatics::GetAllActorsOfClass(ContextActor, ARoadSpline::StaticClass(), RoadSplinesArray);
	for(AActor* RoadActor : RoadSplinesArray)
	{
		//UE_LOG(LogTemp, Warning, TEXT("RoadActor: %s"), *RoadActor->GetName())
		ARoadSpline* Road = Cast<ARoadSpline>(RoadActor);
		if(Road && Road->GetSplineComponent())
		{
			FSplinePointLocContainer SplinePointLocContainer;
			for(int32 i = 0; i < Road->GetSplineComponent()->GetNumberOfSplinePoints(); i++)
			{
				SplinePointLocContainer.SplinePointLocations.Add(Road->GetSplineComponent()->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
			}
			//UE_LOG(LogTemp, Warning, TEXT("Road: %s, NumOfPoints: %d"), *Road->GetName(), SplinePointLocContainer.SplinePointLocations.Num())
			AllSplinePointsMap.Emplace(Road, SplinePointLocContainer);
		}
	}
}

ARoadSpline* UNavigationManager::GetNearestRoadSplinePoint(const AActor* const InActor, FVector& OutSplinePointPosition)
{
	if(InActor == nullptr) return nullptr;
	OutSplinePointPosition = FVector(0.f);
	
	ARoadSpline* ClosestRoad = nullptr;
	int32 ClosestPointIndex = 0;
	float ClosestDistance = 100000.f;
	
	for(const auto& Something : AllSplinePointsMap)
	{
		int32 Index = 0;
		for(const FVector& TempVector : Something.Value.SplinePointLocations)
		{
			const float TempDist = UKismetMathLibrary::Vector_Distance(TempVector, InActor->GetActorLocation());
			
			if(TempDist < ClosestDistance)
			{
				ClosestPointIndex = Index;
				ClosestDistance = TempDist;
				ClosestRoad = Something.Key;
			}
			Index++;
		}
	}
	
	if(ClosestRoad && ClosestRoad->GetSplineComponent())
	{
		OutSplinePointPosition = ClosestRoad->GetSplineComponent()->GetLocationAtSplinePoint(ClosestPointIndex, ESplineCoordinateSpace::World);
	}
	
	
	return ClosestRoad;
}

// First int32 is for the current road, the other is for the next
TMap<int32,int32> UNavigationManager::GetRoadSwitchIndex(ARoadSpline* CurrentRoad, ARoadSpline* NextRoad)
{
	TMap<int32, int32> OutMap;
	int32 OutIndex = -1;
	if(AllSplinePointsMap.Contains(CurrentRoad) && AllSplinePointsMap.Contains(NextRoad))
	{
		float ClosestDistance = 1000000.f;
		int32 Index = 0;
		for(const FVector& VectorCurrent : AllSplinePointsMap[CurrentRoad].SplinePointLocations)
		{
			int32 NextIndex = 0;
			for(const FVector& VectorNext : AllSplinePointsMap[NextRoad].SplinePointLocations)
			{
				const float TempDist = UKismetMathLibrary::Vector_Distance(VectorCurrent, VectorNext);
				
				if(TempDist < ClosestDistance)
				{
					OutMap.Empty();
					OutMap.Emplace(Index, NextIndex);
					
					ClosestDistance = TempDist;
				}
				NextIndex++;
			}
			Index++;
		}
	}

	return OutMap;
}
