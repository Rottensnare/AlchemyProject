// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UScrollableInventoryWidget* ScrollableInventoryWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UClockWidget* ClockWidget;

	FORCEINLINE void SetInventoryWidget(UInventoryWidget* InInventory) {InventoryWidget = InInventory;}
};
