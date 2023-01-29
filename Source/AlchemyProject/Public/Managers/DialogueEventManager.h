// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogueEventManager.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UDialogueEventManager : public UObject
{
	GENERATED_BODY()

public:

	bool HandleDialogueEvent(const FString& EventName);

	TArray<UObject*> GetDialogueObjects(const FName& RowName, bool& bSuccess);
};
