// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GOAP/Goal/AIGoal_Idle.h"

#include "AI/GOAP/WorldState/WorldState.h"

bool UAIGoal_Idle::IsWorldStateSatisfied(const UWorldState* WS)
{
	// Needs to be false always since this is the last resort goal.
	return false;
}
