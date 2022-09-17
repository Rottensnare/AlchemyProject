// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"


AItem::AItem()
{

	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

}


void AItem::BeginPlay()
{
	Super::BeginPlay();

	
}


void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

