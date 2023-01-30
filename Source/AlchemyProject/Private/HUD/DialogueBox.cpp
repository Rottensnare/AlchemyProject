// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DialogueBox.h"

#include "Components/ListView.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/RichTextBlock.h"
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
		UE_LOG(LogTemp, Warning, TEXT("Element string: %s"), *Option)
		UDialogueBoxElement* DialogueBoxElement = Cast<UDialogueBoxElement>(CreateWidget(this, DialogueBoxElementClass));
		//DialogueBoxElement->RichTextBlock->SetText(FText::FromString(FString::Printf(TEXT("<Header2>%s</>"), *Option)));
		DialogueListView->AddItem(DialogueBoxElement);
	}
	
}

void UDialogueBox::AddToListView(TMap<int32, FDialogueOption>& InDialogueOptions)
{
	DialogueOptions.Empty();
	
	for(const auto& Option : InDialogueOptions)
	{
		UDialogueBoxElement* DialogueBoxElement = Cast<UDialogueBoxElement>(CreateWidget(this, DialogueBoxElementClass));
		if(!DialogueBoxElement) continue;
		DialogueOptions.Add(Option.Value);
		UE_LOG(LogTemp, Warning, TEXT("AddToListView: Key: %d TEXT: %s"), Option.Key, *Option.Value.TextToDisplay.ToString())
		DialogueBoxElement->DialogueOption = Option.Value;
		DialogueListView->AddItem(DialogueBoxElement);
	}
}

void UDialogueBox::EmptyListView()
{
	DialogueListView->ClearListItems();
}

void UDialogueBox::OptionSelected(const FDialogueOption SelectedOption) const
{
	OnOptionSelected.Broadcast(SelectedOption);
}

void UDialogueBox::OnOptionsUpdated()
{
	if(DialogueOverlay && DialogueOverlay->DialogueManager && DialogueOverlay->MultiLineTextBox)
	{
		AddToListView(DialogueOverlay->DialogueManager->GetCurrentDialogueOptions());
		DialogueOverlay->MultiLineTextBox->SetText(FText::FromString(DialogueOverlay->DialogueManager->GetNPCDialogue()));
	}
}
