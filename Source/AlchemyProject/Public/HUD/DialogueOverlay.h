// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueOverlay.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UDialogueOverlay : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UDialogueBox* DialogueBox;
};
