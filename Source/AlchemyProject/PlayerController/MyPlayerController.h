// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:


	void SetHUDHealth(float Health, float MaxHealth);
	void SetInventoryVisibility(bool bVisible);
	void SetInventoryGrid(int32 NumberOfSlots);
	void UpdateInventory(const int32 Index);
	void ToggleAlchemyOverlay();
	void SelectAlchemyIngredient(const int32 SelectedSlot);
	UFUNCTION(BlueprintCallable)
	void ClearAlchemySelection(const int32 Index = -1);

protected:

	virtual void BeginPlay() override;


private:


	UPROPERTY()
	class APlayerHUD* PlayerHUD;

	UPROPERTY(EditAnywhere)
	bool bUseScrollableInventory{true};

	UPROPERTY()
	class APlayerCharacter* CurrentCharacter;

	//UI PARAMS

	UPROPERTY(EditAnywhere)
	float AlchemyItemBGOpacity{0.1f};
	UPROPERTY(EditAnywhere)
	float AlchemyItemIconOpacity{0.9f};
	
};
