// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibraries/CustomGasLibrary.h"

FGameplayTagContainer UCustomGasLibrary::AddTagsToGameplayTagContainer(const FGameplayTagContainer& OutGameplayTagContainer,
	const FGameplayTagContainer& TagsToAdd)
{
	FGameplayTagContainer ReturnContainer = OutGameplayTagContainer;
	for(int i = 0; i < TagsToAdd.Num(); i++)
	{
		ReturnContainer.AddTag(TagsToAdd.GetByIndex(i));
	}

	return ReturnContainer;
}

FGameplayTagContainer UCustomGasLibrary::RemoveTagsFromGameplayTagContainer(
	const FGameplayTagContainer& OutGameplayTagContainer, const FGameplayTagContainer& TagsToRemove)
{
	FGameplayTagContainer ReturnContainer = OutGameplayTagContainer;
	ReturnContainer.RemoveTags(TagsToRemove);
	return ReturnContainer;
}

bool UCustomGasLibrary::IsCloseEnough(const FVector& CurrentLocation, const FVector& TargetLocation,
	const float Threshold, const bool bIgnoreZAxis)
{
	if(bIgnoreZAxis == false)
	{
		return FVector::Dist(CurrentLocation, TargetLocation) <= Threshold;
	}

	return FVector::Dist2D(CurrentLocation, TargetLocation) < Threshold;
}
