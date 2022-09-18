// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProject/Enums/EIngredientQuality.h"
#include "AlchemyProject/Enums/EIngredientQuantityValue.h"
#include "AlchemyProject/Enums/EPrimarySubstance.h"
#include "AlchemyProject/Enums/ESecondarySubstance.h"
#include "AlchemyProject/Enums/ETertiarySubstance.h"
#include "UObject/NoExportTypes.h"
#include "IngredientData.generated.h"




/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UIngredientData : public UObject
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere)
	EIngredientQuality IngredientQuality{EIngredientQuality::EIQ_MAX};
	UPROPERTY(EditAnywhere)
	EIngredientQuantityValue IngredientQuantityValue{EIngredientQuantityValue::EIQV_MAX};
	UPROPERTY(EditAnywhere)
	int32 IngredientAmountPerHarvest{1};
	UPROPERTY(EditAnywhere)
	EPrimarySubstance PrimarySubstance{EPrimarySubstance::EPS_MAX};
	UPROPERTY(EditAnywhere)
	ESecondarySubstance SecondarySubstance{ESecondarySubstance::EPS_MAX};
	UPROPERTY(EditAnywhere)
	ETertiarySubstance TertiarySubstance{ETertiarySubstance::ETS_MAX};
	UPROPERTY(EditAnywhere)
	class UTexture2D* IngredientIcon;
};
