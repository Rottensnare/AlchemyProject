// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/GOAP/Goal/AIGoal.h"
#include "AIGoal_GoTo.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UAIGoal_GoTo : public UAIGoal
{
	GENERATED_BODY()

public:

	virtual bool IsWorldStateSatisfied(const UWorldState* WS) override;

protected:

	float Threshold = 100.f;
	FVector TargetLocation = FVector::ZeroVector;
};
