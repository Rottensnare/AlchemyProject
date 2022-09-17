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

	//Overlay for the alchemy table user interface
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAlchemyOverlay> AlchemyOverlayClass;

	UPROPERTY()
	UAlchemyOverlay* AlchemyOverlay;

	UPROPERTY()
	class UInventoryWidget* InventoryWidget;

	UPROPERTY()
	class UScrollableInventoryWidget* ScrollableInventoryWidget;

	

	void AddCharacterOverlay();

	
};
