// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD/DialogueOverlay.h"
#include "DialogueBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionSelected, FDialogueOption, SelectedOption);

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UDialogueBox : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<class UDialogueBoxElement*> ElementArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDialogueBoxElement* SelectedElement;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueBoxElement> DialogueBoxElementClass;
	
public:
	
	void SetSelectedElement(const int32 ElementIndex);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UListView* DialogueListView;

	void AddToListView(TArray<class UDialogueBoxElement>& Options);
	void AddToListView(TArray<FString>& Options);
	void AddToListView(TMap<int32, struct FDialogueOption>& InDialogueOptions);
	void EmptyListView();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> OptionStrings;

	UPROPERTY(BlueprintReadOnly)
	class UDialogueOverlay* DialogueOverlay;

	UFUNCTION(BlueprintCallable)
	void OptionSelected(const FDialogueOption SelectedOption) const;

	FOnOptionSelected OnOptionSelected;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FDialogueOption> DialogueOptions;
	
	void OnOptionsUpdated();
};
