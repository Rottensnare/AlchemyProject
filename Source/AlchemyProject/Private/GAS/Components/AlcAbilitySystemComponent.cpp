// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Components/AlcAbilitySystemComponent.h"

#include "GAS/GameplayAbility/AlcGameplayAbility.h"

void UAlcAbilitySystemComponent::CheckAbilityRangeRequirements()
{
	ABILITYLIST_SCOPE_LOCK()
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		UAlcGameplayAbility* AlcAbility = Cast<UAlcGameplayAbility>(AbilitySpec.Ability);
		if(AlcAbility)
		{
			if(AlcAbility->bHasRangeRequirements == false) continue;
			
			if(DistanceToTarget < AlcAbility->MinRange)
			{
				AlcAbility->AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.TooClose")));
				AlcAbility->AbilityTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.TooFar")));
				AlcAbility->AbilityTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.InRange")));
			}
			else if(DistanceToTarget > AlcAbility->MaxRange)
			{
				AlcAbility->AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.TooFar")));
				AlcAbility->AbilityTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.TooClose")));
				AlcAbility->AbilityTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.InRange")));
			}
			else
			{
				AlcAbility->AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.InRange")));
				AlcAbility->AbilityTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.TooClose")));
				AlcAbility->AbilityTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.TooFar")));
			}
		}
	}
}
