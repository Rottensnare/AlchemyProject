// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DialogueBox.h"

#include "AlchemyProject/PlayerCharacter.h"
#include "Components/ListView.h"
#include "Components/MultiLineEditableTextBox.h"
#include "HUD/DialogueBoxElement.h"
#include "Managers/DialogueManager.h"

void UDialogueBox::SetSelectedElement(const int32 ElementIndex)
{
	if(ElementArray.IsValidIndex(ElementIndex))
	{
		SelectedElement = ElementArray[ElementIndex];
	}
}

void UDialogueBox::AddToListView(TArray<UDialogueBoxElement>& Options)
{
	
}

void UDialogueBox::AddToListView(TArray<FString>& Options)
{
	if(DialogueBoxElementClass == nullptr || Options.IsEmpty()) return;
	OptionStrings = Options;
	for(auto& Option : Options)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Element string: %s"), *Option)
		UDialogueBoxElement* DialogueBoxElement = Cast<UDialogueBoxElement>(CreateWidget(this, DialogueBoxElementClass));
		//DialogueBoxElement->RichTextBlock->SetText(FText::FromString(FString::Printf(TEXT("<Header2>%s</>"), *Option)));
		DialogueListView->AddItem(DialogueBoxElement);
	}
	
}

void UDialogueBox::AddToListView(TMap<int32, FDialogueOption>& InDialogueOptions, APlayerCharacter* InCharacter)
{
	DialogueOptions.Empty();
	
	for(const auto& Option : InDialogueOptions)
	{
		if(Option.Value.Requirements.HasRequirements())
		{
			//If player doesn't have the required tags skip the dialogue option, meaning it won't be shown.
			if(!Option.Value.Requirements.CheckHasAllTags(InCharacter->GetGameplayTags())) continue;
			//Same but if player has a forbidden tag, then skip the option
			if(Option.Value.Requirements.CheckForbiddenTags(InCharacter->GetGameplayTags())) continue;
		}
		UDialogueBoxElement* DialogueBoxElement = Cast<UDialogueBoxElement>(CreateWidget(this, DialogueBoxElementClass));
		if(!DialogueBoxElement) continue;
		DialogueOptions.Add(Option.Value);
		//UE_LOG(LogTemp, Warning, TEXT("AddToListView: Key: %d TEXT: %s"), Option.Key, *Option.Value.TextToDisplay.ToString())
		DialogueBoxElement->DialogueOption = Option.Value;
		DialogueListView->AddItem(DialogueBoxElement);
	}
}

void UDialogueBox::EmptyListView()
{
	DialogueListView->ClearListItems();
}

void UDialogueBox::OptionSelected(const FDialogueOption SelectedOption ) const
{
	OnOptionSelected.Broadcast(SelectedOption);
}

void UDialogueBox::OnOptionsUpdated(APlayerCharacter* InCharacter)
{
	if(DialogueOverlay && DialogueOverlay->DialogueManager && DialogueOverlay->MultiLineTextBox)
	{
		AddToListView(DialogueOverlay->DialogueManager->GetCurrentDialogueOptions(), InCharacter);
		DialogueOverlay->MultiLineTextBox->SetText(FText::FromString(DialogueOverlay->DialogueManager->GetNPCDialogue()));
	}
}
