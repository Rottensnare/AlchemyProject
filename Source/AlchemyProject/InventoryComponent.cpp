// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Ingredient.h"
#include "Item.h"
#include "PlayerCharacter.h"
#include "Alchemy/AlchemyItem.h"
#include "HUD/InventoryWidget.h"
#include "HUD/PlayerHUD.h"
#include "HUD/ScrollableInventoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "PlayerController/MyPlayerController.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for(int32 i = 0; i < NumberOfInventorySlots; i++)
	{
		FInventoriSlot InventorySlot;
		InventorySlot.SlotId = i;
		InventorySlot.ItemAmount  = 0;
		InventorySlot.ItemClass = nullptr;
		InventorySlot.ItemIcon = nullptr;
		InventorySlot.ItemType = EItemType::EIT_MAX;
		
		InventorySlots.Add(InventorySlot);
	}

	//Character->GetWorldTimerManager().SetTimer(GridCreationTimer, this, &UInventoryComponent::InitGridCreation, 0.2f);
	
}

//NOT USED ATM
void UInventoryComponent::InitGridCreation()
{
	Character = Character == nullptr ? Cast<APlayerCharacter>(GetOwner()) : Character;
	if(Character)
	{
		if(Character->Controller)
		{
			//Inefficient but does the job for this prototype
			AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(Character->Controller);
			if(MyPlayerController)
			{
				APlayerHUD* PlayerHUD =  Cast<APlayerHUD>(MyPlayerController->GetHUD());
				if(PlayerHUD && PlayerHUD->InventoryWidget && PlayerHUD->ScrollableInventoryWidget)
				{
					PlayerHUD->InventoryWidget->CreateInventoryGrid(NumberOfInventorySlots);
					PlayerHUD->ScrollableInventoryWidget->CreateInventory(NumberOfInventorySlots);
				}
			}
		}
	}
}

void UInventoryComponent::UpdateItemAmount(const int32 Index, const int32 Change)
{
	if(Index < 0 || Index >= NumberOfInventorySlots) return;

	if(Change < 0)
	{
		 InventorySlots[Index].ItemAmount = FMath::Clamp(InventorySlots[Index].ItemAmount + Change, 0, 999);
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("%d"),  InventorySlots[Index].ItemAmount)
}

void UInventoryComponent::DropItem(const int32 Index)
{
	if(Index < 0 || Index >= NumberOfInventorySlots) return;
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::DropItem"))
	//TODO: Spawn the items in front of the player with the remaining amount of the stack.
	SpawnItemFromInventory(Index, InventorySlots[Index].ItemAmount);
	if(ItemTotalAmountMap.Contains(InventorySlots[Index].ItemClass))
	{
		ItemTotalAmountMap[InventorySlots[Index].ItemClass] -= InventorySlots[Index].ItemAmount;
	}
	InventorySlots[Index].ItemAmount = 0;
	InventorySlots[Index].ItemClass = nullptr;
	InventorySlots[Index].ItemIcon = nullptr;
	InventorySlots[Index].ItemType = EItemType::EIT_MAX;
	
	if(Character) //TODO: This code repeats in multiple places, might need to make it a function
	{
		if(Character->Controller)
		{
			//Inefficient but does the job for this prototype
			AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(Character->Controller);
			if(MyPlayerController)
			{
				MyPlayerController->UpdateInventory(Index);
			}
		}
	}
}

void UInventoryComponent::SpawnItemFromInventory(const int32 InIndex, const int32 InAmount)
{
	Character = Character == nullptr ? Cast<APlayerCharacter>(GetOwner()) : Character;
	UE_LOG(LogTemp, Warning, TEXT("SpawnItemFromInventory: InAmount: %d"), InAmount)
	if(InventorySlots[InIndex].ItemClass && Character)
	{
		
		//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::SpawnItemFromInventory: ItemClass = %s"), *InventorySlots[InIndex].ItemClass->GetName())
		//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::SpawnItemFromInventory: ItemClass->StaticClass = %s"), *InventorySlots[InIndex].ItemClass->StaticClass()->GetName())
		const FActorSpawnParameters SpawnParameters;
		
		for(int i = 0; i < InAmount; i++)
		{
			//TODO: Make the spawning location smarter, so that you can't spawn objects inside other objects or behind walls etc. Maybe with a spring arm?
			auto Item = GetWorld()->SpawnActor<AItem>(InventorySlots[InIndex].ItemClass, Character->GetActorLocation() + FVector(100.f, 0.f, 0.f), FRotator(0.f));
			Item->SetItemState(EItemState::EIS_Dropped);
			Item->GetItemMesh()->SetSimulatePhysics(true);
			
			UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Item->GetName())/*
			UE_LOG(LogTemp, Warning, TEXT("Item Class Name using StaticClass: %s"), *Item->StaticClass()->GetName())
			UE_LOG(LogTemp, Warning, TEXT("Item Class Name using GetClass: %s"), *Item->GetClass()->GetName())
			*/
		}
	}
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UInventoryComponent::ShowInventory(bool bVisible)
{
	Character = Character == nullptr ? Cast<APlayerCharacter>(GetOwner()) : Character;
	if(Character)
	{
		if(Character->Controller)
		{
			//Inefficient but does the job for this prototype
			AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(Character->Controller);
			if(MyPlayerController)
			{
				MyPlayerController->SetInventoryVisibility(bVisible);
			}
		}
	}
}

void UInventoryComponent::AddToInventory(AItem* InItem, int32 InAmount) //TODO: Might need to make this into several smaller functions
{
	int32 ItemIndex{-1};
	UE_LOG(LogTemp, Warning, TEXT("AddToInventory: Class Name: %s"),  *InItem->GetClass()->GetName());
	

	for(auto& Slot : InventorySlots)
	{
		
		if(Slot.ItemClass == InItem->GetClass() && Slot.ItemAmount < InItem->GetMaxStackSize())
		{
			if(Slot.ItemAmount + InAmount <= InItem->GetMaxStackSize())
			{
				Slot.ItemAmount += InAmount;
				if(ItemTotalAmountMap.Contains(Slot.ItemClass))
				{
					ItemTotalAmountMap[Slot.ItemClass] += InAmount;
				}
				else
				{
					ItemTotalAmountMap.Emplace(Slot.ItemClass, InAmount);
				}
				InItem->Destroy();
				Character = Character == nullptr ? Cast<APlayerCharacter>(GetOwner()) : Character;
				if(Character)
				{
					if(Character->Controller)
					{
						//Inefficient but does the job for this prototype
						AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(Character->Controller);
						if(MyPlayerController)
						{
							MyPlayerController->UpdateInventory(Slot.SlotId);
						}
					}
				}
				break;
			}
			else
			{
				if(ItemTotalAmountMap.Contains(Slot.ItemClass))
				{
					ItemTotalAmountMap[Slot.ItemClass] += InAmount;
				}
				else
				{
					ItemTotalAmountMap.Emplace(Slot.ItemClass, InAmount);
				}
				int32 TempAmount = Slot.ItemAmount + InAmount - InItem->GetMaxStackSize();
				Slot.ItemAmount = InItem->GetMaxStackSize();
				AddToInventory(InItem, TempAmount);
				break;
			}
		}
		if(Slot.ItemClass == nullptr)
		{
			Slot.ItemClass = InItem->GetClass();
			Slot.ItemAmount = InAmount;
			Slot.ItemIcon = InItem->GetSlotImage();
			Slot.ItemType = InItem->GetItemType();
			if(Slot.ItemClass->ImplementsInterface(UIngredient::StaticClass()))
			{
				AAlchemyItem* TempAlchemyItem = Cast<AAlchemyItem>(InItem);
				if(TempAlchemyItem)
				{
					Slot.IngredientInfo.IngredientIcon = TempAlchemyItem->GetSlotImage();
					Slot.IngredientInfo.IngredientQuality = TempAlchemyItem->IngredientData.IngredientQuality;
					Slot.IngredientInfo.IngredientQuantityValue = TempAlchemyItem->IngredientData.IngredientQuantityValue;
					Slot.IngredientInfo.PrimarySubstance = TempAlchemyItem->IngredientData.PrimarySubstance;
					Slot.IngredientInfo.SecondarySubstance = TempAlchemyItem->IngredientData.SecondarySubstance;
					Slot.IngredientInfo.TertiarySubstance = TempAlchemyItem->IngredientData.TertiarySubstance;
				}
			}
				
			if(ItemTotalAmountMap.Contains(Slot.ItemClass))
			{
				ItemTotalAmountMap[Slot.ItemClass] += InAmount;
			}
			else
			{
				ItemTotalAmountMap.Emplace(Slot.ItemClass, InAmount);
			}
			InItem->Destroy();
			Character = Character == nullptr ? Cast<APlayerCharacter>(GetOwner()) : Character;
			if(Character)
			{
				if(Character->Controller)
				{
					//Inefficient but does the job for this prototype
					AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(Character->Controller);
					if(MyPlayerController)
					{
						MyPlayerController->UpdateInventory(Slot.SlotId);
					}
				}
			}
			break;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("AddToInventory: Amount in slot 0: %d"), InventorySlots[0].ItemAmount)
	//UE_LOG(LogTemp, Warning, TEXT("AddToInventory: Amount in slot 1: %d"), InventorySlots[1].ItemAmount)
	//UE_LOG(LogTemp, Warning, TEXT("AddToInventory: Total Item Amount: %d"), ItemTotalAmountMap[InItem->GetClass()])
}

