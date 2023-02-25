// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Alchemy/FAlchemyPackage.h"
#include "Alchemy/FIngredientInfo.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

//BUG: Not really a bug, but this component probably needs to be attached to the player controller because if the character dies, it loses the inventory, probably...
//TODO: Make the inventory more like Skyrim inventory, otherwise this alchemy system isn't going to work that well. 

USTRUCT(BlueprintType)
struct FInventoriSlot
{
	GENERATED_BODY()

	//TODO: Make this const so that someone (That would be Me) couldn't edit this
	UPROPERTY(BlueprintReadOnly)
	int32 SlotId;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemAmount;
	
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<AItem> ItemClass;

	UPROPERTY(BlueprintReadOnly)
	class UTexture2D* ItemIcon;

	UPROPERTY(BlueprintReadOnly)
	EItemType ItemType;

	UPROPERTY(BlueprintReadOnly)
	FIngredientInfo IngredientInfo;

	UPROPERTY(BlueprintReadOnly)
	FProductInfo ProductInfo;

	UPROPERTY(VisibleAnywhere)
	uint32 HashCode;
	
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UInventoryComponent();
	friend class APlayerCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ShowInventory(bool bVisible);
	
	void AddToInventory(AItem* InItem, int32 InAmount);
	void UpdateInventorySlot(const int32 Index);
	void AddPotionToInventory(class APotion* const InPotion, const int32 InAmount, const uint32 InHashCode);
	void DropItem(const int32 Index);
	void SpawnItemFromInventory(const int32 InIndex, const int32 InAmount);

	//Will be used to easily check if character has the correct amount of required item
	UPROPERTY()
	TMap<TSubclassOf<AItem>, int32> ItemTotalAmountMap;

	UPROPERTY()
	TMap<uint32, int32> HashSlotIDMap;

protected:

	virtual void BeginPlay() override;

	void InitGridCreation();

	//Index of the item in inventory array, Change = number to increase or decrease by
	void UpdateItemAmount(const int32 Index, const int32 Change);

	
	UPROPERTY(BlueprintReadWrite)
	TArray<class UListInventoryElement*> InventoryElements;

	
private:

	UPROPERTY()
	class APlayerCharacter* Character;
	
	UPROPERTY(EditAnywhere)
	TArray<FInventoriSlot> InventorySlots;

	UPROPERTY(EditAnywhere)
	int32 NumberOfInventorySlots{12};

	FTimerHandle GridCreationTimer;

	

public:
	
	TArray<FInventoriSlot>& GetInventory() {return InventorySlots;}
	
	
};
