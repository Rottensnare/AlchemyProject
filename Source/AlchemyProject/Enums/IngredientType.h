#pragma once

UENUM(BlueprintType)
enum class EIngredientType : uint8
{
	EIT_Substance UMETA(DisplayName = "Substance"),
	EIT_Special UMETA(DisplayName = "Special"),
	EIT_Catalyst UMETA(DisplayName = "Catalyst"),
	EIT_Base UMETA(DisplayName = "Base"),
	EIT_Container UMETA(DisplayName = "Container"),
	EIT_All UMETA(DisplayName = "All"), //All is used with inventory slots to indicate that the slot can take any type of ingredient

	EIT_MAX UMETA(DisplayName = "DefaultMax")
};