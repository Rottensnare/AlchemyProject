// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SquadManager.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API USquadManager : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Squad)
	TArray<class AAIBase*> SquadMembers;

	int32 Squad_ID{-1};

	//Called when first assigning to squad or when changing squads
	UFUNCTION(BlueprintCallable)
	void AddToSquad(AAIBase* AIToAdd);

	//Should be called when AI dies
	UFUNCTION(BlueprintCallable)
	void RemoveFromSquad(AAIBase* AIToRemove);
};
