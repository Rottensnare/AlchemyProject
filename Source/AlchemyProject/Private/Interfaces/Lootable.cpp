// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/Lootable.h"

// Add default functionality here for any ILootable functions that are not pure virtual.
TObjectPtr<UInventoryComponent> ILootable::GetInventoryComp()
{
	return nullptr;
}
