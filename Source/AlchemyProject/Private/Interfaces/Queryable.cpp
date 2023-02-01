// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/Queryable.h"

// Add default functionality here for any IQueryable functions that are not pure virtual.
void IQueryable::InitializeGameplayTagContainer(FGameplayTagContainer InGameplayTagContainer)
{
	//UE_LOG(LogTemp, Warning, TEXT("IQueryable First Tag: %s"), *InGameplayTagContainer.First().GetTagName().ToString())
	InterfaceGameplayTagContainer = InGameplayTagContainer;
}
