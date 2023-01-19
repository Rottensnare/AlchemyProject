// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PotionComponent.h"
#include "TestInvisibilityPotion.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UTestInvisibilityPotion : public UPotionComponent
{
	GENERATED_BODY()

public:


protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void MakePlayerVisible();

	virtual void ExecuteFunctionality() override;

private:

	UPROPERTY(EditAnywhere, Category = "Properties|Values")
	float InvisibilityDuration{5.f};

	FTimerHandle InvisibilityTimer;

public:

	FORCEINLINE void SetInvisibilityDuration(const float InDuration) {InvisibilityDuration = InDuration;} 
	
};
