// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "PlayerOverlay.h"
#include "AlchemyProject/Alchemy/AlchemyOverlay.h"
#include "AlchemyProject/PlayerController/MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void APlayerHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if(PlayerController && OverlayClass)
	{
		PlayerOverlay = CreateWidget<UPlayerOverlay>(PlayerController, OverlayClass);
		PlayerOverlay->AddToViewport();
	}
	if(PlayerController && AlchemyOverlayClass)
	{
		AlchemyOverlay = CreateWidget<UAlchemyOverlay>(PlayerController, AlchemyOverlayClass);
		AlchemyOverlay->AddToViewport();
		AlchemyOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::UpdateHealthBar(const float InHealth, const float InMaxHealth)
{
	if(PlayerOverlay == nullptr || PlayerOverlay->HealthBar == nullptr || PlayerOverlay->HealthText == nullptr) return;

	PlayerOverlay->HealthBar->SetPercent(InHealth/InMaxHealth);
	PlayerOverlay->HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::TruncToInt(InHealth), FMath::TruncToInt(InMaxHealth))));
}


