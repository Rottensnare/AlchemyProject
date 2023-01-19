// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomAIContainer.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ALCHEMYPROJECT_API UCustomAIContainer : public UObject
{
	GENERATED_BODY()

	
public:
	
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> ActorContainer;
	
};
