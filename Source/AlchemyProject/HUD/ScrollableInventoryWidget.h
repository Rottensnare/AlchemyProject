// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollableInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UScrollableInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void UpdateInventorySlot(const int32 Index);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UInventorySlot* ActiveSlot;

	UFUNCTION()
	void HandleComboSelection(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable)
	void CreateInventory(const int32 NumberOfElements);

protected:



private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlot> InventorySlotClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Slot", meta = (AllowPrivateAccess = "true"))
	TArray<UInventorySlot*> InventorySlots;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* InventoryScrollBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Slot", meta = (AllowPrivateAccess = "true"))
	int32 SelectedSlotIndex;

	UPROPERTY(EditAnywhere)
	TArray<FString> ComboSelections;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInfoBox> InfoBoxClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot", meta = (AllowPrivateAccess = "true"))
	UInfoBox* InfoBox;

	FVector2D ScreenMiddle;


public:

	FORCEINLINE void SetSelectedSlotIndex(const int32 Index) {SelectedSlotIndex = Index;}
	FORCEINLINE UInfoBox* GetInfoBox() const {return InfoBox;}
};
