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
	
	const FString RecipeDataTablePath(TEXT("DataTable'/Game/Assets/Datatables/RecipeDataTable.RecipeDataTable'"));
	const UDataTable* RecipeTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RecipeDataTablePath));
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

void AAlchemyProduct::BeginPlay()
{
	Super::BeginPlay();
}

void AAlchemyProduct::UpdatePopUp()
{
	//Setting the Popup text
	FString Quality = FString::Printf(TEXT("%s"), *UEnum::GetDisplayValueAsText(ProductInfo.ProductQuality).ToString());
	FString TempString("");
	Quality.Split(" ", &Quality, &TempString, ESearchCase::IgnoreCase);
	PopUpText = FString::Printf(TEXT("<Header1>%s</>\n<Header2>Quality:</><%s>%s</>\n"), *ProductInfo.Name.ToString(), *Quality, *Quality);
	//UE_LOG(LogTemp, Warning, TEXT("Quality: %s"), *Quality)
	
	Super::UpdatePopUp();
}
