// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Queryable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQueryable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALCHEMYPROJECT_API IQueryable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	FGameplayTagContainer InterfaceGameplayTagContainer;

	virtual void InitializeGameplayTagContainer(FGameplayTagContainer InGameplayTagContainer);
};
