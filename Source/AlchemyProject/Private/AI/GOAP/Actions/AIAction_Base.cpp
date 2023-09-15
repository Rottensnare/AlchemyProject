// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GOAP/Actions/AIAction_Base.h"

bool UAIAction_Base::CanSolveWSGoal(AAgentBase* InAgent, UWorldState* CurrentWS, UWorldState* GoalWS)
{
	constexpr uint8 EffectNum = static_cast<uint8>(EWorldStateEffect::EWSE_MAX);
	for(uint8 i = 0; i < EffectNum; i++)
	{
		const FWorldStateProperty* PropEffect = WSEffects.Get()->GetWSProp(i);
		if(PropEffect == nullptr) continue;

		const FWorldStateProperty* PropCurrent = CurrentWS->GetWSProp(PropEffect->WSEffect);
		const FWorldStateProperty* PropGoal = GoalWS->GetWSProp(PropEffect->WSEffect);

		if(PropCurrent && PropGoal)
		{
			//TODO: Modify this
			if(PropCurrent->WSType != PropGoal->WSType || PropCurrent->WS_Object != PropGoal->WS_Object)
			{
				return true;
			}
		}
	}

	return false;
}
