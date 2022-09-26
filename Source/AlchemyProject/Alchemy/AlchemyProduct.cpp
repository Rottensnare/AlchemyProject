// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyProduct.h"

#include "Engine/UserDefinedStruct.h"

void AAlchemyProduct::Use()
{
	
}

void AAlchemyProduct::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	
}

void AAlchemyProduct::InitProperties(const FName& InName)
{
	ProductInfo.Name = InName;
	
	FString RecipeDataTablePath(TEXT("DataTable'/Game/Assets/Datatables/RecipeDataTable.RecipeDataTable'"));
	UDataTable* RecipeTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RecipeDataTablePath));
	if(!RecipeTableObject) return;
	
	FRecipeTable* RecipeDataRow = nullptr;
	RecipeDataRow = RecipeTableObject->FindRow<FRecipeTable>(ProductInfo.Name, TEXT(""));
	if(RecipeDataRow)
	{
		Recipe.AlchemyClass = RecipeDataRow->AlchemyClass;
		Recipe.RequiredBase = RecipeDataRow->RequiredBase;
		Recipe.AmountPerSubstanceMap = RecipeDataRow->AmountPerSubstanceMap;
		Recipe.BaseSuccessChance = RecipeDataRow->BaseSuccessChance;
		ItemType = EItemType::EIT_Consumable;
		ProductInfo.AlchemyClass = RecipeDataRow->AlchemyClass;
		
		//UE_LOG(LogTemp, Warning, TEXT("Recipe copy successful"))
		UE_LOG(LogTemp, Warning, TEXT("Recipe Hash as a uin32: %u"), UUserDefinedStruct::GetUserDefinedStructTypeHash(&Recipe, FRecipe::StaticStruct()));
		OnInitialized.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No such recipe exists!"))
	}

	
}
