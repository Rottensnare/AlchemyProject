// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyItem.h"

void AAlchemyItem::UpdatePopUp()
{
	FString Quality = FString::Printf(TEXT("%s"), *UEnum::GetDisplayValueAsText(IngredientData.IngredientQuality).ToString());
	FString TempString("");
	Quality.Split(" ", &Quality, &TempString, ESearchCase::IgnoreCase);

	FString Quantity = FString::Printf(TEXT("%s"), *UEnum::GetDisplayValueAsText(IngredientData.IngredientQuantityValue).ToString());
	Quantity.Split(" ", &Quantity, &TempString, ESearchCase::IgnoreCase);

	const FString PrimarySub = FString::Printf(TEXT("%s"), *UEnum::GetDisplayValueAsText(IngredientData.PrimarySubstance).ToString());
	FString ClassName("");
	if(IngredientData.IngredientClass)
	{
		ClassName = IngredientData.IngredientClass->GetName();
		ClassName.Split("_", &TempString, &ClassName, ESearchCase::IgnoreCase);
		ClassName.Split("_", &ClassName, &TempString, ESearchCase::IgnoreCase);
	}  

	//TODO: Have icons for each primary and secondary substance so that they can be displayed here as image decorators
	PopUpText = FString::Printf(TEXT("<Header1>%s</>\n<Header2>Quality:</><%s>%s</>\n<Header2>Quantity value:</><%s>%s</>\n<Header2>Primary: %s</>"), *ClassName, *Quality, *Quality, *Quantity, *Quantity, *PrimarySub);
	
	Super::UpdatePopUp();
}

void AAlchemyItem::InitGameplayTagContainer()
{
	//Can only have 1 primary substance so we remove the last tag if the substance of the item is changed
	//TODO: Might need to remove all ingredient tags to be sure
	if(GameplayTagContainer.HasTag(PrimarySubstanceGameplayTag)) GameplayTagContainer.RemoveTag(PrimarySubstanceGameplayTag);
	
	const FName TagName =
		FName(FString::Printf(TEXT("Alchemy.Ingredient.%s"),
			*UEnum::GetDisplayValueAsText(IngredientData.PrimarySubstance).ToString())); //BUG: Doesn't work in release build, need to make another version
	PrimarySubstanceGameplayTag = FGameplayTag::RequestGameplayTag(TagName);
	GameplayTagContainer.AddTag(PrimarySubstanceGameplayTag);
	
	Super::InitGameplayTagContainer();
}

void AAlchemyItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//UE_LOG(LogTemp, Warning, TEXT("Changed Property Name: %s"), *PropertyChangedEvent.Property->GetFName().ToString())
	static const FName NAME_IngredientData = FName("PrimarySubstance");
	if(PropertyChangedEvent.Property)
	{
		if(PropertyChangedEvent.Property->GetFName() == NAME_IngredientData)
		{
			InitGameplayTagContainer();
		}
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}


