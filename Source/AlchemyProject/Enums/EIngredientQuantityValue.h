#pragma once

UENUM(BlueprintType)
enum class EIngredientQuantityValue : uint8
{
	EIQV_Poor UMETA(DisplayName = "Poor Quantity"),
	EIQV_Medium UMETA(DisplayName = "Medium Quantity"),
	EIQV_Rich UMETA(DisplayName = "Rich Quantity"),
	EIQV_None UMETA(DisplayName = "No Substance"),

	EIQV_MAX UMETA(DisplayName = "DefaultMax"),
};

namespace QuantityValue
{
	static int32 GetQuantityValueInt(const EIngredientQuantityValue InEIQV)
	{
		switch(InEIQV)
		{
		case EIngredientQuantityValue::EIQV_Poor:
			return 4;
		case EIngredientQuantityValue::EIQV_Medium:
			return 2;
		case EIngredientQuantityValue::EIQV_Rich:
			return 1;
		default:
			return 999;
		}
	}
}
