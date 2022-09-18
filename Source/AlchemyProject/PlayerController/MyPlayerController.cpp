// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "AlchemyProject/Alchemy/AlchemyOverlay.h"
#include "AlchemyProject/HUD/InfoBox.h"
#include "AlchemyProject/HUD/InventorySlot.h"
#include "AlchemyProject/HUD/InventoryWidget.h"
#include "AlchemyProject/HUD/PlayerHUD.h"
#include "AlchemyProject/HUD/PlayerOverlay.h"
#include "AlchemyProject/HUD/ScrollableInventoryWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if(PlayerHUD && PlayerHUD->AlchemyOverlay && PlayerHUD->AlchemyOverlay->CharacterInventory && PlayerHUD->PlayerOverlay)
	{
		if(PlayerHUD->AlchemyOverlay->GetVisibility() == ESlateVisibility::Collapsed)
		{
			PlayerHUD->AlchemyOverlay->SetVisibility(ESlateVisibility::Visible);
			PlayerHUD->PlayerOverlay->SetVisibility(ESlateVisibility::Collapsed);
			FInputModeGameAndUI InputModeGameAndUI;
			SetInputMode(InputModeGameAndUI);
			SetShowMouseCursor(true);
			PlayerHUD->AlchemyOverlay->CharacterInventory->UpdateAllSlots();
			
		}
		else
		{
			PlayerHUD->AlchemyOverlay->SetVisibility(ESlateVisibility::Collapsed);
			PlayerHUD->PlayerOverlay->SetVisibility(ESlateVisibility::Visible);
			FInputModeGameOnly InputModeGameOnly;
			SetInputMode(InputModeGameOnly);
			SetShowMouseCursor(false);
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


