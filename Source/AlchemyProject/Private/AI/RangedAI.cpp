// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RangedAI.h"

void ARangedAI::HandleEnemySeen(AActor* Enemy)
{
	float DistanceToEnemy = GetDistanceTo(Enemy);
	SetBeingCareful(false);
	
	Super::HandleEnemySeen(Enemy);
}

void ARangedAI::HandleLostEnemy(AActor* Enemy)
{
	SetBeingCareful(true);
	
	Super::HandleLostEnemy(Enemy);
}
