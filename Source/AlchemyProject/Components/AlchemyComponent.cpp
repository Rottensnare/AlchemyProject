// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyComponent.h"

#include "AlchemyProject/InventoryComponent.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "AlchemyProject/Alchemy/AlchemyBase.h"
#include "AlchemyProject/Alchemy/AlchemyProduct.h"
#include "AlchemyProject/Alchemy/AlchemyTable.h"
#include "Components/CapsuleComponent.h"


UAlchemyComponent::UAlchemyComponent()
{

	PrimaryComponentTick.bCanEverTick = false;


}



void UAlchemyComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Character == nullptr ? Cast<APlayerCharacter>(GetOwner()) : Character;
	
}

void UAlchemyComponent::CreateAlchemyProduct(const FAlchemyPackage& AlchemyPackage)
{
	//Checking which substances are selected
	FString TempString{""};
	TArray<EPrimarySubstance> PrimarySubstances;
	TMap<ESecondarySubstance, int32> SecondarySubstances;
	for(auto& InInfo : AlchemyPackage.IngredientInfos)
	{
		if(!SecondarySubstances.Contains(InInfo.SecondarySubstance)) SecondarySubstances.Emplace(InInfo.SecondarySubstance, 1);
		else SecondarySubstances[InInfo.SecondarySubstance]++;
	}
	for(auto& InInfo : AlchemyPackage.IngredientInfos)
	{
		PrimarySubstances.AddUnique(InInfo.PrimarySubstance);
		TempString.Append(FString::Printf(TEXT("%s \n"), *UEnum::GetDisplayValueAsText(InInfo.PrimarySubstance).ToString()));
	}
	UE_LOG(LogTemp, Warning, TEXT("Selected Substances:\n %s"), *TempString)
	
	//TODO: Check known recipes that match selected ingredients

	FString RecipeDataTablePath(TEXT("DataTable'/Game/Assets/Datatables/RecipeDataTable.RecipeDataTable'"));
	UDataTable* RecipeTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RecipeDataTablePath));
	if(!RecipeTableObject) return;
	
	FRecipeTable* RecipeDataRow = nullptr;
	bool bAllSubstancesMatch{true};
	
	for(const auto& Recipe : KnownRecipeNames)
	{
		RecipeDataRow = RecipeTableObject->FindRow<FRecipeTable>(Recipe, TEXT(""));
		if(RecipeDataRow)
		{
			for(const auto& Substance : PrimarySubstances)
			{
				if(!RecipeDataRow->AmountPerSubstanceMap.Contains(Substance))
				{
					bAllSubstancesMatch = false;
					break;
				}
			}
			if(!bAllSubstancesMatch) continue;
			//Recipe was found
			UE_LOG(LogTemp, Warning, TEXT("Compatible recipe: %s"), *RecipeDataRow->AlchemyClass->GetName())
			//TODO: Create a spawn point for potions inside the Alchemy Table Class
			Character = Character == nullptr ? Cast<APlayerCharacter>(GetOwner()) : Character;
			
			Aitem = GetWorld()->SpawnActor<AAlchemyProduct>(RecipeDataRow->AlchemyClass, FVector(2500.f, 1500.f, 100.f), Character->GetActorRotation());
			Aitem->OnInitialized.AddDynamic(this, &UAlchemyComponent::AddAitemToInventory);
			Aitem->InitProperties(Recipe);
		}
	}

	
	//TODO: Check if player has required materials and then spend the resources
	//TODO: Check if player has enough inventory space, if not spawn the potion next to player anyway
}


FAlchemyPackage UAlchemyComponent::CreateAlchemyPackage(const TArray<FIngredientInfo>& InIngredientInfos,
	AAlchemyBase* InAlchemyBase)
{
	FAlchemyPackage AlchemyPackage;
	AlchemyPackage.IngredientInfos = InIngredientInfos;
	AlchemyPackage.AlchemyBase = InAlchemyBase;
	return AlchemyPackage;
}

void UAlchemyComponent::AddAitemToInventory()
{
	Character->GetInventoryComponent()->AddToInventory(Aitem, 1);
}
