// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "AlchemyProject/Alchemy/FIngredientInfo.h"
#include "AlchemyProject/Enums/EPrimarySubstance.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API AMyPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	AMyPlayerController();

	void SetHUDHealth(float Health, float MaxHealth);
	void SetInventoryVisibility(bool bVisible);
	void SetInventoryGrid(int32 NumberOfSlots);
	void UpdateInventory(const int32 Index);
	void ToggleAlchemyOverlay();
	
	UFUNCTION(BlueprintCallable)
	void SelectAlchemyIngredient(const int32 SelectedSlot);
	UFUNCTION(BlueprintCallable)
	void ClearAlchemySelection(const int32 Index = -1);

	void FindIngredients(const FName& RecipeName);

	void PlaySound(const FName& SFXName);
	//Native version
	void CreateNoiseNative(const float InVolume, const float InMaxRange, TOptional<FName> InTag);
	//Blueprint version
	UFUNCTION(BlueprintCallable)
	void CreateNoise(const float InVolume, const float InMaxRange, FName InTag = NAME_None);

	FGenericTeamId TeamId{FGenericTeamId(4)};
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:

	virtual void BeginPlay() override;


private:


	UPROPERTY()
	class APlayerHUD* PlayerHUD;

	UPROPERTY(EditAnywhere)
	bool bUseScrollableInventory{true};

	UPROPERTY()
	class APlayerCharacter* CurrentCharacter;

	UPROPERTY()
	class UPawnNoiseEmitterComponent* PawnNoiseEmitterComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<EPrimarySubstance> SelectedSubstances;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FIngredientInfo> IngredientInfos;

	TMap<EPrimarySubstance, int32> CurrentRecipeMap;

	//UI PARAMS

	UPROPERTY(EditAnywhere)
	float AlchemyItemBGOpacity{0.1f};
	UPROPERTY(EditAnywhere)
	float AlchemyItemIconOpacity{0.9f};

	
public:

	FORCEINLINE APlayerHUD* GetPlayerHUD() const {return PlayerHUD;}
};
