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

	UPROPERTY(BlueprintReadWrite)
	bool bFollowingRoad = false;

	FActorNavPackage(int32 InMoveDir, int32 InSplineIndex, int32 InNextMoveDir) {MoveDir = InMoveDir; SwitchRoadSplineIndex = InSplineIndex; NextRoadMoveDir = InNextMoveDir;}
	FActorNavPackage(){};
};

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

	FName RoadName {NAME_None};

	//Stores the current spline point index for each actor using the road.
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> ActorSplineIndexMap;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> ActorMoveDirMap;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, FActorNavPackage> ActorNavPackages;
	
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

};
