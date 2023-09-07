// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GOAP/Character/AgentBase.h"


AAgentBase::AAgentBase()
{

	PrimaryActorTick.bCanEverTick = true;

}


void AAgentBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AAgentBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAgentBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

