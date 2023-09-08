// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/GOAP/GoalTypes.h"
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

	virtual void CalculateGoalRelevancy() {};
	FORCEINLINE float GetGoalRelevancy() const {return GoalRelevancy;}
	virtual void InitGoal(AAgentBase* InAgent, EGoalType InGoalType);
	virtual bool IsWorldStateSatisfied(const UWorldState* WS) {return false;}
	virtual void SetWorldStateSatisfaction(UWorldState* WS) {};
	virtual bool ReplanRequired() {return false;}
	virtual void ActivateGoal();
	bool					 bForceReplan = false;
	
protected:
	
	TObjectPtr<AAgentBase>	 Agent;
	EGoalType				 GoalType;
	float					 RecalculationFrequency; /**TODO: Edit from editor	 */	
	float					 GoalRelevancy;			 /**TODO: Edit from editor	 */	
	double					 ActivationTime;
	
	
private:



public:
};
