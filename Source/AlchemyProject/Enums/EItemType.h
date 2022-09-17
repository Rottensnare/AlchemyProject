#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Consumable UMETA(DisplayName = "Consumable"),
	EIT_Ingredient UMETA(DisplayName = "Ingredient"),
	EIT_Equippable UMETA(DisplayName = "Equippable"),
	EIT_Misc UMETA(DisplayName = "Misc"),
	EIT_Quest UMETA(DisplayName = "Quest"),
	EIT_All UMETA(DisplayName = "All"), //All is used with inventory slots to tell the slot to take all types of items

	EIT_MAX UMETA(DisplayName = "DefaultMax")

};