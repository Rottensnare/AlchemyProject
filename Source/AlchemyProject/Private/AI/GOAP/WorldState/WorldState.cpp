// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GOAP/WorldState/WorldState.h"

#include "BehaviorTree/BlackboardComponent.h"

UWorldState::UWorldState()
{
	WorldBlackboard = CreateDefaultSubobject<UBlackboardComponent>("WorldBlackboard");
	
}

FWorldStateProperty* UWorldState::GetWSProp(const uint8 Flag)
{
	if(Flag >= WSEffectNum)
	{
		return nullptr;
	}

	if(EffectFlags.test(Flag))
	{
		return &WSPropList[Flag];
	}

	return nullptr;
}

FWorldStateProperty* UWorldState::GetWSProp(const EWorldStateEffect Effect)
{
	const uint8 Flag = static_cast<uint8>(Effect);
	if(EffectFlags.test(Flag))
	{
		return &WSPropList[Flag];
	}

	return nullptr;
}
