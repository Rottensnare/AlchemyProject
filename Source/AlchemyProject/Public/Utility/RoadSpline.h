// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SplineComponentVisualizer.h"
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

	
public:
	
	FORCEINLINE URoadSplineComponent* GetSplineComponent() const {return SplineComponent;}
	
	UFUNCTION(BlueprintCallable)
	FVector GetSplinePointPosition() const; 

};
