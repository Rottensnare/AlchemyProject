// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PotionComponent.h"
#include "TestHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UTestHealthPotion : public UPotionComponent
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

	virtual void DestroyThisComponent() override;

private:

	UPROPERTY(EditAnywhere)
	float InstantHealAmount{25.f};
	
};
