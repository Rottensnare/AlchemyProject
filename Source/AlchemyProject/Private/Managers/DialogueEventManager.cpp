// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/DialogueEventManager.h"

#include "AI/AIBase.h"
#include "Engine/DataTable.h"
#include "HUD/DialogueOverlay.h"

bool UDialogueEventManager::HandleDialogueEvent(const FString& EventName)
{
	

	
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
			UE_LOG(LogTemp, Display, TEXT("Object Name: %s"), *Object.Get()->GetName())
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
