// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomGasLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UCustomGasLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	UFUNCTION(BlueprintCallable, Category = "Gameplay Tags")
	static FGameplayTagContainer AddTagsToGameplayTagContainer(const FGameplayTagContainer& OutGameplayTagContainer, const FGameplayTagContainer& TagsToAdd);
};
