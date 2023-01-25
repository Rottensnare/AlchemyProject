// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueBox.generated.h"

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
	void EmptyListView();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> OptionStrings;
};
