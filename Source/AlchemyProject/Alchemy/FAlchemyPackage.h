#pragma once

#include "CoreMinimal.h"
#include "FIngredientInfo.h"
#include "FAlchemyPackage.generated.h"

USTRUCT(BlueprintType)
struct FAlchemyPackage
{
	GENERATED_BODY()

	TArray<FIngredientInfo> IngredientInfos;
	UPROPERTY(EditAnywhere)
	class AAlchemyBase* AlchemyBase;
};

USTRUCT()
struct FRecipe
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<EPrimarySubstance> RequiredSubstances;

	UPROPERTY(EditAnywhere)
	AAlchemyBase* RequiredBase;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APotion> PotionClass;

	UPROPERTY(EditAnywhere)
	float BaseSuccessChance{1.f};
};
