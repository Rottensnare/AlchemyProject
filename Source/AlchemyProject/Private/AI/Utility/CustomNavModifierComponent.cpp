// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/CustomNavModifierComponent.h"

void UCustomNavModifierComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NAME_Cost = FName("Cost");
	static const FName NAME_AreaEnteringCost = FName("AreaEnteringCost");
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
