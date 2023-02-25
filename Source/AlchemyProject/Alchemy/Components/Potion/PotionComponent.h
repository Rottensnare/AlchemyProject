// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProject/Enums/EIngredientQuality.h"
#include "Components/ActorComponent.h"
#include "PotionComponent.generated.h"

USTRUCT(BlueprintType)
struct FQualityCurve : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCurveFloat* FloatCurve;
	
};


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMYPROJECT_API UPotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPotionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ExecuteFunctionality();

	UFUNCTION(BlueprintCallable)
	virtual void DestroyThisComponent();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	
	UPROPERTY(BlueprintReadWrite)
	class APlayerCharacter* CurrentCharacter;

	UPROPERTY(BlueprintReadOnly)
	EProductQuality ProductQuality;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveFloat* QualityCurve;

	UPROPERTY(EditDefaultsOnly)
	FName PotionName{NAME_None};

	//This is here if someone (Me) forgets to select the curve but it's present in the data table.
	UCurveFloat* GetCurveFromDataTable();

private:

public:

	FORCEINLINE void SetProductQuality(const EProductQuality InProductQuality) {ProductQuality = InProductQuality;}
	
		
};
