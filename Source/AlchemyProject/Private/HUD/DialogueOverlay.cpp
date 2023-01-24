// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DialogueOverlay.h"

#include "Components/Button.h"
#include "Managers/DialogueManager.h"

void FDialogueOption::AssignID()
{
	//TODO: Check the DataTable and see if there are more than one instances of the same ID. If so, generate a new one that is available.
}

void FDialogueOption::HandlePlayerChoice()
{
	int32 Iterator = 0;
	for(const FName FuncName : FunctionNames)
	{
		if(FunctionObjects.IsValidIndex(Iterator))
		{
			FunctionObjects[Iterator]->ProcessEvent(FunctionObjects[Iterator]->FindFunction(FuncName), nullptr);
		}
		Iterator++;
	}
}

void FDialogueState::AssignID()
{
	//TODO: Check the DataTable and see if there are more than one instances of the same ID. If so, generate a new one that is available.
}

void UDialogueOverlay::NativeOnInitialized()
{
	DialogueManager = NewObject<UDialogueManager>();
	
	Super::NativeOnInitialized();
}

void UDialogueOverlay::ExitButtonPressed()
{
	ExitButton->OnClicked.Broadcast();
	
}
