// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/DialogueOverlay.h"
#include "UObject/NoExportTypes.h"
#include "DialogueManager.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UDialogueManager : public UObject
{
	GENERATED_BODY()

public:

	UDialogueManager();

	void StartDialogue(const int32 NPCId);

private:

	FDialogueState CurrentDialogueState;

	FString NPCName;

public:

	FORCEINLINE FString GetNPCName() const {return NPCName;}
	
};
