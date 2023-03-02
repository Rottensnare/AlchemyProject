// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AlchemyProject/AlchemyProject.h"
#include "AlcGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UAlcGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UAlcGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID AbilityInputID{EGASAbilityInputID::None};

	
};
