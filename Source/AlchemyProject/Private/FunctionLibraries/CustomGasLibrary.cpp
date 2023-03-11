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
