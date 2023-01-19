#pragma once

UENUM(BlueprintType)
enum class EIngredientQuality : uint8
{
	EIQ_None UMETA(DisplayName = "No Substance"),
	EIQ_Poor UMETA(DisplayName = "Poor Quality"),
	EIQ_Medium UMETA(DisplayName = "Medium Quality"),
	EIQ_High UMETA(DisplayName = "High Quality"),
	EIQ_Pure UMETA(DisplayName = "Pure Quality"),

	EIQ_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EProductQuality : uint8
{
	EPQ_None UMETA(DisplayName = "No Quality"),
	EPQ_Poor UMETA(DisplayName = "Poor Quality"),
	EPQ_Medium UMETA(DisplayName = "Medium Quality"),
	EPQ_High UMETA(DisplayName = "High Quality"),
	EPQ_Pure UMETA(DisplayName = "Pure Quality"),
	
	EIQ_MAX UMETA(DisplayName = "DefaultMax")
};

namespace ProductQuality
{
	static EProductQuality CalculateProductQuality(const TArray<EIngredientQuality>& IngredientQualities)
	{
		uint8 TempValue{0};
		for(const auto& Quality : IngredientQualities)
		{
			TempValue += (uint8)Quality;
		}
		if(IngredientQualities.Num() > 0) TempValue /= IngredientQualities.Num();
		switch (TempValue)
		{
		case 1:
			return EProductQuality::EPQ_Poor;
		case 2:
			return EProductQuality::EPQ_Medium;
		case 3:
			return EProductQuality::EPQ_High;
		case 4:
			return EProductQuality::EPQ_Pure;
		default:
			return EProductQuality::EPQ_None;
		}
	}
}
