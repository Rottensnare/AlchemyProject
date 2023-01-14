// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "MyAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UMyAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

protected:

	virtual void HandleExpiredStimulus(FAIStimulus& StimulusStore) override;
	
};
