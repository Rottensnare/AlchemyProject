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

	void StartDialogue(const int32 DialogueStateID);

	bool GetJSON(const FString& FilePath, int32 ID);

private:

	FDialogueState CurrentDialogueState;
	int32 CurrentDialogueStateID;
	
	FDialogueState PreviousDialogueState;
	int32 PreviousDialogueStateID;
	
	FString NPCName;

	FString DialogueFilePath_Options;
	FString DialogueFilePath_States;

	FString NPCDialogue{""};
	TArray<FString> OptionStrings;

public:

	FORCEINLINE FString GetNPCName() const {return NPCName;}
	FORCEINLINE TArray<FString>& GetOptionStrings() {return OptionStrings;}
	FORCEINLINE FString GetNPCDialogue() const {return NPCDialogue;}
};
