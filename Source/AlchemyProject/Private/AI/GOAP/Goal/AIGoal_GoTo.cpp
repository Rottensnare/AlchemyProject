// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GOAP/Goal/AIGoal_GoTo.h"

#include "AI/GOAP/Character/AgentBase.h"
#include "FunctionLibraries/CustomGasLibrary.h"

bool UAIGoal_GoTo::IsWorldStateSatisfied(const UWorldState* WS)
{
	//TODO: Figure out how to save different types of data and check if WS is a ok.
	if(Agent == nullptr)
	{
		return true;
	}
	
	return UCustomGasLibrary::IsCloseEnough(Agent->GetActorLocation(), TargetLocation, Threshold);
}
