// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DialogueOverlay.h"

#include "Components/Button.h"
#include "Components/ListView.h"
#include "HUD/DialogueBox.h"
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

void UDialogueOverlay::NativeOnInitialized()
{
	DialogueManager = NewObject<UDialogueManager>();
	DialogueBox->DialogueOverlay = this;
	DialogueBox->OnOptionSelected.AddDynamic(this, &UDialogueOverlay::OptionSelected);
	
	Super::NativeOnInitialized();
}

void UDialogueOverlay::ExitButtonPressed()
{
	ExitButton->OnClicked.Broadcast();
	
}

void UDialogueOverlay::OptionSelected(int32 ID)
{
	if(DialogueBox == nullptr || DialogueBox->DialogueListView == nullptr || DialogueManager == nullptr) return;
	
	for(const auto& Item : DialogueBox->DialogueListView->GetListItems())
	{
		DialogueBox->DialogueListView->RemoveItem(Item);
	}
	DialogueManager->StartDialogue(ID);
	DialogueBox->OnOptionsUpdated();
}

