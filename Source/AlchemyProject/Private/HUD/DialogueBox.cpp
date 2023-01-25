// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DialogueBox.h"

#include "Components/ListView.h"
#include "Components/RichTextBlock.h"
#include "HUD/DialogueBoxElement.h"

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

void UDialogueBox::EmptyListView()
{
	DialogueListView->ClearListItems();
}
