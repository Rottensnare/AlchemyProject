﻿#pragma once

UENUM(BlueprintType)
enum class EIngredientQuantityValue : uint8
{
	EIQV_Poor UMETA(DisplayName = "Poor Quantity"),
	EIQV_Medium UMETA(DisplayName = "Medium Quantity"),
	EIQV_Rich UMETA(DisplayName = "Rich Quantity"),

	EIQV_MAX UMETA(DisplayName = "DefaultMAX"),
};