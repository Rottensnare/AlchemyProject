#pragma once

#include "CoreMinimal.h"

#include "AlchemyProject/Enums/EIngredientQuality.h"
#include "AlchemyProject/Enums/EIngredientQuantityValue.h"
#include "AlchemyProject/Enums/EPrimarySubstance.h"
#include "AlchemyProject/Enums/ESecondarySubstance.h"
#include "AlchemyProject/Enums/ETertiarySubstance.h"

#include "FIngredientInfo.generated.h"

USTRUCT(BlueprintType)
struct FIngredientInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIngredientQuality IngredientQuality{EIngredientQuality::EIQ_MAX};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIngredientQuantityValue IngredientQuantityValue{EIngredientQuantityValue::EIQV_MAX};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IngredientAmountPerHarvest{1};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPrimarySubstance PrimarySubstance{EPrimarySubstance::EPS_MAX};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESecondarySubstance SecondarySubstance{ESecondarySubstance::EPS_MAX};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETertiarySubstance TertiarySubstance{ETertiarySubstance::ETS_MAX};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* IngredientIcon;
};
