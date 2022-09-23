// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "PlayerOverlay.h"
#include "AlchemyProject/Alchemy/AlchemyOverlay.h"
#include "AlchemyProject/PlayerController/MyPlayerController.h"
#include "Blueprint/UserWidget.h"

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


