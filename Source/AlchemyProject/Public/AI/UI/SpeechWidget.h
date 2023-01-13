// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SpeechWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API USpeechWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class URichTextBlock* RichTextBlock;

	UFUNCTION(BlueprintNativeEvent)
	void SetBlockTextEvent(const FString& InText);
	
};
