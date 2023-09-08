// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GOAP/Goal/AIGoal.h"

UAIGoal::UAIGoal()
	:
Agent(nullptr),
RecalculationFrequency(0.f),
GoalRelevancy(0.f),
ActivationTime(0)
{
	
}

void UAIGoal::InitGoal(AAgentBase* InAgent, EGoalType InGoalType)
{
	Agent = InAgent;
	GoalType = InGoalType;
}

void UAIGoal::ActivateGoal()
{
	ActivationTime = GetWorld()->GetTimeSeconds();
}
