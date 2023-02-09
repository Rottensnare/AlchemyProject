// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/RoadSpline.h"

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


