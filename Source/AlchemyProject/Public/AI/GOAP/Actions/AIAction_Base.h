// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/GOAP/WorldState/WorldState.h"
#include "UObject/NoExportTypes.h"
#include "AIAction_Base.generated.h"

class AAgentBase;
class UWorldState;

/**
 * 
 */
UCLASS(Abstract)
class ALCHEMYPROJECT_API UAIAction_Base : public UObject
{
	GENERATED_BODY()

public:

	virtual void ActivateAction(AAgentBase* InAgent, UWorldState* InWS);
	virtual float GetActionCost();
	virtual bool CanSolveWSGoal(AAgentBase* InAgent, UWorldState* CurrentWS, UWorldState* GoalWS);

protected:

	TWeakObjectPtr<UWorldState> WSPreconditions;
	TWeakObjectPtr<UWorldState> WSEffects;

public:

	FORCEINLINE TWeakObjectPtr<UWorldState> GetWSEffects() const {return WSEffects;}
};
