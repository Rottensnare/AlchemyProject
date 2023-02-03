// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/AIInteractionComponent.h"


UAIInteractionComponent::UAIInteractionComponent()
{

	PrimaryComponentTick.bCanEverTick = false;


}



void UAIInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}



void UAIInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

bool UAIInteractionComponent::ExecuteFunctionality_Implementation()
{
	return true;
}



