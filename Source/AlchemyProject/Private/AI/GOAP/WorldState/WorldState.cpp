// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GOAP/WorldState/WorldState.h"

#include "BehaviorTree/BlackboardComponent.h"

UWorldState::UWorldState()
{
	WorldBlackboard = CreateDefaultSubobject<UBlackboardComponent>("WorldBlackboard");
}
