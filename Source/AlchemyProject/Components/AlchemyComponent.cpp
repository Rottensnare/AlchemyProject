// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyComponent.h"

#include "AlchemyProject/InventoryComponent.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "AlchemyProject/Alchemy/AlchemyBase.h"
#include "AlchemyProject/Alchemy/AlchemyOverlay.h"
#include "AlchemyProject/Alchemy/AlchemyProduct.h"
#include "AlchemyProject/Alchemy/AlchemyTable.h"
#include "AlchemyProject/HUD/InventoryWidget.h"
#include "AlchemyProject/HUD/PlayerHUD.h"
#include "AlchemyProject/HUD/PlayerOverlay.h"
#include "AlchemyProject/PlayerController/MyPlayerController.h"


UAlchemyComponent::UAlchemyComponent()
{

	PrimaryComponentTick.bCanEverTick = false;


}



void UAlchemyComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Character == nullptr ? Cast<APlayerCharacter>(GetOwner()) : Character;
	
}

void UAlchemyComponent::FailedPotionCreation()
{
	MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(Character->GetController()) : MyPlayerController;
	if(MyPlayerController)
	{
		MyPlayerController->PlaySound(FName("FailedCreation"));
	}
}

//TODO: Divide this function to several smaller functions
void UAlchemyComponent::CreateAlchemyProduct(const FAlchemyPackage& AlchemyPackage)
{
	//Checking which substances are selected

	if(AlchemyPackage.IngredientInfos.Num() < 2)
	{
		FailedPotionCreation();
		return; //No potion exists that can be made with less than 2 different substances
	}
	
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
	//UE_LOG(LogTemp, Warning, TEXT("Selected Substances:\n %s"), *TempString)
	
	//TODO: Check known recipes that match selected ingredients

	FString RecipeDataTablePath(TEXT("DataTable'/Game/Assets/Datatables/RecipeDataTable.RecipeDataTable'"));
	UDataTable* RecipeTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RecipeDataTablePath));
	if(!RecipeTableObject) return;
	
	FRecipeTable* RecipeDataRow = nullptr;
	
	
	for(const auto& Recipe : KnownRecipeNames)
	{
		bool bAllSubstancesMatch{true};
		RecipeDataRow = RecipeTableObject->FindRow<FRecipeTable>(Recipe, TEXT(""));
		if(RecipeDataRow)
		{
			for(const auto& SubstanceMapRow : RecipeDataRow->AmountPerSubstanceMap)
			{
				if(!PrimarySubstances.Contains(SubstanceMapRow.Key))
				{
					bAllSubstancesMatch = false;
					break;
				}
			}
			if(!bAllSubstancesMatch) continue;
			
			//Recipe was found
			if(RecipeDataRow->AlchemyClass) UE_LOG(LogTemp, Warning, TEXT("Compatible recipe: %s"), *RecipeDataRow->AlchemyClass->GetName())
			//TODO: Create a spawn point for potions inside the Alchemy Table Class
			
			Character = Character == nullptr ? Cast<APlayerCharacter>(GetOwner()) : Character;
			if(Character == nullptr) return;
			
			//Check if player has required materials and then spend the resources
			TMap<EPrimarySubstance, bool> TempMap;
			TMap<int32, int32> DecreasePerIndexMap;
			TArray<EIngredientQuality> IngredientQualities;
			for(auto& Slut : Character->GetInventoryComponent()->GetInventory())
			{
				for(auto& InInfo : AlchemyPackage.IngredientInfos)
				{
					
					if(Slut.ItemClass == InInfo.IngredientClass)
					{
						//BUG: Causes a crash when selecting ingredients manually that and no recipe is found corresponding to the selected ingredients
						if(!RecipeDataRow->AmountPerSubstanceMap.Contains(Slut.IngredientInfo.PrimarySubstance))
						{
							TempMap.Emplace(EPrimarySubstance::EPS_MAX, false);
							break;
						}
						if(Slut.ItemAmount >= RecipeDataRow->AmountPerSubstanceMap[Slut.IngredientInfo.PrimarySubstance] * QuantityValue::GetQuantityValueInt(Slut.IngredientInfo.IngredientQuantityValue))
						{
							DecreasePerIndexMap.Emplace(Slut.SlotId, RecipeDataRow->AmountPerSubstanceMap[Slut.IngredientInfo.PrimarySubstance] * QuantityValue::GetQuantityValueInt(Slut.IngredientInfo.IngredientQuantityValue));
							IngredientQualities.Add(Slut.IngredientInfo.IngredientQuality);
							TempMap.Emplace(InInfo.PrimarySubstance, true);
							break;
						}
						else
						{
							TempMap.Emplace(InInfo.PrimarySubstance, false);
						}
					}
				}
			}
			//Return if player doesn't have enough resources
			for(const auto& Sub : TempMap)
			{
				if(!Sub.Value)
				{
					UE_LOG(LogTemp, Error, TEXT("Not enough %s"), *UEnum::GetDisplayValueAsText(Sub.Key).ToString())
					FailedPotionCreation();
					return;
				}
			}
			//Consume ingredients from the player's inventory
			for(const auto& Slut : DecreasePerIndexMap)
			{
				Character->GetInventoryComponent()->GetInventory()[Slut.Key].ItemAmount -= Slut.Value;
				if(Character->GetInventoryComponent()->GetInventory()[Slut.Key].ItemAmount <= 0) //Drop item if amount is 0 or less
				{
					Character->GetInventoryComponent()->DropItem(Slut.Key);
					MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(Character->GetController()) : MyPlayerController;
					if(MyPlayerController)
					{
						MyPlayerController->ClearAlchemySelection();
					}
					
				}
			}
			
			Aitem = GetWorld()->SpawnActor<AAlchemyProduct>(RecipeDataRow->AlchemyClass, Character->GetActorLocation() + Character->GetActorForwardVector() * 25.f, Character->GetActorRotation());
			Aitem->OnInitialized.AddDynamic(this, &UAlchemyComponent::AddAitemToInventory);
			
			//Calculate potion quality
			//BUG: With the current inventory system, the product quality makes no difference and will be instantly reset to default value
			Aitem->ProductQuality = ProductQuality::CalculateProductQuality(IngredientQualities);
			UE_LOG(LogTemp, Warning, TEXT("Product quality: %s"), *UEnum::GetDisplayValueAsText(Aitem->ProductQuality).ToString())
			
			Aitem->InitProperties(Recipe);
			
			MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(Character->GetController()) : MyPlayerController;
			if(MyPlayerController)
			{
				MyPlayerController->PlaySound(FName("PotionCreated"));
				if(MyPlayerController->GetPlayerHUD() && MyPlayerController->GetPlayerHUD()->AlchemyOverlay && MyPlayerController->GetPlayerHUD()->AlchemyOverlay->CharacterInventory)
				{
					MyPlayerController->GetPlayerHUD()->AlchemyOverlay->CharacterInventory->UpdateAllSlots();
					MyPlayerController->GetPlayerHUD()->PlayerOverlay->InventoryWidget->UpdateAllSlots();
				}
			}
			return;
		}
	}

	FailedPotionCreation();
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
