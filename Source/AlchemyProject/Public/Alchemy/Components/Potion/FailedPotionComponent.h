// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProject/Alchemy/Components/Potion/PotionComponent.h"
#include "FailedPotionComponent.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UFailedPotionComponent : public UPotionComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void DestroyThisComponent() override;
	virtual void ExecuteFunctionality() override;
};
