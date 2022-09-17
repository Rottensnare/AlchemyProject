// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "AlchemyOverlay.generated.h"

/**
 * Alchemy overlay needs info widget to show alchemy product information, as well as ingredient information.
 * Needs 9 total slots for different things.
 * Needs upgrade window (Advanced stuff).
 * Needs the characters inventory so that ingredients can be dragged into the table inventory.
 * Needs a list of known potions.
 * Needs a way to cycle/sort through ingredients
 * Quantity and quality selector
 */
UCLASS()
class ALCHEMYPROJECT_API UAlchemyOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UInventoryWidget* CharacterInventory;

	//TODO: Table needs its own custom inventory widget.
	//4 slots are needed for ingredients, 1 for special ingredient, 1 for catalyst, 1 for base, 1 for container, 1 for end product
	//Total of 9 slots
	UPROPERTY(meta = (BindWidget)) 
	class UInventoryWidget* TableInventory;

	UPROPERTY(meta = (BindWidget))
	class UInfoBox* ProductInfoBox;

	//Creates the alchemy product if possible
	UPROPERTY(meta = (BindWidget))
	class UButton* CreateButton;

	//Clears ingredients from the alchemy inventory
	UPROPERTY(meta = (BindWidget))
	UButton* ClearButton;
	
};
