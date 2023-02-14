// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadSpline.generated.h"

USTRUCT(BlueprintType)
struct FActorNavPackage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 MoveDir = 0;
	
	// At what spline index do we switch to the next road
	UPROPERTY(BlueprintReadOnly)
	int32 SwitchRoadSplineIndex = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 NextRoadMoveDir = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 NextRoadSplineIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bFollowingRoad = false;

	FActorNavPackage(int32 InMoveDir, int32 InSplineIndex, int32 InNextMoveDir) {MoveDir = InMoveDir; SwitchRoadSplineIndex = InSplineIndex; NextRoadMoveDir = InNextMoveDir;}
	FActorNavPackage(){};
};

/**	This is used for calculating the optimal route */
USTRUCT()
struct FRoadNavigationData
{
	GENERATED_BODY()

	UPROPERTY()
	class ARoadSpline* NearestRoadToDestination{nullptr};

	UPROPERTY()
	ARoadSpline* NearestRoadToActor{nullptr};

	int32 NearestIndexToActor = 0;
	int32 NearestIndexToDestination = 0;

	
};

// NOTE Move most of the logic to the new NavigationManager class. 

UCLASS()
class ALCHEMYPROJECT_API ARoadSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ARoadSpline();

protected:
	
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URoadSplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ARoadSpline*> RoadConnectionsToBeAdded;

	int32 CalculateMoveDirection(class AAIBase* const AIBase, const int32 NextRoadIndex);
	
public:
	
	FORCEINLINE URoadSplineComponent* GetSplineComponent() const {return SplineComponent;}
	
	UFUNCTION(BlueprintCallable)
	FVector GetSplinePointPosition() const;
	UFUNCTION(BlueprintCallable)
	FActorNavPackage GetActorNavPackage(AActor* InActor);
	UFUNCTION(BlueprintCallable)
	FVector FindClosestSplinePoint(AActor* InActor);
	UFUNCTION(BlueprintCallable)
	FVector FindNextSplinePoint(AActor* InActor, bool& bSuccess);

	// Explanation in the Cpp file
	bool GetNextRoad(AActor* InActor);

	UPROPERTY(EditAnywhere)
	FName RoadName {NAME_None};

	//Stores the current spline point index for each actor using the road.
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> ActorSplineIndexMap;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, FActorNavPackage> ActorNavPackages;

	//DEPRECATED This has been moved to the NavigationManager Class
	static FVector GetNearestRoadSplinePoint(const AActor* const InActor, const ARoadSpline* OutRoadSpline = nullptr);
	
	TMap<ARoadSpline*, TArray<ARoadSpline*>> RoadConnections;
	

	UFUNCTION(BlueprintCallable)
	void ShowRoadConnections();



};
