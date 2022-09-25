#pragma once

#include "CoreMinimal.h"
#include "FIngredientInfo.h"
#include "Engine/DataTable.h"
#include "FAlchemyPackage.generated.h"

USTRUCT(BlueprintType)
struct FAlchemyPackage
{
	GENERATED_BODY()

	TArray<FIngredientInfo> IngredientInfos;
	UPROPERTY(EditAnywhere)
	class AAlchemyBase* AlchemyBase;
};

USTRUCT(BlueprintType)
struct FRecipeTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EPrimarySubstance, int32> AmountPerSubstanceMap;

	UPROPERTY(EditAnywhere)
	AAlchemyBase* RequiredBase;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAlchemyProduct> AlchemyClass;

	UPROPERTY(EditAnywhere)
	float BaseSuccessChance{1.f};
	
};

USTRUCT(BlueprintType)
struct FRecipe
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EPrimarySubstance, int32> AmountPerSubstanceMap;

	UPROPERTY(EditAnywhere)
	AAlchemyBase* RequiredBase;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAlchemyProduct> AlchemyClass;

	UPROPERTY(EditAnywhere)
	float BaseSuccessChance{1.f};

	//Inline functions shouldn't have looping, but compiler didn't give me an error, so let it be for now.
	FORCEINLINE bool operator ==(const FRecipe& Recipe) const
	{
		for(const auto& Subs : Recipe.AmountPerSubstanceMap)
		{
			if(!AmountPerSubstanceMap.Contains(Subs.Key))
			{
				return false;
			}
		}
		return Recipe.RequiredBase != RequiredBase;
	}
};
//Hash will be constructed based on this struct, which will be compared in the inventory system
//Contains all the necessary data for applying product values when creating or consuming the product
USTRUCT(BlueprintType)
struct FProductInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAlchemyProduct> AlchemyClass;

	//Name of the potion
	UPROPERTY(EditAnywhere)
	FName Name;

	//Main factor in determining the potency
	UPROPERTY(EditAnywhere)
	EProductQuality ProductQuality;

	
	
};
