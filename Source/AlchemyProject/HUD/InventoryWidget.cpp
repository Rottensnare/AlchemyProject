// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InfoBox.h"
#include "InventorySlot.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "Components/ComboBoxString.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/MultiLineEditableText.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::CreateInventoryGrid(const int32 NumberOfElements)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
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
					UE_LOG(LogTemp, Warning, TEXT("Adding inventory slot #%d"), Index)
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

void UInventoryWidget::UpdateInventorySlot(const int32 Index)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;

	if(PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemAmount <= 0)
	{
		UpdateSlotFromInventory(Index);
	}
	//InventorySlots[Index]->AmountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemAmount)));
}

void UInventoryWidget::HandleComboSelection(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;

	
	if(SelectionType != ESelectInfo::Type::OnMouseClick) UE_LOG(LogTemp, Warning, TEXT("NOT a MouseClick Event"))

	if(SelectedItem == "Use")
	{
		PlayerCharacter->UpdateInventorySlotAmount(SelectedSlotIndex, -1);
		UpdateInventorySlot(SelectedSlotIndex);
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
			InfoBox->DescriptionTextBox->SetText(InventorySlots[SelectedSlotIndex]->AmountText->Text);
		}
		else
		{
			InfoBox->SetVisibility(ESlateVisibility::Collapsed);
			InfoBox->bCollapsed = true;
			InfoBox->DescriptionTextBox->SetText(FText());
		}
	}

	InventorySlots[SelectedSlotIndex]->ComboBox->ClearSelection();
	

}

void UInventoryWidget::UpdateSlotFromInventory(const int32 Index)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;

	InventorySlots[Index]->AmountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemAmount)));
	InventorySlots[Index]->SlotIcon->SetBrushFromTexture(PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemIcon);
	InventorySlots[Index]->ComboBox->ClearOptions();
	if(PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemType != EItemType::EIT_Consumable)
	{
		InventorySlots[Index]->ComboBox->AddOption("Drop");
		InventorySlots[Index]->ComboBox->AddOption("Show Description");
	}
	else
	{
		
		InventorySlots[Index]->ComboBox->AddOption("Use");
		InventorySlots[Index]->ComboBox->AddOption("Drop");
		InventorySlots[Index]->ComboBox->AddOption("Show Description");
	}
	
}
