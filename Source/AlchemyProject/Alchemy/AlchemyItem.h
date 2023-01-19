// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FIngredientInfo.h"
#include "AlchemyProject/Ingredient.h"
#include "AlchemyProject/Item.h"
#include "AlchemyItem.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API AAlchemyItem : public AItem, public IIngredient
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIngredientInfo IngredientData;
	
	virtual void UpdatePopUp() override;

protected:

	virtual void InitGameplayTagContainer() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
private:

	FGameplayTag PrimarySubstanceGameplayTag;
	
};
