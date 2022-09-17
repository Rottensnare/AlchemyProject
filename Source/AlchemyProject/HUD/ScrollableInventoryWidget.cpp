// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollableInventoryWidget.h"

#include "InfoBox.h"
#include "InventorySlot.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "Components/ComboBoxString.h"
#include "Components/MultiLineEditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UScrollableInventoryWidget::UpdateInventorySlot(const int32 Index)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;

	InventorySlots[Index]->AmountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerCharacter->GetInventoryComponent()->GetInventory()[Index].ItemAmount)));
}

void UScrollableInventoryWidget::HandleComboSelection(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;

	
	if(SelectionType != ESelectInfo::Type::OnMouseClick && SelectionType != ESelectInfo::Type::OnKeyPress)
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT a MouseClick Event"))
		return;
	}

	if(SelectedItem == "Use")
	{
		PlayerCharacter->UpdateInventorySlotAmount(SelectedSlotIndex, -1);
		UpdateInventorySlot(SelectedSlotIndex);
	}
	else if(SelectedItem == "Drop")
	{
		//PlayerCharacter->UpdateInventorySlotAmount(SelectedSlotIndex, -999);
		UpdateInventorySlot(SelectedSlotIndex);
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

void UScrollableInventoryWidget::CreateInventory(const int32 NumberOfElements)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter == nullptr || PlayerCharacter->GetInventoryComponent() == nullptr) return;

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
			TempSlot->ScrollableInventoryWidget = this;
			TempSlot->HUDInventorySlot = PlayerCharacter->GetInventoryComponent()->GetInventory()[i];
			TempSlot->AmountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TempSlot->HUDInventorySlot.ItemAmount)));
			TempSlot->Padding = 5;
			TempSlot->ComboBox->AddOption(ComboSelections[0]);
			TempSlot->ComboBox->AddOption(ComboSelections[1]);
			TempSlot->ComboBox->AddOption(ComboSelections[2]);
			TempSlot->ComboBox->OnSelectionChanged.AddDynamic(this, &UScrollableInventoryWidget::HandleComboSelection);
			InventorySlots.Add(TempSlot);
			if(IsValid(InventorySlots[i]))
			{
				InventoryScrollBox->AddChild(InventorySlots[i]);
			}
		}
	}
}
