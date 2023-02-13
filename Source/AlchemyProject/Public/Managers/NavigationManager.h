// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProject/Alchemy/CustomStructs/NPCStructs.h"
#include "UObject/NoExportTypes.h"
#include "NavigationManager.generated.h"

/**
 * 
 */

class ARoadSpline;

USTRUCT()
struct FSplinePointLocContainer
{
	GENERATED_BODY()

	TArray<FVector> SplinePointLocations;
	
};

UCLASS()
class ALCHEMYPROJECT_API UNavigationManager : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool CalculateRequiredRoads(class AAIBase* InActor, ARoadSpline* StartRoad, ARoadSpline* EndRoad, TArray<FRoadInfo>& OutRoadInfos);
	
	bool InitRoads();

	UFUNCTION(BlueprintCallable)
	void FillAllSplinePointsMap(const AActor* const ContextActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<AActor*, FRoadConnectionContainer> RoadsToFollowMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FRoadInfo, FRoadInfoContainer> RoadConnectionMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FRoadInfo> RoadInfos;

	UPROPERTY(VisibleAnywhere)
	TMap<ARoadSpline*, FSplinePointLocContainer> AllSplinePointsMap;
	
	UFUNCTION(BlueprintCallable)
	ARoadSpline* GetNearestRoadSplinePoint(const AActor* const InActor, FVector& OutSplinePointPosition);

	UFUNCTION(BlueprintCallable)
	TMap<int32, int32> GetRoadSwitchIndex(ARoadSpline* CurrentRoad, ARoadSpline* NextRoad);
	
};
