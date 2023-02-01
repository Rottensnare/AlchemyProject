// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProject/Enums/CustomDataTables.h"
#include "UObject/NoExportTypes.h"
#include "Faction.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UFaction : public UObject
{
	GENERATED_BODY()

public:

private:
	
	FFactionInfo FactionInfo;
	
	
public:

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FFactionInfo& GetFactionInfo() {return FactionInfo;}
	
};
