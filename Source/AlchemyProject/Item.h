// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enums/EItemType.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Pickable.h"
#include "Interfaces/Queryable.h"
#include "Item.generated.h"



UENUM()
enum class EItemState : uint8
{
	EIS_Dropped UMETA(DisplayName = "Dropped"),
	EIS_Initial UMETA(DisplayName = "Initial"),

	EIS_MAX UMETA(DisplayName = "DefaultMax")
};



UCLASS()
class ALCHEMYPROJECT_API AItem : public AActor, public IPickable, public IQueryable
{
	GENERATED_BODY()
	
public:	

	AItem();

	bool bInventoryPlaceable{true};

	//TODO: Add a short timer to the character's trace function so that the pop up won't become visible immediately
	void ShowItemPopupWidget() const;
	
	UFUNCTION()
	virtual void UpdatePopUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info|GameplayTags")
	FGameplayTagContainer GameplayTagContainer;
	
protected:

	virtual void BeginPlay() override;

	virtual void InitGameplayTagContainer();

	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	UPROPERTY(EditAnywhere)
	class UTexture2D* ImageInSlot;

	//If the item can be stacked to an inventory slot
	UPROPERTY(VisibleAnywhere)
	bool bStackable{true};

	//DEPRECATED: There will be no Max Stack amount.
	//How many items of this type can be stacked per slot.
	UPROPERTY(EditAnywhere)
	int32 MaxStack{16};

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EItemState ItemState{EItemState::EIS_Initial};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EItemType ItemType{EItemType::EIT_Misc};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Popup, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* ItemPopUp;

	UPROPERTY(EditAnywhere)
	FString PopUpText;

	UPROPERTY(EditAnywhere)
	FString ItemName;

	bool bShouldInitGameplayTags{true};

	UFUNCTION()
	void OnComponentWake_Delegate(UPrimitiveComponent* WakingComponent, FName BoneName);
	UFUNCTION()
	void OnComponentSleep_Delegate(UPrimitiveComponent* SleepingComponent, FName BoneName);

private:


public:


	FORCEINLINE UTexture2D* GetSlotImage() const {return ImageInSlot;}
	FORCEINLINE int32 GetMaxStackSize() const {return MaxStack;}
	FORCEINLINE void SetItemState(EItemState InState) {ItemState = InState;}
	FORCEINLINE EItemState GetItemState() const {return ItemState;}
	FORCEINLINE UStaticMeshComponent* GetItemMesh() const {return ItemMesh;}
	FORCEINLINE EItemType GetItemType() const {return ItemType;}


	/********************
	 * Interface overrides
	 *******************/

	//virtual void InitializeGameplayTagContainer(FGameplayTagContainer InGameplayTagContainer) override;
	virtual FGameplayTagContainer& GetGameplayTagContainer() override;
};
