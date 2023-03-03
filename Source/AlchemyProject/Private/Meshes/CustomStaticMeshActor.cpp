// Fill out your copyright notice in the Description page of Project Settings.


#include "Meshes/CustomStaticMeshActor.h"

// Sets default values
ACustomStaticMeshActor::ACustomStaticMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomStaticMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomStaticMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

