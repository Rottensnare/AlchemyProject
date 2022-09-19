// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyOverlay.h"

#include "AlchemyProject/HUD/InfoBox.h"
#include "AlchemyProject/HUD/InventoryWidget.h"
#include "Components/MultiLineEditableText.h"

void UAlchemyOverlay::NativeOnInitialized()
{
	TableInventory->CreateContainerGrid(9);
	TableInventory->SetVisibility(ESlateVisibility::Visible);
	
	Super::NativeOnInitialized();
}

void UAlchemyOverlay::UpdateInfoBox(TArray<FIngredientInfo>& IngredientInfos)
{
	FString DescriptionString{"Primary: \n"};
	for(auto& InInfo : IngredientInfos)
	{
		if(InInfo.PrimarySubstance != EPrimarySubstance::EPS_None)
		{
			DescriptionString.Append(FString::Printf(TEXT("%s \n"), *UEnum::GetDisplayValueAsText(InInfo.PrimarySubstance).ToString()));
		}
	}
	DescriptionString.Append("Secondary: \n");
	for(auto& InInfo : IngredientInfos)
	{
		if(InInfo.SecondarySubstance != ESecondarySubstance::ESS_None)
		{
			DescriptionString.Append(FString::Printf(TEXT("%s \n"), *UEnum::GetDisplayValueAsText(InInfo.SecondarySubstance).ToString()));
		}
	}
	ProductInfoBox->DescriptionTextBox->SetText(FText::FromString(DescriptionString));
	//ProductInfoBox->DescriptionTextBox->SetText(FText::FromString());
}
