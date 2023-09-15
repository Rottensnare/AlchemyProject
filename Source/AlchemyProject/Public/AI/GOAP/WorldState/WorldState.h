// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldState.generated.h"

class AAgentBase;
class UBlackboardComponent;

//Used to indicate how to interpret the union bits
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

	/**	Increments the UniquePropertyID and then returns it */
	static uint32 GetUniquePropertyID()
	{
		return ++UniquePropertyID;
	}

	/**	Returns current highest unique property ID without incrementing it */
	/**	NOTE: NEVER ASSIGN USING THIS FUNCTION */
	static uint32 PeekUniquePropertyID()
	{
		return UniquePropertyID;
	}

	//BUG: Possible bug due to TObjectPtr being 64-bits when in Editor
	FWorldStateProperty& operator=(const FWorldStateProperty& OtherProp)
	{
		PropertyID = OtherProp.PropertyID;
		WSType = OtherProp.WSType;
		WS_Object = OtherProp.WS_Object;
		
		return *this;
	}
	
	uint32 PropertyID = 0;
	
	EWorldStateType WSType;
	union
	{
		TObjectPtr<UObject> WS_Object;
		FVector_NetQuantize	WS_Location;
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

	TArray<FWorldStateProperty> WSPropList;
	
	uint32 GetNumWorldStateDifferences(UWorldState* OtherWorldState);
	uint32 GetNumUnsatisfiedWorldStateProps(UWorldState* OtherWorldState);
};
