// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "HUD/ItemPopUp.h"
#include "UObject/ConstructorHelpers.h"


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
	ItemMesh->bTraceComplexOnMove = true;
	
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	ItemPopUp->SetVisibility(false);
	ItemMesh->OnComponentWake.AddDynamic(this, &ThisClass::OnComponentWake_Delegate);
	ItemMesh->OnComponentSleep.AddDynamic(this, &ThisClass::OnComponentSleep_Delegate);

	FTimerDelegate TimerDelegate2;
	TimerDelegate2.BindUFunction(this, FName("UpdatePopUp"));
	GetWorldTimerManager().SetTimerForNextTick(TimerDelegate2);

	IQueryable::InitializeGameplayTagContainer(GameplayTagContainer);
}

void AItem::InitGameplayTagContainer()
{
	IQueryable::InitializeGameplayTagContainer(GameplayTagContainer);
}

void AItem::OnConstruction(const FTransform& Transform)
{
	if(bShouldInitGameplayTags)
	{
		InitGameplayTagContainer();
		bShouldInitGameplayTags = false; //Resets to true when restarting the editor
	}
	
	Super::OnConstruction(Transform);
}

void AItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{

	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void AItem::OnComponentWake_Delegate(UPrimitiveComponent* WakingComponent,
	FName BoneName)
{
	if(ItemMesh) ItemMesh->SetUseCCD(true);
}

void AItem::OnComponentSleep_Delegate(UPrimitiveComponent* SleepingComponent, FName BoneName)
{
	if(ItemMesh) ItemMesh->SetUseCCD(false);
}

FGameplayTagContainer& AItem::GetGameplayTagContainer()
{
	return GameplayTagContainer;
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
