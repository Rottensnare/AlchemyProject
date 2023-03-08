// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/AlcGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AI/AIBase.h"
#include "GAS/Components/AlcAbilitySystemComponent.h"

UAlcGameplayAbility::UAlcGameplayAbility():
bHasRangeRequirements(false)
{
	
}

bool UAlcGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	
	if(bHasRangeRequirements)
	{
		UAlcAbilitySystemComponent* AlcAbilitySystemComponent = Cast<UAlcAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
		
		if(AlcAbilitySystemComponent)
		{
			const FGameplayAbilitySpec* Spec = AlcAbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
			if(Spec && Spec->Ability && !Spec->Ability->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.InRange"))))
			{
				return false;
			}
		}
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}



