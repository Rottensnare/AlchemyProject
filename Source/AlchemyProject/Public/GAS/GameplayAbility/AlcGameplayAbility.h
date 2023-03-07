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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Range")
	bool bHasRangeRequirements;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Range", meta = (EditCondition="bHasRangeRequirements"))
	float MinRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Range", meta = (EditCondition="bHasRangeRequirements"))
	float PreferredRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Range", meta = (EditCondition="bHasRangeRequirements"))
	float MaxRange;

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	UFUNCTION(BlueprintCallable)
	virtual void SetTarget(AActor* AvatarActor) const;
};
