// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/EItemType.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Pickable.h"
#include "Item.generated.h"



UENUM()
enum class EItemState : uint8
{
	EIS_Dropped UMETA(DisplayName = "Dropped"),
	EIS_Initial UMETA(DisplayName = "Initial"),

	EIS_MAX UMETA(DisplayName = "DefaultMax")
};



UCLASS()
class ALCHEMYPROJECT_API AItem : public AActor, public IPickable
{
	GENERATED_BODY()
	
public:	

	AItem();
	virtual void Tick(float DeltaTime) override;

	bool bInventoryPlaceable{true};

	
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UTexture2D* ImageInSlot;

	//If the item can be stacked to an inventory slot
	UPROPERTY(VisibleAnywhere)
	bool bStackable{true};

	//How many items of this type can be stacked per slot
	UPROPERTY(EditAnywhere)
	int32 MaxStack{16};

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EItemState ItemState{EItemState::EIS_Initial};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EItemType ItemType{EItemType::EIT_Misc};

private:



public:


	FORCEINLINE UTexture2D* GetSlotImage() const {return ImageInSlot;}
	FORCEINLINE int32 GetMaxStackSize() const {return MaxStack;}
	FORCEINLINE void SetItemState(EItemState InState) {ItemState = InState;}
	FORCEINLINE UStaticMeshComponent* GetItemMesh() const {return ItemMesh;}
	FORCEINLINE EItemType GetItemType() const {return ItemType;}
	
};
