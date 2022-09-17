#pragma once

UENUM(BlueprintType)
enum class EIngredientQuality : uint8
{
	EIQ_Poor UMETA(DisplayName = "Poor Quality"),
	EIQ_Medium UMETA(DisplayName = "Medium Quality"),
	EIQ_High UMETA(DisplayName = "High Quality"),
	EIQ_Pure UMETA(DisplayName = "Pure Quality"),

	EIQ_MAX UMETA(DisplayName = "DefaultMAX")
};