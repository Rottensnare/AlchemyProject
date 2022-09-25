// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProduct.h"
#include "Potion.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API APotion : public AAlchemyProduct
{
	GENERATED_BODY()

public:

	APotion();

protected:

	UPROPERTY(EditAnywhere)
	FProductInfo PotionInfo;

private:


public:

	FORCEINLINE FProductInfo GetProductInfo() const {return PotionInfo;}
	
};
