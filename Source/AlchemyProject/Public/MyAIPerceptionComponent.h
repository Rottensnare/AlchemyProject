// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "MyAIPerceptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSightStimulusExpired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHearingStimulusExpired);

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UMyAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:

	FOnSightStimulusExpired OnSightStimulusExpired;
	FOnHearingStimulusExpired OnHearingStimulusExpired;

protected:

	virtual void HandleExpiredStimulus(FAIStimulus& StimulusStore) override;
	
};
