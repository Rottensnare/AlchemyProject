// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/DialogueEventManager.h"

#include "AI/AIBase.h"
#include "AlchemyProject/HealthComponent.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "AlchemyProject/HUD/PlayerHUD.h"
#include "AlchemyProject/PlayerController/MyPlayerController.h"
#include "Engine/DataTable.h"
#include "HUD/DialogueOverlay.h"

//TODO: This is a temporary solution
bool UDialogueEventManager::HandleDialogueEvent(const FString& EventName, const TArray<TSoftObjectPtr<>>& EventObjects)
{
	TArray<FString> Arguments;
	EventName.ParseIntoArray(Arguments, TEXT(","));
	
	for(const FString& Argument : Arguments)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Argument: %s"), *Argument)
		if(Argument == FString("Heal"))
		{
			for(const auto& EventObject : EventObjects)
			{
				APlayerCharacter* PlayerChara = Cast<APlayerCharacter>(EventObject.Get());
				if(PlayerChara)
				{
					//UE_LOG(LogTemp, Warning, TEXT("PlayerChara"))
					PlayerChara->GetHealthComponent()->SetHealth(
					FMath::Clamp(
					PlayerChara->GetHealthComponent()->GetHealth() + FMath::Clamp(200.f , 0, 10000)
					, 0.f
					, PlayerChara->GetHealthComponent()->GetMaxHealth()));

					//Update HUD
					if(AMyPlayerController* const TempController = Cast<AMyPlayerController>(PlayerChara->Controller))
					{
						if(APlayerHUD* const TempHUD = Cast<APlayerHUD>(TempController->GetHUD()))
						{
							TempHUD->UpdateHealthBar(PlayerChara->GetHealthComponent()->GetHealth(), PlayerChara->GetHealthComponent()->GetMaxHealth());
							TempController->PlaySound(FName("Healing"));
						}
					}
				}
			}
		}
	}
	
	return true;
}

TArray<UObject*> UDialogueEventManager::GetDialogueObjects(const FName& RowName, bool& bSuccess)
{
	TArray<UObject*> OutObjectArray;
	const FString OptionsDataTablePath(TEXT("DataTable'/Game/Assets/Datatables/DialogueOptionTable.DialogueOptionTable'"));
	const UDataTable* OptionTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *OptionsDataTablePath)); 
	if(!OptionTableObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("OptionTableObject was not valid"))
		bSuccess = false;
		return TArray<UObject*>();
	}
	
	const FDialogueOption* TestDataRow = OptionTableObject->FindRow<FDialogueOption>(RowName, TEXT(""));
	
	if(TestDataRow && TestDataRow->FunctionObjects.IsValidIndex(0))
	{
		for(const auto& Object : TestDataRow->FunctionObjects)
		{
			OutObjectArray.Add(Object.Get());
			//UE_LOG(LogTemp, Display, TEXT("Object Name: %s"), *Object.Get()->GetName())
			if(AAIBase* TempAI = Cast<AAIBase>(Object.Get()))
			{
				
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Objects Not Valid"))
	}
	
	bSuccess = !OutObjectArray.IsEmpty();
	return OutObjectArray;
}
