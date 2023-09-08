// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIGoal.generated.h"

class UWorldState;
class AAgentBase;
/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UAIGoal : public UObject
{
	GENERATED_BODY()

public:

	UAIGoal();

	virtual void CalculateGoalRelevancy();
	virtual bool IsWorldStateSatisfied(const UWorldState* WS) {return false;}
	virtual void SetWorldStateSatisfaction(UWorldState* WS);
	virtual bool ReplanRequired();
	virtual void ActivateGoal();
	bool					 bForceReplan = false;
	
protected:
	
	TObjectPtr<AAgentBase>	 Agent;
	float					 RecalculationFrequency;
	float					 GoalRelevancy;
	double					 ActivationTime;
	
	
private:



public:
};
