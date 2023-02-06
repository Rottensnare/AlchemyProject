// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FactionManager.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UFactionManager : public UObject
{
	GENERATED_BODY()
public:

	bool InitFactions();
	bool RegisterFaction(class UFaction* InFaction);
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TArray<UFaction*> Factions;

public:

	FORCEINLINE TArray<UFaction*>& GetFactions() {return Factions;}
};
