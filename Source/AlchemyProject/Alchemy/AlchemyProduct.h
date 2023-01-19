// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FAlchemyPackage.h"
#include "AlchemyProject/Item.h"
#include "AlchemyProduct.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitialized);

/**
 * 
 */
UCLASS(Abstract)
class ALCHEMYPROJECT_API AAlchemyProduct : public AItem
{
	GENERATED_BODY()

public:
	
	//BlueprintReadWrite because might need to dynamically add or remove parts of the string
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ProductDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRecipe Recipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FProductInfo ProductInfo;
	
	void Use();

	FOnInitialized OnInitialized;

	//Probably not a good idea to use OnConstruction for this but meesa fix later
	virtual void OnConstruction(const FTransform& Transform) override;

	void InitProperties(const FName& InName);
	virtual void UpdatePopUp() override;

protected:

	virtual void BeginPlay() override;

	

private:


	
};
