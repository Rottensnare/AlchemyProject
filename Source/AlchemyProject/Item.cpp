// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "HUD/ItemPopUp.h"


AItem::AItem()
{

	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	ItemPopUp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemPopUp"));
	ItemPopUp->SetupAttachment(GetRootComponent());
	ItemPopUp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	const ConstructorHelpers::FClassFinder<UUserWidget> PopupClass (TEXT("/Game/Blueprints/HUD/WBP_ItemPopUp"));
	ItemPopUp->SetWidgetClass(PopupClass.Class);
	ItemPopUp->SetDrawSize(FVector2D(400.f, 200.f));
	ItemPopUp->SetWidgetSpace(EWidgetSpace::Screen);
	
}




void AItem::BeginPlay()
{
	Super::BeginPlay();

	ItemPopUp->SetVisibility(false);

	FTimerDelegate TimerDelegate2;
	TimerDelegate2.BindUFunction(this, FName("UpdatePopUp"));
	GetWorldTimerManager().SetTimerForNextTick(TimerDelegate2);
	
}

void AItem::UpdatePopUp()
{
	if(ItemPopUp)
	{
		if(const UItemPopUp* const PopUpWidget = Cast<UItemPopUp>(ItemPopUp->GetWidget()))
		{
			PopUpWidget->SetBlockText(PopUpText);
		}
	}
}

void AItem::ShowItemPopupWidget() const
{
	if(ItemPopUp == nullptr) return;
	if(ItemPopUp->IsWidgetVisible()) ItemPopUp->SetVisibility(false);
	else ItemPopUp->SetVisibility(true);
}
