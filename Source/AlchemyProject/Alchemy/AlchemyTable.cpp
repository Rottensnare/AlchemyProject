// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyTable.h"

#include "AlchemyProject/InventoryComponent.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "AlchemyProject/PlayerController/MyPlayerController.h"
#include "Components/BoxComponent.h"

AAlchemyTable::AAlchemyTable()
{
	PrimaryActorTick.bCanEverTick = false;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMesh"));
	SetRootComponent(TableMesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	PotionSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PotionSpawnPoint"));
	PotionSpawnPoint->SetupAttachment(RootComponent);

	AlchemyInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("AlchemyInventory"));

}

void AAlchemyTable::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AAlchemyTable::OnBoxOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AAlchemyTable::OnBoxEndOverlap);
	
}

void AAlchemyTable::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* TempCharacter = Cast<APlayerCharacter>(OtherActor);
	if(TempCharacter)
	{
		CurrentCharacter = TempCharacter;
		TempCharacter->SetAlchemyTable(this);
		AMyPlayerController* TempController = Cast<AMyPlayerController>(TempCharacter->Controller);
		if(TempController)
		{
			TempController->ToggleAlchemyOverlay();
		}
	}
}

void AAlchemyTable::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* TempCharacter = Cast<APlayerCharacter>(OtherActor);
	if(TempCharacter)
	{
		CurrentCharacter = nullptr;
		TempCharacter->SetAlchemyTable(nullptr);
		AMyPlayerController* TempController = Cast<AMyPlayerController>(TempCharacter->Controller);
		if(TempController)
		{
			TempController->ToggleAlchemyOverlay();
		}
	}
}


