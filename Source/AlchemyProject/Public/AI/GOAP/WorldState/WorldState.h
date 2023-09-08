// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldState.generated.h"

class UBlackboardComponent;

USTRUCT()
struct FFact
{
	GENERATED_BODY()

	
};

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UWorldState : public UObject
{
	GENERATED_BODY()

public:

	UWorldState();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World State")
	TObjectPtr<UBlackboardComponent> WorldBlackboard;
};
