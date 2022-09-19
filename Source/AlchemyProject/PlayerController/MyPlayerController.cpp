// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "AlchemyProject/Ingredient.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "AlchemyProject/Alchemy/AlchemyOverlay.h"
#include "AlchemyProject/Alchemy/IngredientData.h"
#include "AlchemyProject/HUD/InfoBox.h"
#include "AlchemyProject/HUD/InventorySlot.h"
#include "AlchemyProject/HUD/InventoryWidget.h"
#include "AlchemyProject/HUD/PlayerHUD.h"
#include "AlchemyProject/HUD/PlayerOverlay.h"
#include "AlchemyProject/HUD/ScrollableInventoryWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

void AMyPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD == nullptr) return;

	bool bIsHUDValid =	PlayerHUD->PlayerOverlay &&
						PlayerHUD->PlayerOverlay->HealthBar &&
						PlayerHUD->PlayerOverlay->HealthText;

	if(bIsHUDValid)
	{
		PlayerHUD->PlayerOverlay->HealthBar->SetPercent(Health / MaxHealth);
		const FText HealthTxt = FText::FromString(FString::Printf(TEXT("%d : %d"), FMath::TruncToInt32(Health), FMath::TruncToInt32(MaxHealth)));
		PlayerHUD->PlayerOverlay->HealthText->SetText(HealthTxt);
	}
}

void AMyPlayerController::SetInventoryVisibility(bool bVisible)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD == nullptr) return;

	bool bIsHUDValid =	PlayerHUD->PlayerOverlay &&
						PlayerHUD->PlayerOverlay->InventoryWidget &&
							PlayerHUD->PlayerOverlay->ScrollableInventoryWidget;
	if(bIsHUDValid)
	{
		if(bVisible)
		{
			if(bUseScrollableInventory)
			{
				PlayerHUD->PlayerOverlay->ScrollableInventoryWidget->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				PlayerHUD->PlayerOverlay->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			if(bUseScrollableInventory)
			{
				PlayerHUD->PlayerOverlay->ScrollableInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				PlayerHUD->PlayerOverlay->InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void AMyPlayerController::SetInventoryGrid(int32 NumberOfSlots)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD == nullptr) return;

	bool bIsHUDValid =	PlayerHUD->AlchemyOverlay &&
						PlayerHUD->PlayerOverlay &&
						PlayerHUD->PlayerOverlay->InventoryWidget &&
						PlayerHUD->PlayerOverlay->ScrollableInventoryWidget;
	if(bIsHUDValid)
	{
		if(bUseScrollableInventory)
		{
			PlayerHUD->PlayerOverlay->ScrollableInventoryWidget->CreateInventory(NumberOfSlots);
		}
		else
		{
			PlayerHUD->PlayerOverlay->InventoryWidget->CreateInventoryGrid(NumberOfSlots);
			PlayerHUD->AlchemyOverlay->CharacterInventory->CreateInventoryGrid(NumberOfSlots);
		}
		
	}
}

void AMyPlayerController::UpdateInventory(const int32 Index)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD && PlayerHUD->PlayerOverlay && PlayerHUD->PlayerOverlay->InventoryWidget && PlayerHUD->AlchemyOverlay && PlayerHUD->AlchemyOverlay->CharacterInventory)
	{
		PlayerHUD->PlayerOverlay->InventoryWidget->UpdateSlotFromInventory(Index);
		
	}
}

void AMyPlayerController::ToggleAlchemyOverlay()
{
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetCharacter()) : CurrentCharacter;
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD && PlayerHUD->AlchemyOverlay && PlayerHUD->AlchemyOverlay->CharacterInventory && PlayerHUD->PlayerOverlay && CurrentCharacter)
	{
		if(PlayerHUD->AlchemyOverlay->GetVisibility() == ESlateVisibility::Collapsed)
		{
			PlayerHUD->AlchemyOverlay->SetVisibility(ESlateVisibility::Visible);
			PlayerHUD->PlayerOverlay->SetVisibility(ESlateVisibility::Collapsed);
			FInputModeGameAndUI InputModeGameAndUI;
			SetInputMode(InputModeGameAndUI);
			SetShowMouseCursor(true);
			PlayerHUD->AlchemyOverlay->CharacterInventory->UpdateAllSlots();
			CurrentCharacter->bIsDoingAlchemy = true;
			
		}
		else
		{
			PlayerHUD->AlchemyOverlay->SetVisibility(ESlateVisibility::Collapsed);
			PlayerHUD->PlayerOverlay->SetVisibility(ESlateVisibility::Visible);
			FInputModeGameOnly InputModeGameOnly;
			SetInputMode(InputModeGameOnly);
			SetShowMouseCursor(false);
			CurrentCharacter->bIsDoingAlchemy = false;
			ClearAlchemySelection();
			
		}
	}
}

void AMyPlayerController::SelectAlchemyIngredient(const int32 SelectedSlot)
{
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetCharacter()) : CurrentCharacter;
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(!PlayerHUD || !PlayerHUD->AlchemyOverlay || !PlayerHUD->AlchemyOverlay->CharacterInventory || !PlayerHUD->PlayerOverlay || !CurrentCharacter) return;

	if(CurrentCharacter->GetInventoryComponent()->GetInventory()[SelectedSlot].ItemClass->ImplementsInterface(UIngredient::StaticClass())) //If item is an ingredient
	{
		UE_LOG(LogTemp, Warning, TEXT("Primary Substance: %s"), *UEnum::GetDisplayValueAsText(CurrentCharacter->GetInventoryComponent()->GetInventory()[SelectedSlot].IngredientInfo.PrimarySubstance).ToString());
		if(CurrentCharacter->GetInventoryComponent()->GetInventory()[SelectedSlot].IngredientInfo.IngredientType == EIngredientType::EIT_Substance)
		{
			for(int i = 0; i < 4; i++)
			{
				UInventorySlot* TempSlot = Cast<UInventorySlot>(PlayerHUD->AlchemyOverlay->TableInventory->GetInventoryGrid()->GetChildAt(i));
				if(TempSlot && TempSlot->bEmpty)
				{
					TempSlot->InventoryIndex = SelectedSlot;
					TempSlot->SlotIcon->SetBrushFromTexture(CurrentCharacter->GetInventoryComponent()->GetInventory()[SelectedSlot].ItemIcon);
					TempSlot->SlotIcon->SetOpacity(AlchemyItemIconOpacity);
					TempSlot->bEmpty = false;
					TempSlot->BackgroundImage->SetOpacity(AlchemyItemBGOpacity);
					break;
				}
			}
		}
	}

}

void AMyPlayerController::ClearAlchemySelection(const int32 Index)
{
	if(Index == -1)
	{
		for(int i = 0; i < 9; i++)
		{
			UInventorySlot* TempSlot = Cast<UInventorySlot>(PlayerHUD->AlchemyOverlay->TableInventory->GetInventoryGrid()->GetChildAt(i));
			if(TempSlot)
			{
				TempSlot->InventoryIndex = -1;
				TempSlot->bEmpty = true;
				TempSlot->SlotIcon->SetBrush(FSlateBrush());
				TempSlot->SlotIcon->SetOpacity(0.f);
				TempSlot->BackgroundImage->SetOpacity(1.f);
			}
		}
	}
	else
	{
		UInventorySlot* TempSlot = Cast<UInventorySlot>(PlayerHUD->AlchemyOverlay->TableInventory->GetInventoryGrid()->GetChildAt(Index));
		if(TempSlot)
		{
			TempSlot->InventoryIndex = -1;
			TempSlot->bEmpty = true;
			TempSlot->SlotIcon->SetBrush(FSlateBrush());
			TempSlot->SlotIcon->SetOpacity(0.f);
			TempSlot->BackgroundImage->SetOpacity(1.f);
		}
	}
	
}


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD)
	{
		PlayerHUD->AddCharacterOverlay();
	}
}


