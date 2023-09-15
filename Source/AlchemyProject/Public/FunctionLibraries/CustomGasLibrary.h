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
	
public:

	UFUNCTION(BlueprintCallable, Category = "Gameplay Tags")
	static FGameplayTagContainer AddTagsToGameplayTagContainer(const FGameplayTagContainer& OutGameplayTagContainer, const FGameplayTagContainer& TagsToAdd);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Tags")
	static FGameplayTagContainer RemoveTagsFromGameplayTagContainer(const FGameplayTagContainer& OutGameplayTagContainer, const FGameplayTagContainer& TagsToRemove);

	UFUNCTION(BlueprintPure, Category = "GOAP")
	static bool IsCloseEnough(const FVector& CurrentLocation, const FVector& TargetLocation, const float Threshold, const bool bIgnoreZAxis = false);
};
