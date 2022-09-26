// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionComponent.h"

#include "AlchemyProject/PlayerCharacter.h"


UPotionComponent::UPotionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	
}


void UPotionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UPotionComponent::ExecuteFunctionality()
{
	
}

void UPotionComponent::DestroyThisComponent()
{
	DestroyComponent();
}

void UPotionComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : CurrentCharacter;
	if(CurrentCharacter)
	{
		CurrentCharacter->CurrentPotionComponents.Remove(this);
	}
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}


void UPotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

