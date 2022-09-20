// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyItem.h"
#include "AlchemyBase.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API AAlchemyBase : public AAlchemyItem
{
	GENERATED_BODY()

public:

	const EIngredientType IngredientType{EIngredientType::EIT_Base};
	
	UPROPERTY(EditDefaultsOnly)
	EBaseType BaseType{EBaseType::EBT_MAX};
	
};
