// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemySelectionElement.h"

#include "AlchemyProject/PlayerCharacter.h"
#include "AlchemyProject/PlayerController/MyPlayerController.h"

void UAlchemySelectionElement::OnButtonPressed(const FString& RecipeName)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter == nullptr) return;
	AMyPlayerController* TempController = Cast<AMyPlayerController>(PlayerCharacter->GetController());
	if(TempController)
	{
		TempController->FindIngredients(FName(RecipeName));
	}
}
