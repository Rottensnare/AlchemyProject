// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "DialogueBoxElement.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UDialogueBoxElement : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* RichTextBlock;
	
};
