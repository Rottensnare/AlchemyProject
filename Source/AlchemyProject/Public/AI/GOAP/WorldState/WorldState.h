// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldState.generated.h"

class AAgentBase;
class UBlackboardComponent;

enum class EWorldStateType : uint8
{
	EWST_MAX,

	EWST_Object,
	EWST_Location,
	EWST_Integer,
	EWST_Boolean
};

USTRUCT()
struct FWorldStateProperty
{
	GENERATED_BODY()

	FWorldStateProperty()
	{
		PropertyID = GetUniquePropertyID();
		WSType = EWorldStateType::EWST_MAX;
		WS_IntValue = 0;
	}

	inline static uint32 UniquePropertyID = 0;

	uint32 GetUniquePropertyID()
	{
		return ++UniquePropertyID;
	}
	
	uint32 PropertyID = 0;
	EWorldStateType WSType;
	union
	{
		TObjectPtr<UObject> WS_Object;
		FVector				WS_Location;
		int32				WS_IntValue;
		bool				WS_Boolean;
	};
};

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

	TArray<FWorldStateProperty> PropList;
	
	uint32 GetNumWorldStateDifferences(UWorldState* OtherWorldState);
	uint32 GetNumUnsatisfiedWorldStateProps(UWorldState* OtherWorldState);
};
