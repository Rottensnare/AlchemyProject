// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> OverlayClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InventoryClass;

	UPROPERTY()
	class UPlayerOverlay* PlayerOverlay;

	UPROPERTY()
	class UInventoryWidget* InventoryWidget;

	UPROPERTY()
	class UScrollableInventoryWidget* ScrollableInventoryWidget;

	

	void AddCharacterOverlay();

	
};
