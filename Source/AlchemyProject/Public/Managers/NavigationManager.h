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

	/**
	 *	Calculates roads AI needs to take to reach EndRoad and saves them
	 *
	 *	Fills in the OutRoadInfos with the required roads. Sets road names for the AI that it needs to take.
	 *	Uses BFS for searching the "shortest" path.
	 *	NOTE: Needs to be reworked, or have other versions.
	 *
	 *	@param InActor		AI we are calculating the required roads for
	 *	@param StartRoad	First road AI needs to take
	 *	@param EndRoad		Last road AI needs to take
	 *	@param OutRoadInfos	Info for roads that gets filled
	 *
	 *	@returns boolean if failed or succeeded
	 */
	UFUNCTION(BlueprintCallable)
	bool CalculateRequiredRoads(class AAIBase* InActor, ARoadSpline* StartRoad, ARoadSpline* EndRoad, TArray<FRoadInfo>& OutRoadInfos);

	
	bool HandleStartEndRoadsAreSame(AAIBase* BaseAI, ARoadSpline* StartRoad, TArray<FRoadInfo>& OutRoadInfos);
	
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
