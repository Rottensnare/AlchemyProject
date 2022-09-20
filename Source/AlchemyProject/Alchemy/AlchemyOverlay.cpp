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
	TMap<ESecondarySubstance, int32> SecondarySubstanceAmountMap;
	for(auto& InInfo : IngredientInfos)
	{
		if(!SecondarySubstanceAmountMap.Contains(InInfo.SecondarySubstance)) SecondarySubstanceAmountMap.Emplace(InInfo.SecondarySubstance, 1);
		else SecondarySubstanceAmountMap[InInfo.SecondarySubstance]++;
	}
	
	
	FString DescriptionString{"Primary: \n"};
	for(auto& InInfo : IngredientInfos)
	{
		if(InInfo.PrimarySubstance != EPrimarySubstance::EPS_None)
		{
			DescriptionString.Append(FString::Printf(TEXT("%s \n"), *UEnum::GetDisplayValueAsText(InInfo.PrimarySubstance).ToString()));
		}
	}
	DescriptionString.Append("Secondary: \n");
	for(auto& InInfo : SecondarySubstanceAmountMap)
	{
		if(InInfo.Key != ESecondarySubstance::ESS_None)
		{
			DescriptionString.Append(FString::Printf(TEXT("%s  X%d\n"), *UEnum::GetDisplayValueAsText(InInfo.Key).ToString(), InInfo.Value));
		}
	}
	ProductInfoBox->DescriptionTextBox->SetText(FText::FromString(DescriptionString));
	//ProductInfoBox->DescriptionTextBox->SetText(FText::FromString());
}
