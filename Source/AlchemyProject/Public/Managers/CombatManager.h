// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatManager.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UCombatManager : public UObject
{
	GENERATED_BODY()

public:

	void ReportCombat(TObjectPtr<class AAIBase> InActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Squad)
	TArray<class USquadManager*> Squads;
};
