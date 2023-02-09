// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadSpline.generated.h"


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
	UPROPERTY()
	TMap<AActor*, int32> ActorSplineIndexMap;
	UPROPERTY()
	TMap<AActor*, int32> ActorMoveDirMap;
	
public:
	
	FORCEINLINE URoadSplineComponent* GetSplineComponent() const {return SplineComponent;}
	
	UFUNCTION(BlueprintCallable)
	FVector GetSplinePointPosition() const;
	UFUNCTION(BlueprintCallable)
	int32 GetMoveDirection(AActor* InActor);
	UFUNCTION(BlueprintCallable)
	FVector FindClosestSplinePoint(AActor* InActor);
	UFUNCTION(BlueprintCallable)
	FVector FindNextSplinePoint(AActor* InActor, bool& bSuccess);

};
