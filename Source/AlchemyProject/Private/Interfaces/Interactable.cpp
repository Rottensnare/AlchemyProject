// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/Interactable.h"

// Add default functionality here for any IInteractable functions that are not pure virtual.
bool IInteractable::Interact(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Other Actor: %s"), *OtherActor->GetName())
	return true;
}
