// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DialogueOverlay.h"

#include "Components/Button.h"
#include "Components/ListView.h"
#include "HUD/DialogueBox.h"
#include "Managers/DialogueEventManager.h"
#include "Managers/DialogueManager.h"

bool FDialogueOptionRequirements::HasRequirements() const
{
	if(RequiredTags.IsEmpty() && ForbiddenTags.IsEmpty()) return false;
	return true;
}

bool FDialogueOptionRequirements::CheckHasAllTags(const FGameplayTagContainer InContainer) const
{
	const FGameplayTagQuery TagQuery = FGameplayTagQuery::MakeQuery_MatchAllTags(RequiredTags);
	return InContainer.MatchesQuery(TagQuery);
}

bool FDialogueOptionRequirements::CheckForbiddenTags(const FGameplayTagContainer InContainer) const
{
	const FGameplayTagQuery TagQuery = FGameplayTagQuery::MakeQuery_MatchNoTags(RequiredTags);
	return InContainer.MatchesQuery(TagQuery);
}

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
	DialogueManager->RegexPattern = RegexPatternString; //Not used since regex doesn't work //BUG
	DialogueEventManager = NewObject<UDialogueEventManager>();
	DialogueBox->DialogueOverlay = this;
	DialogueBox->OnOptionSelected.AddDynamic(this, &UDialogueOverlay::OptionSelected);
	
	Super::NativeOnInitialized();
}

void UDialogueOverlay::ExitButtonPressed()
{
	ExitButton->OnClicked.Broadcast();
}

void UDialogueOverlay::OptionSelected(FDialogueOption SelectedOption)
{
	if(DialogueBox == nullptr || DialogueBox->DialogueListView == nullptr || DialogueManager == nullptr || DialogueEventManager == nullptr) return;
	RemoveItems();

	for(const auto& Argument : SelectedOption.EventArguments)
	{
		DialogueEventManager.Get()->HandleDialogueEvent(Argument, SelectedOption.FunctionObjects);
	}
	
	
	//UE_LOG(LogTemp, Warning, TEXT("ID: %d"), SelectedOption.NextDialogueStateID)
	if(SelectedOption.NextDialogueStateID == 0)
	{
		DialogueManager->EndDialogue();
		return;
	}
	
	if(SelectedOption.NextDialogueStateID == -1 && (DialogueManager->GetPreviousDialogueStateID() > 0))
	{
		
		DialogueManager->StartDialogue(DialogueManager->GetPreviousDialogueStateID());
	}
	else
	{
		DialogueManager->StartDialogue(SelectedOption.NextDialogueStateID);
	}

	bool bWasSuccess = false;
	TArray<UObject*> Objects = DialogueEventManager->GetDialogueObjects("TestRow", bWasSuccess);
	if(bWasSuccess)
	{
		for(const auto& Object : Objects)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Object Name is %s"), *Object->GetName())
		}
	}
	
	DialogueBox->OnOptionsUpdated(Player);
}

void UDialogueOverlay::RemoveItems()
{
	for(const auto& Item : DialogueBox->DialogueListView->GetListItems())
	{
		DialogueBox->DialogueListView->RemoveItem(Item);
	}
}

