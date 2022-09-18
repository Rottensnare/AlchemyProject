// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProject/InventoryComponent.h"
#include "AlchemyProject/Enums/IngredientType.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	//This is a big no no, but whatever, just testing. In other words, This thing basically duplicates the inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Items)
	FInventoriSlot HUDInventorySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (BindWidget))
	class UImage* SlotIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items , meta = (BindWidget))
	class UTextBlock* AmountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items , meta = (BindWidget))
	class UButton* SlotButton;

	UPROPERTY(BlueprintReadOnly)
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(BlueprintReadOnly)
	class UScrollableInventoryWidget* ScrollableInventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items , meta = (BindWidget))
	class UComboBoxString* ComboBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType{EItemType::EIT_All};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EIngredientType IngredientType{EIngredientType::EIT_All};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EIngredientType, FLinearColor> IngredientTypeColors;

protected:
	virtual void NativeOnInitialized() override;
	void InitIngredientTypeColors();
	
};
