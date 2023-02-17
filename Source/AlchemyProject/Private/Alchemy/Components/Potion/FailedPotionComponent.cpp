// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemy/Components/Potion/FailedPotionComponent.h"

#include "AlchemyProject/PlayerCharacter.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"

void UFailedPotionComponent::BeginPlay()
{
	Super::BeginPlay();

	//Needs a timer because otherwise ProductQuality isn't set to correct value
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : CurrentCharacter;
	if(CurrentCharacter)
	{
		FTimerDelegate TimerDelegate1;
		TimerDelegate1.BindUFunction(this, FName("ExecuteFunctionality"));
		CurrentCharacter->GetWorldTimerManager().SetTimerForNextTick(TimerDelegate1);
	}
}

void UFailedPotionComponent::ExecuteFunctionality()
{
	UE_LOG(LogTemp, Warning, TEXT("ExecuteFunctionality"))
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : CurrentCharacter;
	if(CurrentCharacter)
	{
		if(GetWorld()->PostProcessVolumes.IsValidIndex(0))
		{
			UE_LOG(LogTemp, Warning, TEXT("ChromaticAberrationStartOffset: %f"), GetWorld()->PostProcessVolumes[0]->GetProperties().Settings->ChromaticAberrationStartOffset)
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NotValidIndex"))
		}
	}
	
	//DestroyComponent can't be executed within the same frame, so we wait for the next frame
	FTimerDelegate TimerDelegate2;
	TimerDelegate2.BindUFunction(this, FName("DestroyThisComponent"));
	CurrentCharacter->GetWorldTimerManager().SetTimerForNextTick(TimerDelegate2);
}

void UFailedPotionComponent::DestroyThisComponent()
{
	Super::DestroyThisComponent();
}


