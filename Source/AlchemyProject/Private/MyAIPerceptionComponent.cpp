// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIPerceptionComponent.h"

void UMyAIPerceptionComponent::HandleExpiredStimulus(FAIStimulus& StimulusStore)
{
	if(StimulusStore.Type.Name.ToString().Contains("Sight"))
	{
		OnSightStimulusExpired.Broadcast();
	}
	else if(StimulusStore.Type.Name.ToString().Contains("Hearing"))
	{
		OnHearingStimulusExpired.Broadcast();
	}
	
	Super::HandleExpiredStimulus(StimulusStore);
}
