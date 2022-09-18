// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CreateInventoryGrid(const int32 NumberOfElements);

	UFUNCTION(BlueprintCallable)
	void CreateContainerGrid(const int32 NumberOfElements);

	UFUNCTION(BlueprintCallable)
	void UpdateInventorySlot(const int32 Index);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UInventorySlot* ActiveSlot;

	UFUNCTION()
	void HandleComboSelection(FString SelectedItem, ESelectInfo::Type SelectionType);

	void UpdateSlotFromInventory(const int32 Index);

	void UpdateAllSlots();
	
protected:

	


private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlot> InventorySlotClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Slot", meta = (AllowPrivateAccess = "true"))
	TArray<UInventorySlot*> InventorySlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UUniformGridPanel* InventoryGrid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory Slot", meta = (AllowPrivateAccess = "true"))
	int32 SelectedSlotIndex;

	UPROPERTY(EditAnywhere)
	TArray<FString> ComboSelections;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInfoBox> InfoBoxClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot", meta = (AllowPrivateAccess = "true"))
	UInfoBox* InfoBox;

	FVector2D ScreenMiddle;

	UPROPERTY(EditAnywhere)
	int32 NumberOfColumns;
	UPROPERTY(EditAnywhere)
	int32 NumberOfRows;

public:

	FORCEINLINE void SetSelectedSlotIndex(const int32 Index) {SelectedSlotIndex = Index;}
	FORCEINLINE UInfoBox* GetInfoBox() const {return InfoBox;}
	FORCEINLINE UUniformGridPanel* GetInventoryGrid() const {return InventoryGrid;}


	
};
