// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfoBox.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UInfoBox : public UUserWidget
{
	GENERATED_BODY()

public:


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UMultiLineEditableText* DescriptionTextBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMultiLineEditableText* ItemInfoTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCollapsed{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex;

	
};
