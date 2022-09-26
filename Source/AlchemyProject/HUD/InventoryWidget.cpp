// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InfoBox.h"
#include "InventorySlot.h"
#include "AlchemyProject/Ingredient.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "AlchemyProject/Alchemy/Potion.h"
#include "AlchemyProject/PlayerController/MyPlayerController.h"
#include "Components/ComboBoxString.h"
#include "Components/Image.h"
#include "Components/MultiLineEditableText.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"

#define INVENTORY(x) PlayerCharacter->GetInventoryComponent()->GetInventory()[x]

void UInventoryWidget::CreateInventoryGrid(const int32 NumberOfElements)
{
	PlayerCharacter = PlayerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwningPlayerPawn()) : PlayerCharacter;
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;
	
	
	UE_LOG(LogTemp, Warning, TEXT("Creating Inventory Grid"))
	if(InventorySlotClass && InfoBoxClass)
	{
		ComboSelections.Add("Use");
		ComboSelections.Add("Drop");
		ComboSelections.Add("Show Description");

		InfoBox = CreateWidget<UInfoBox>(this, InfoBoxClass);
		InfoBox->AddToViewport();
		InfoBox->SetVisibility(ESlateVisibility::Collapsed);
		
		for(int32 i = 0; i < NumberOfElements; i++)
		{
			UInventorySlot* TempSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
			TempSlot->bIsFocusable = true;
			TempSlot->InventoryWidget = this;
			TempSlot->HUDInventorySlot = PlayerCharacter->GetInventoryComponent()->GetInventory()[i];
			TempSlot->AmountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TempSlot->HUDInventorySlot.ItemAmount)));
			TempSlot->Padding = 5;
			TempSlot->ComboBox->AddOption(ComboSelections[0]);
			TempSlot->ComboBox->AddOption(ComboSelections[1]);
			TempSlot->ComboBox->AddOption(ComboSelections[2]);
			TempSlot->ComboBox->OnSelectionChanged.AddDynamic(this, &UInventoryWidget::HandleComboSelection);
			InventorySlots.Add(TempSlot);
		}
		
		for(int i = 0; i < NumberOfRows; i++)
		{
			for(int j = 0; j < NumberOfColumns; j++)
			{
				const int32 Index = j + (i * NumberOfColumns);
				if(IsValid(InventorySlots[Index]))
				{
					//UE_LOG(LogTemp, Warning, TEXT("Adding inventory slot #%d"), Index)
					InventoryGrid->AddChildToUniformGrid(InventorySlots[Index], i, j);
					if(InventoryGrid->GetChildrenCount() >= NumberOfElements)
					{
						return;
					}
				}
			}
		}
	}
}

void UInventoryWidget::CreateContainerGrid(const int32 NumberOfElements)
{
	if(InventorySlotClass)
	{
		for(int i = 0; i < NumberOfElements; i++)
		{
			UInventorySlot* TempSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
			TempSlot->InventoryWidget = this;
			TempSlot->ComboBox->SetVisibility(ESlateVisibility::Collapsed);
			TempSlot->ComboBox->SetIsEnabled(false);
			if(i < 4)
			{
				TempSlot->IngredientType = EIngredientType::EIT_Substance;
			}
			else if(i == 4)
			{
				TempSlot->IngredientType = EIngredientType::EIT_Special;
			}
			else if(i == 5)
			{
				TempSlot->IngredientType = EIngredientType::EIT_Base;
			}
			else if(i == 6)
			{
				TempSlot->IngredientType = EIngredientType::EIT_Catalyst;
			}
			else if(i == 7)
			{
				TempSlot->IngredientType = EIngredientType::EIT_Container;
			}
			else
			{
				TempSlot->IngredientType = EIngredientType::EIT_All;
			}
			InventorySlots.Add(TempSlot);
			InventoryGrid->AddChildToUniformGrid(TempSlot, 0, i);
		}	
	}
}

void UInventoryWidget::UpdateInventorySlot(const int32 Index)
{
	PlayerCharacter = PlayerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwningPlayerPawn()) : PlayerCharacter;
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;

	if(PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemAmount > 0)
	{
		UpdateSlotFromInventory(Index);
	}
	//InventorySlots[Index]->AmountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemAmount)));
}

void UInventoryWidget::HandleComboSelection(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	PlayerCharacter = PlayerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwningPlayerPawn()) : PlayerCharacter;
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;

	
	if(SelectionType != ESelectInfo::Type::OnMouseClick) UE_LOG(LogTemp, Warning, TEXT("NOT a MouseClick Event"))

	if(SelectedItem == "Use")
	{
		if(true) //TODO: FIX THIS
		{
			PlayerCharacter->UsePotion(INVENTORY(SelectedSlotIndex).ProductInfo.PotionComponentClass, INVENTORY(SelectedSlotIndex).ProductInfo.ProductQuality);
		}
		PlayerCharacter->UpdateInventorySlotAmount(SelectedSlotIndex, -1);
		
		if(INVENTORY(SelectedSlotIndex).ItemAmount <= 0)
		{
			PlayerCharacter->GetInventoryComponent()->DropItem(SelectedSlotIndex);
		}
		else
		{
			UpdateInventorySlot(SelectedSlotIndex);
		}
	}
	else if(SelectedItem == "Drop")
	{
		//PlayerCharacter->UpdateInventorySlotAmount(SelectedSlotIndex, -999);
		//UpdateInventorySlot(SelectedSlotIndex);
		PlayerCharacter->GetInventoryComponent()->DropItem(SelectedSlotIndex);
	}
	else if(SelectedItem == "Show Description")
	{
		if(InfoBox->bCollapsed)
		{
			if(GEngine && GEngine->GameViewport)
			{
				GEngine->GameViewport->GetViewportSize(ScreenMiddle);
				ScreenMiddle /= 2;
			}
			InfoBox->SetPositionInViewport(ScreenMiddle);
			InfoBox->SetVisibility(ESlateVisibility::HitTestInvisible);
			InfoBox->bCollapsed = false;
			UpdateInfoBox();
			
		}
		else
		{
			InfoBox->SetVisibility(ESlateVisibility::Collapsed);
			InfoBox->bCollapsed = true;
			InfoBox->DescriptionTextBox->SetText(FText());
		}
	}else if(SelectedItem == "Transfer")
	{
		AMyPlayerController* TempController = Cast<AMyPlayerController>(PlayerCharacter->GetController());
		if(TempController)
		{
			TempController->SelectAlchemyIngredient(SelectedSlotIndex);
		}
	}

	InventorySlots[SelectedSlotIndex]->ComboBox->ClearSelection();
	InventorySlots[SelectedSlotIndex]->ComboBox->SetVisibility(ESlateVisibility::Collapsed);
	

}

void UInventoryWidget::UpdateSlotFromInventory(const int32 Index)
{
	PlayerCharacter = PlayerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwningPlayerPawn()) : PlayerCharacter;
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;

	InventorySlots[Index]->AmountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemAmount)));
	InventorySlots[Index]->SlotIcon->SetBrushFromTexture(PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemIcon);
	InventorySlots[Index]->ComboBox->ClearOptions();
	if(INVENTORY(Index).ItemType == EItemType::EIT_Consumable)
	{
		InventorySlots[Index]->ComboBox->AddOption("Use");
	}
	InventorySlots[Index]->ComboBox->AddOption("Drop");
	InventorySlots[Index]->ComboBox->AddOption("Show Description");
	
	
}

void UInventoryWidget::UpdateAllSlots()
{
	PlayerCharacter = PlayerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwningPlayerPawn()) : PlayerCharacter;
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;
	int32 Index = 0;
	for(auto& Slut : InventorySlots)
	{
		Slut->AmountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemAmount)));
		Slut->SlotIcon->SetBrushFromTexture(PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemIcon);
		Slut->ComboBox->ClearOptions();
		if(INVENTORY(Index).ItemType == EItemType::EIT_Ingredient)
		{
			Slut->ComboBox->AddOption("Transfer");
			Slut->ComboBox->AddOption("Taste_NI");
		}else if(INVENTORY(Index).ItemType == EItemType::EIT_Consumable)
		{
			Slut->ComboBox->AddOption("Use");
		}
		Slut->ComboBox->AddOption("Description");
		Slut->ComboBox->AddOption("Drop");
		Slut->ComboBox->SetVisibility(ESlateVisibility::Collapsed);
		Index++;
	}
}

void UInventoryWidget::UpdateInfoBox()
{
	PlayerCharacter = PlayerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwningPlayerPawn()) : PlayerCharacter;
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;

	if(INVENTORY(SelectedSlotIndex).ItemClass->ImplementsInterface(UIngredient::StaticClass()))
	{
	InfoBox->DescriptionTextBox->SetText(FText::FromString(FString::Printf(TEXT(
	"Ingredient: %s \n"
	"Primary substance: %s \n"
	"Secondary substance: %s \n"
	"Tertiary substance: %s \n"
	"Quality: %s \n"
	"Quantity value: %s \n") ,
	*INVENTORY(SelectedSlotIndex).ItemClass->GetName(),
	*UEnum::GetDisplayValueAsText(INVENTORY(SelectedSlotIndex).IngredientInfo.PrimarySubstance).ToString(),
	*UEnum::GetDisplayValueAsText(INVENTORY(SelectedSlotIndex).IngredientInfo.SecondarySubstance).ToString(),
	*UEnum::GetDisplayValueAsText(INVENTORY(SelectedSlotIndex).IngredientInfo.TertiarySubstance).ToString(),
	*UEnum::GetDisplayValueAsText(INVENTORY(SelectedSlotIndex).IngredientInfo.IngredientQuality).ToString(),
	*UEnum::GetDisplayValueAsText(INVENTORY(SelectedSlotIndex).IngredientInfo.IngredientQuantityValue).ToString()
	)));
	}
	
}
