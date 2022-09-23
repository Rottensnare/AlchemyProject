// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyProduct.h"

void AAlchemyProduct::Use()
{
	
}

void AAlchemyProduct::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	
}

void AAlchemyProduct::InitProperties(const FName& InName)
{
	Name = InName;
	
	FString RecipeDataTablePath(TEXT("DataTable'/Game/Assets/Datatables/RecipeDataTable.RecipeDataTable'"));
	UDataTable* RecipeTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RecipeDataTablePath));
	if(!RecipeTableObject) return;
	
	FRecipeTable* RecipeDataRow = nullptr;
	RecipeDataRow = RecipeTableObject->FindRow<FRecipeTable>(Name, TEXT(""));
	if(RecipeDataRow)
	{
		Recipe.AlchemyClass = RecipeDataRow->AlchemyClass;
		Recipe.RequiredBase = RecipeDataRow->RequiredBase;
		Recipe.AmountPerSubstanceMap = RecipeDataRow->AmountPerSubstanceMap;
		Recipe.BaseSuccessChance = RecipeDataRow->BaseSuccessChance;
		UE_LOG(LogTemp, Warning, TEXT("Recipe copy successful"))
		OnInitialized.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No such recipe exists!"))
	}
}
