// Fill out your copyright notice in the Description page of Project Settings.


#include "TestHealthPotion.h"

#include "AlchemyProject/HealthComponent.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "AlchemyProject/HUD/PlayerHUD.h"
#include "AlchemyProject/PlayerController/MyPlayerController.h"

void UTestHealthPotion::BeginPlay()
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

void UTestHealthPotion::DestroyThisComponent()
{
	Super::DestroyThisComponent();
}

void UTestHealthPotion::ExecuteFunctionality()
{
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : CurrentCharacter;
	if(CurrentCharacter)
	{
		float HealthToAdd = 0.f;
		if(QualityCurve)
		{
			HealthToAdd = QualityCurve->GetFloatValue(static_cast<uint8>(ProductQuality));
		}
		else
		{
			HealthToAdd = FMath::Clamp(InstantHealAmount * ((1 + (0.5f * static_cast<uint8>(ProductQuality)))), 0, 10000);
		}
		//Heal Player
		CurrentCharacter->GetHealthComponent()->SetHealth(
		FMath::Clamp(
			CurrentCharacter->GetHealthComponent()->GetHealth() + HealthToAdd
			, 0.f
			, CurrentCharacter->GetHealthComponent()->GetMaxHealth()));

		//Update HUD
		if(const AMyPlayerController* const TempController = Cast<AMyPlayerController>(CurrentCharacter->Controller))
		{
			if(APlayerHUD* const TempHUD = Cast<APlayerHUD>(TempController->GetHUD()))
			{
				TempHUD->UpdateHealthBar(CurrentCharacter->GetHealthComponent()->GetHealth(), CurrentCharacter->GetHealthComponent()->GetMaxHealth());
			}
		}
		
		//DestroyComponent can't be executed within the same frame, se we wait for the next frame
		FTimerDelegate TimerDelegate2;
		TimerDelegate2.BindUFunction(this, FName("DestroyThisComponent"));
		CurrentCharacter->GetWorldTimerManager().SetTimerForNextTick(TimerDelegate2);
	}
}


