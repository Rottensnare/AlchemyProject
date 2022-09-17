// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "InventoryWidget.h"
#include "PlayerOverlay.h"
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
}


