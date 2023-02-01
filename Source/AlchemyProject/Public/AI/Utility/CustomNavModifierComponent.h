// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavModifierComponent.h"
#include "CustomNavModifierComponent.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UCustomNavModifierComponent : public UNavModifierComponent
{
	GENERATED_BODY()

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditAnywhere, Category = "NavArea")
	float Cost{0.8f};

	UPROPERTY(EditAnywhere, Category = "NavArea")
	float AreaEnteringCost{0.f};
};
