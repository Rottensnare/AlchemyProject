// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/AlcAttributeSet.h"

#include "Net/UnrealNetwork.h"

UAlcAttributeSet::UAlcAttributeSet()
{
	
}


void UAlcAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlcAttributeSet, Health, OldHealth);
}

void UAlcAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAlcAttributeSet, AttackPower, OldAttackPower);
}

void UAlcAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAlcAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAlcAttributeSet, AttackPower, COND_None, REPNOTIFY_Always)
}
