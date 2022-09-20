// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FAlchemyPackage.h"
#include "AlchemyProject/Item.h"
#include "AlchemyProduct.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ALCHEMYPROJECT_API AAlchemyProduct : public AItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FRecipe Recipe;

	UPROPERTY(EditAnywhere)
	FString ProductDescription;
	
	void Use();
	
};
