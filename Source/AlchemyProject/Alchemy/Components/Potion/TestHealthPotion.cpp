// Fill out your copyright notice in the Description page of Project Settings.


#include "TestHealthPotion.h"

#include "AlchemyProject/HealthComponent.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "AlchemyProject/HUD/PlayerHUD.h"
#include "AlchemyProject/PlayerController/MyPlayerController.h"

void UTestHealthPotion::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetDisplayValueAsText(ProductQuality).ToString())
	
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : CurrentCharacter;
	if(CurrentCharacter)
	{
		//Heal Player
		CurrentCharacter->GetHealthComponent()->SetHealth(
		FMath::Clamp(
			CurrentCharacter->GetHealthComponent()->GetHealth() + FMath::Clamp(InstantHealAmount * (1 + (0.2f * (uint8)ProductQuality) - 1), 0, 10000)
			, 0.f
			, CurrentCharacter->GetHealthComponent()->GetMaxHealth()));

		//Update HUD
		AMyPlayerController* TempController = Cast<AMyPlayerController>(CurrentCharacter->Controller);
		if(TempController)
		{
			if(APlayerHUD* TempHUD = Cast<APlayerHUD>(TempController->GetHUD()))
			{
				TempHUD->UpdateHealthBar(CurrentCharacter->GetHealthComponent()->GetHealth(), CurrentCharacter->GetHealthComponent()->GetMaxHealth());
			}
		}
		
		//DestroyComponent can't be executed within the same frame, se we wait for the next frame
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("DestroyThisComponent"));
		CurrentCharacter->GetWorldTimerManager().SetTimerForNextTick(TimerDelegate);
	}
}

void UTestHealthPotion::DestroyThisComponent()
{
	Super::DestroyThisComponent();
}


