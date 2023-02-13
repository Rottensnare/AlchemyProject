// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/NavigationManager.h"

#include "AI/AIBase.h"
#include "Utility/RoadSpline.h"

bool UNavigationManager::CalculateRequiredRoads(AAIBase* InActor, ARoadSpline* StartRoad, ARoadSpline* EndRoad, TArray<FRoadInfo>& OutRoadInfos)
{
	UE_LOG(LogTemp, Warning, TEXT("CalculateRequiredRoads"))
	AAIBase* BaseAI = Cast<AAIBase>(InActor);
	if(BaseAI == nullptr) return false;
	UE_LOG(LogTemp, Warning, TEXT("BaseAI ok"))
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
		return true;
	}
	
	FRoadInfo StartRoadInfo;
	FRoadInfo EndRoadInfo;
	for(FRoadInfo& RoadInfo : RoadInfos)
	{
		if (RoadInfo.RoadSpline.Get() == StartRoad)
		{
			UE_LOG(LogTemp, Warning, TEXT("== StartRoad"))
			StartRoadInfo = RoadInfo;
		}
		if (RoadInfo.RoadSpline.Get() == EndRoad)
		{
			UE_LOG(LogTemp, Warning, TEXT("== EndRoad"))
			EndRoadInfo = RoadInfo;
		}
		if (StartRoadInfo.RoadSpline != nullptr && EndRoadInfo.RoadSpline != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("StartRoadInfo != nullptr && EndRoadInfo != nullptr"))
			break;
		}
	}
	
	if (StartRoadInfo.RoadSpline == nullptr || EndRoadInfo.RoadSpline == nullptr) return false;

	UE_LOG(LogTemp, Warning, TEXT("StartRoadInfo != nullptr && EndRoadInfo != nullptr"))
	// First FRoadInfo* is the road that was visited and the second one in the array is the FRoadInfo* that the road was visited from
	TMap<FRoadInfo, FRoadInfo> VisitedRoads;
	TQueue<FRoadInfo> RoadsToVisit;
	TArray<FRoadInfo> OutRoads;

	RoadsToVisit.Enqueue(StartRoadInfo);
	VisitedRoads.Add(StartRoadInfo, StartRoadInfo);
	bool bFoundRoute = false;
	for(auto& Entry : RoadConnectionMap)
	{
		if(Entry.Key.RoadSpline.Get()) UE_LOG(LogTemp, Warning, TEXT("RoadConnectionMap: RoadName: %s"), *Entry.Key.RoadSpline.Get()->GetName())
	}
	while(!RoadsToVisit.IsEmpty())
	{
		FRoadInfo CurrentRoad;
		RoadsToVisit.Dequeue(CurrentRoad);
		if(CurrentRoad.RoadSpline) UE_LOG(LogTemp, Warning, TEXT("CurrentRoad Name: %s"), *CurrentRoad.RoadSpline->GetName())
		
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
			UE_LOG(LogTemp, Warning, TEXT("bContains"))
			
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
						UE_LOG(LogTemp, Warning, TEXT("!VisitedRoads: RoadInfo: %s, EndRoadInfo: %s"), *RoadInfo.RoadSpline.Get()->GetName(), *EndRoadInfo.RoadSpline.Get()->GetName())
						RoadsToVisit.Enqueue(RoadInfo);
						VisitedRoads.Add(RoadInfo, CurrentRoad);
						if(RoadInfo.RoadSpline.Get() == EndRoadInfo.RoadSpline.Get())
						{
							UE_LOG(LogTemp, Warning, TEXT("RoadInfo.RoadSpline.Get() == EndRoadInfo.RoadSpline.Get()"))
							OutRoads.AddUnique(RoadInfo);
							FRoadInfo& RoadToAdd = RoadInfo;
							while(RoadToAdd.RoadSpline.Get() != StartRoadInfo.RoadSpline.Get())
							{
								UE_LOG(LogTemp, Warning, TEXT("RoadToAdd: %s, StartRoadInfo: %s"), *RoadToAdd.RoadSpline.Get()->GetName(), *StartRoadInfo.RoadSpline.Get()->GetName())
								OutRoads.AddUnique(VisitedRoads[RoadToAdd]);
								RoadToAdd = VisitedRoads[RoadToAdd];
							}
							bFoundRoute = true;
						}//else UE_LOG(LogTemp, Warning, TEXT("RoadInfo != EndRoadInfo"))
					}else UE_LOG(LogTemp, Warning, TEXT("Contains:  %s"), *RoadInfo.RoadSpline.Get()->GetName())
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
	
	OutRoadInfos = OutRoads;
	return false;
	
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
			if(Temp.RoadSpline) UE_LOG(LogTemp, Warning, TEXT("Road to be added: %s"), *Temp.RoadSpline->GetName())
			else UE_LOG(LogTemp, Warning, TEXT("Road spline was nullptr"))
			RoadInfos.Add(Temp);
		}
		for (FRoadInfo RoadInfo : TempRoadInfos)
		{
			
			TArray<FRoadInfo> ConnectedRoads;
			FRoadInfoContainer RoadInfoContainer;
			for (TSoftObjectPtr<ARoadSpline>& RoadSpline : RoadInfo.RoadConnections.RoadSplines)
			{
				UE_LOG(LogTemp, Warning, TEXT("RoadInfo: %s, Connected Road: %s"), *RoadInfo.RoadSpline->GetName(), *RoadSpline.Get()->GetName())
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
		if(Entry.Key.RoadSpline.Get()) UE_LOG(LogTemp, Warning, TEXT("RoadConnectionMap: RoadName: %s"), *Entry.Key.RoadSpline.Get()->GetName())
	}
	
	return true;
	
}
