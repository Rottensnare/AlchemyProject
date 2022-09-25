// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyScrollBox.h"

#include "AlchemySelectionElement.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UAlchemyScrollBox::UpdateInfo(const TArray<FName>& InNames)
{
	if(SelectionElementClass == nullptr) return;
	
	
	for(const FName& TempName : InNames)
	{
		if(!RecipeNames.Contains(TempName))
		{
			UAlchemySelectionElement* TempSelectionElement = CreateWidget<UAlchemySelectionElement>(this, SelectionElementClass);
			TempSelectionElement->ButtonText->SetText(FText::FromString(TempName.ToString()));
			ScrollBox->AddChild(TempSelectionElement);
			RecipeNames.AddUnique(TempName);
		}
	}
}
