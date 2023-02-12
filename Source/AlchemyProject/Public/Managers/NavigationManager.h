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



UCLASS()
class ALCHEMYPROJECT_API UNavigationManager : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool CalculateRequiredRoads(class AAIBase* InActor, ARoadSpline* StartRoad, ARoadSpline* EndRoad, TArray<FRoadInfo>& OutRoadInfos);
	
	bool InitRoads();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<AActor*, FRoadConnectionContainer> RoadsToFollowMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FRoadInfo, FRoadInfoContainer> RoadConnectionMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FRoadInfo> RoadInfos;
	
};
