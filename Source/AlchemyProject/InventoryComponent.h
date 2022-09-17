// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

//BUG: Not really a bug, but this component probably needs to be attached to the player controller because if the character dies, it loses the inventory, probably...

USTRUCT(BlueprintType)
struct FInventoriSlot
{
	GENERATED_BODY()

	//TODO: Make this const so that someone (That would be Me) couldn't edit this
	UPROPERTY(BlueprintReadOnly)
	int32 SlotId;

	UPROPERTY(BlueprintReadWrite)
	int32 ItemAmount;

	//UPROPERTY(BlueprintReadWrite)
	//class AItem* ItemInSlot;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<AItem> ItemClass;

	UPROPERTY(BlueprintReadOnly)
	class UTexture2D* ItemIcon;

	UPROPERTY(BlueprintReadOnly)
	EItemType ItemType;
	
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
	void DropItem(const int32 Index);
	void SpawnItemFromInventory(const int32 InIndex, const int32 InAmount);


protected:

	virtual void BeginPlay() override;

	void InitGridCreation();

	//Index of the item in inventory array, Change = number to increase or decrease by
	void UpdateItemAmount(const int32 Index, const int32 Change);

	
	

	
private:

	UPROPERTY()
	class APlayerCharacter* Character;
	
	UPROPERTY(EditAnywhere)
	TArray<FInventoriSlot> InventorySlots;

	UPROPERTY(EditAnywhere)
	int32 NumberOfInventorySlots{12};

	FTimerHandle GridCreationTimer;

	//Will be used to easily check if character has the correct amount of required item
	UPROPERTY()
	TMap<TSubclassOf<AItem>, int32> ItemTotalAmountMap;

public:
	
	TArray<FInventoriSlot>& GetInventory() {return InventorySlots;}
	
};
