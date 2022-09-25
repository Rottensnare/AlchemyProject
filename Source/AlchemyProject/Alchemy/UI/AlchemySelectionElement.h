// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlchemySelectionElement.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UAlchemySelectionElement : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonElement;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ButtonText;

private:

	UFUNCTION(BlueprintCallable)
	void OnButtonPressed(const FString& RecipeName);
	
};
