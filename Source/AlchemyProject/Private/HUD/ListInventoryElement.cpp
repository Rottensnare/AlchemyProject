// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ListInventoryElement.h"

void UListInventoryElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnListItemObjectSet"))
	
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}
