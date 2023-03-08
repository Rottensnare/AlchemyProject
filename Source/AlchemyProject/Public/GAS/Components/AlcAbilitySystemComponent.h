// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AlcAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UAlcAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CheckAbilityRangeRequirements();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability|Target")
	float DistanceToTarget;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability|Target", meta = (AllowPrivateAccess = "true"))
	AActor* TargetActor;

	UFUNCTION(BlueprintCallable)
	virtual void SetTargetAndCheckRange(AActor* InAvatarActor) const;

	UFUNCTION(BlueprintCallable)
	TArray<FGameplayAbilitySpecHandle> GetAbilitiesInRange();

public:

	FORCEINLINE void SetTargetActor(AActor* InActor) {TargetActor = InActor;}
};
