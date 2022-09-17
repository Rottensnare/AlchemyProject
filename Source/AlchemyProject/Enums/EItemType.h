#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Consumable UMETA(DisplayName = "Consumable"),
	EIT_Ingredient UMETA(DisplayName = "Ingredient"),
	EIT_Equippable UMETA(DisplayName = "Equippable"),
	EIT_Misc UMETA(DisplayName = "Misc"),

	EIT_MAX UMETA(DisplayName = "DefaultMax"),

};