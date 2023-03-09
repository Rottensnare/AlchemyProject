// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Components/AlcAbilitySystemComponent.h"

#include "AI/AIBase.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "GAS/GameplayAbility/AlcGameplayAbility.h"

void UAlcAbilitySystemComponent::CheckAbilityRangeRequirements()
{
	ABILITYLIST_SCOPE_LOCK()
	for(const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
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
//NOTE: Should be called before trying to activate an ability
void UAlcAbilitySystemComponent::SetTargetAndCheckRange()
{
	const AAIBase* AIActor = Cast<AAIBase>(GetAvatarActor());
	
	if(AIActor == nullptr)
	{
		const APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActor());
		if(Player)
		{
			if(Player->GetTargetActor())
			{
				SetTargetActor(Player->GetTargetActor());
				DistanceToTarget = FVector::Distance(GetAvatarActor()->GetActorLocation(), Player->GetTargetActor()->GetActorLocation());
				CheckAbilityRangeRequirements();
			}
		}
		
		return;
	}

	AActor* CurrentTarget = Cast<AActor>(AIActor->GetBaseAIController()->GetAIBlackboardComponent()->GetValueAsObject(FName("Target")));
	if(CurrentTarget)
	{
		SetTargetActor(CurrentTarget);
		DistanceToTarget = FVector::Distance(GetAvatarActor()->GetActorLocation(), CurrentTarget->GetActorLocation());
		CheckAbilityRangeRequirements();
	}
	
}

//NOTE: Should be called after "SetTargetAndCheckRange"
TArray<FGameplayAbilitySpecHandle> UAlcAbilitySystemComponent::GetAbilitiesInRange()
{
	TArray<FGameplayAbilitySpecHandle> InRangeAbilities;
	
	ABILITYLIST_SCOPE_LOCK()
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Ability.Requirement.InRange"))))
		{
			InRangeAbilities.AddUnique(AbilitySpec.Handle);
		}
	}
	
	return InRangeAbilities;
}
