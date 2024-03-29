// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "DialogueOverlay.generated.h"

DECLARE_DYNAMIC_DELEGATE(FExecuteDialogueEvent);

USTRUCT(BlueprintType)
struct FDialogueOptionRequirements
{
	GENERATED_BODY()

	//Tags that will be checked that the player has.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer RequiredTags;

	//Tags that the player can't have
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer ForbiddenTags;
	
	bool HasRequirements() const;
	bool CheckHasAllTags(const FGameplayTagContainer InContainer) const;
	bool CheckForbiddenTags(const FGameplayTagContainer InContainer) const;
	
};

USTRUCT(BlueprintType)
struct FDialogueOption : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //TODO: Make auto increment
	int32 DialogueID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText TextToDisplay;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NextDialogueStateID{0};

	bool LeadsToOtherDialogue() const {return NextDialogueStateID == 0 ? true : false;}

	void AssignID();

	//Used to store the name of the function to be called.
	//Functions in this map will be executed if the option is selected.
	//TODO: Make this Blueprint usable. Not really intuitive to write function names like &APlayerCharacter::DoSomething inside the editor.
	UPROPERTY(EditAnywhere)
	TArray<FName> FunctionNames;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UObject>>FunctionObjects;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> EventArguments;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDialogueOptionRequirements Requirements;

	//@Deprecated
	void HandlePlayerChoice();
	
};


USTRUCT(BlueprintType)
struct FDialogueState : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 DialogueStateID;
	
	UPROPERTY(EditAnywhere)
	TArray<FText> NPCDialogueTexts;

	UPROPERTY(EditAnywhere)
	TArray<int32> DialogueOptions;
	
};



/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UDialogueOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UDialogueBox* DialogueBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* NameTextBlock;

	auto ExitButtonPressed() -> void;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UDialogueManager> DialogueManager;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UDialogueEventManager> DialogueEventManager;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UMultiLineEditableTextBox* MultiLineTextBox;

	UFUNCTION()
	void OptionSelected(FDialogueOption SelectedOption);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString RegexPatternString{"\"([^\"]*)\""};

	UPROPERTY(BlueprintReadOnly)
	class APlayerCharacter* Player;

private:

	void RemoveItems();
};