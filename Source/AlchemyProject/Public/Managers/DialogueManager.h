// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/DialogueOverlay.h"
#include "UObject/NoExportTypes.h"
#include "DialogueManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnd);

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

	void EndDialogue();

private:

	FDialogueState CurrentDialogueState;
	int32 CurrentDialogueStateID{0};
	
	FDialogueState PreviousDialogueState;
	int32 PreviousDialogueStateID;

	int32 NextDialogueStateID{0};
	
	FString NPCName;

	FString DialogueFilePath_Options;
	FString DialogueFilePath_States;

	FString NPCDialogue{""};
	
	TMap<int32, FDialogueOption> CurrentDialogueOptions;
	
	TArray<FString> OptionStrings;

public:

	FORCEINLINE FString GetNPCName() const {return NPCName;}
	FORCEINLINE TArray<FString>& GetOptionStrings() {return OptionStrings;}
	FORCEINLINE FString GetNPCDialogue() const {return NPCDialogue;}
	FORCEINLINE int32 GetNextDialogueStateID() const {return NextDialogueStateID;}
	FORCEINLINE int32 GetPreviousDialogueStateID() const {return PreviousDialogueStateID;}
	FORCEINLINE TMap<int32, FDialogueOption>&GetCurrentDialogueOptions() {return CurrentDialogueOptions;}

	void EmptyDialogueOptions();

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetDialogueOptions(TArray<FDialogueOption>& OutOptions);

	FOnDialogueEnd OnDialogueEnd;
};
