// Fill out your copyright notice in the Description page of Project Settings.


#include "TestInvisibilityPotion.h"

#include "AlchemyProject/PlayerCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

void UTestInvisibilityPotion::BeginPlay()
{
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : CurrentCharacter;
	if(CurrentCharacter == nullptr) return;
	FTimerDelegate TimerDelegate1;
	TimerDelegate1.BindUFunction(this, FName("ExecuteFunctionality"));
	CurrentCharacter->GetWorldTimerManager().SetTimerForNextTick(TimerDelegate1);

	
	Super::BeginPlay();
}

void UTestInvisibilityPotion::MakePlayerVisible()
{
	if(CurrentCharacter == nullptr) return;
	
	CurrentCharacter->GetMesh()->SetVisibility(true);
	CurrentCharacter->GetPerceptionStimuliSourceComponent()->RegisterForSense(UAISense_Sight::StaticClass());

	FTimerDelegate TimerDelegate2;
	TimerDelegate2.BindUFunction(this, FName("DestroyThisComponent"));
	CurrentCharacter->GetWorldTimerManager().SetTimerForNextTick(TimerDelegate2);
	

}

void UTestInvisibilityPotion::ExecuteFunctionality()
{
	if(CurrentCharacter == nullptr) return;

	CurrentCharacter->GetMesh()->SetVisibility(false);

	InvisibilityDuration += (uint8)ProductQuality - 1;

	CurrentCharacter->GetWorldTimerManager().SetTimer(InvisibilityTimer, this, &UTestInvisibilityPotion::MakePlayerVisible, InvisibilityDuration);
	CurrentCharacter->GetPerceptionStimuliSourceComponent()->UnregisterFromSense(UAISense_Sight::StaticClass());
	
	Super::ExecuteFunctionality();
}
