// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DialogueOverlay.h"

#include "Components/Button.h"
#include "Components/ListView.h"
#include "HUD/DialogueBox.h"
#include "Managers/DialogueEventManager.h"
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
	DialogueEventManager = NewObject<UDialogueEventManager>();
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
	RemoveItems();
	UE_LOG(LogTemp, Warning, TEXT("ID: %d"), ID)
	if(ID == 0)
	{
		DialogueManager->EndDialogue();
		return;
	}
	
	if(ID == -1 && (DialogueManager->GetPreviousDialogueStateID() > 0))
	{
		
		DialogueManager->StartDialogue(DialogueManager->GetPreviousDialogueStateID());
	}
	else
	{
		DialogueManager->StartDialogue(ID);
	}

	UE_LOG(LogTemp, Warning, TEXT("ExitButtonPressed"))
	bool bWasSuccess = false;
	TArray<UObject*> Objects = DialogueEventManager->GetDialogueObjects("TestRow", bWasSuccess);
	if(bWasSuccess)
	{
		for(const auto& Object : Objects)
		{
			UE_LOG(LogTemp, Warning, TEXT("Object Name is %s"), *Object->GetName())
		}
	}
	
	DialogueBox->OnOptionsUpdated();
}

void UDialogueOverlay::RemoveItems()
{
	for(const auto& Item : DialogueBox->DialogueListView->GetListItems())
	{
		DialogueBox->DialogueListView->RemoveItem(Item);
	}
}

