// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/GOAP/Goal/AIGoal.h"
#include "AIGoal_Idle.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UAIGoal_Idle : public UAIGoal
{
	GENERATED_BODY()

public:


protected:

	virtual bool IsWorldStateSatisfied(const UWorldState* WS) override;

	
private:
};
