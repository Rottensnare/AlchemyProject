// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemPopUp.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UItemPopUp : public UUserWidget
{
	GENERATED_BODY()

	
public:
	
	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* RichTextBlock;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* RichTextDataTable;

	void SetBlockText(const FString& ItemDescription) const;

};
