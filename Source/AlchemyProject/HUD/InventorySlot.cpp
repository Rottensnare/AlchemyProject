// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"

void UInventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InitIngredientTypeColors();
}

void UInventorySlot::InitIngredientTypeColors()
{

	IngredientTypeColors.Emplace(EIngredientType::EIT_Substance, FLinearColor::Green);
	IngredientTypeColors.Emplace(EIngredientType::EIT_Container, FLinearColor::Gray);
	IngredientTypeColors.Emplace(EIngredientType::EIT_Catalyst, FLinearColor::Red);
	IngredientTypeColors.Emplace(EIngredientType::EIT_Special, FLinearColor::Blue);
	IngredientTypeColors.Emplace(EIngredientType::EIT_All, FLinearColor::Transparent); //The final product
	IngredientTypeColors.Emplace(EIngredientType::EIT_Base, FLinearColor::Yellow);
	
}
