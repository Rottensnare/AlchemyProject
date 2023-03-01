// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ListInventoryBox.generated.h"

UENUM(BlueprintType)
enum class EInventoryCategory : uint8
{
	EIC_All UMETA(DisplayName = "All"),
	EIC_Misc UMETA(DisplayName = "Misc"),
	EIC_Alchemy UMETA(DisplayName = "Alchemy"),
	EIC_Consumable UMETA(DisplayName = "Consumable"),
	EIC_Equipment UMETA(DisplayName = "Equipment"),
	EIC_Quest UMETA(DisplayName = "Quest"),
	EIC_Key UMETA(DisplayName = "Key"),
	EIC_Magic UMETA(DisplayName = "Magic"),
	EIC_Crafting UMETA(DisplayName = "Crafting"),

	EIC_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class ESortingType : uint8
{
	EST_Name UMETA(DisplayName = "Name"),
	EST_Type UMETA(DisplayName = "Type"),
	EST_Amount UMETA(DisplayName = "Amount"),
	EST_Value UMETA(DisplayName = "Value"),
	EST_Weight UMETA(DisplayName = "Weight"),
	EST_NoSort UMETA(DisplayName = "NoSort"),
	
	EST_MAX UMETA(DisplayName = "DefaultMax")
};

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UListInventoryBox : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UListView* InventoryList;

	UPROPERTY(BlueprintReadWrite)
	EInventoryCategory CurrentInventoryCategory{EInventoryCategory::EIC_All};

	void SortList(const ESortingType SortingType, const bool bInverse);

	UPROPERTY(BlueprintReadWrite)
	TArray<class UListInventoryElement*> InventoryElements;

private:

	

public:

	
};
