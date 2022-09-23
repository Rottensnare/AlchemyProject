// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProject/Alchemy/FAlchemyPackage.h"
#include "Components/ActorComponent.h"
#include "AlchemyComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMYPROJECT_API UAlchemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	friend class APlayerCharacter;
	UAlchemyComponent();

protected:
	
	virtual void BeginPlay() override;

private:
	
	UFUNCTION(BlueprintCallable)
	void CreateAlchemyProduct(const FAlchemyPackage& AlchemyPackage);
	
	UFUNCTION(BlueprintCallable)
	FAlchemyPackage CreateAlchemyPackage(const TArray<FIngredientInfo>& InIngredientInfos, AAlchemyBase* InAlchemyBase);

	UPROPERTY(EditAnywhere)
	TArray<FName> KnownRecipeNames;

	UPROPERTY(VisibleAnywhere)
	class APlayerCharacter* Character;

	UPROPERTY()
	class AAlchemyProduct* Aitem;

	UFUNCTION()
	void AddAitemToInventory();

public:

	FORCEINLINE TArray<FName>& GetKnownRecipes() {return KnownRecipeNames;}
};

