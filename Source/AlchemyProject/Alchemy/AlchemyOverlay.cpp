// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyOverlay.h"

#include "AlchemyProject/HUD/InventoryWidget.h"

void UAlchemyOverlay::NativeOnInitialized()
{
	TableInventory->CreateContainerGrid(9);
	TableInventory->SetVisibility(ESlateVisibility::Visible);
	
	Super::NativeOnInitialized();
}
