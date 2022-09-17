// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyTable.h"

#include "AlchemyProject/InventoryComponent.h"
#include "Components/BoxComponent.h"

AAlchemyTable::AAlchemyTable()
{
	PrimaryActorTick.bCanEverTick = false;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMesh"));
	SetRootComponent(TableMesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	AlchemyInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("AlchemyInventory"));

}

void AAlchemyTable::BeginPlay()
{
	Super::BeginPlay();
	
}


