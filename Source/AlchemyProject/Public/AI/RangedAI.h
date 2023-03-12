// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBase.h"
#include "RangedAI.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API ARangedAI : public AAIBase
{
	GENERATED_BODY()

public:
	

protected:

	virtual void HandleEnemySeen(AActor* Enemy) override;

	virtual void HandleLostEnemy(AActor* Enemy) override;
	
};
