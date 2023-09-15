// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <bitset>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldState.generated.h"

class AAgentBase;
class UBlackboardComponent;

//Used for determining the type of world state effect an action can have
enum class EWorldStateEffect : uint8
{
	EWSE_AtLocation,
	EWSE_ObjectEquipped,
	EWSE_ObjectInInventory,
	EWSE_UsingAnObject,
	EWSE_Idling,
	EWSE_PlayingAnimation,

	EWSE_MAX
};

//Used to indicate how to interpret the union bits
enum class EWorldStateType : uint8
{
	EWST_Object,
	EWST_Location,
	EWST_Integer,
	EWST_Boolean,

	EWST_MAX
};

inline constexpr static uint8 WSEffectNum = static_cast<uint8>(EWorldStateEffect::EWSE_MAX);

USTRUCT()
struct FWorldStateProperty
{
	GENERATED_BODY()

	FWorldStateProperty()
	{
		PropertyID = GetUniquePropertyID();
		WSEffect = EWorldStateEffect::EWSE_MAX;
		WSType = EWorldStateType::EWST_MAX;
		WS_IntValue = 0;
	}

	//Maybe not necessary
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
		WSEffect = OtherProp.WSEffect;
		WSType = OtherProp.WSType;
		WS_Object = OtherProp.WS_Object;
		
		return *this;
	}
	
	uint32 PropertyID = 0;
	EWorldStateEffect WSEffect;
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

typedef std::bitset< EWorldStateEffect::EWSE_MAX > FEffectFlags;

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

	
	
	uint32 GetNumWorldStateDifferences(UWorldState* OtherWorldState);
	uint32 GetNumUnsatisfiedWorldStateProps(UWorldState* OtherWorldState);

	FWorldStateProperty* GetWSProp(const uint8 Flag);
	FWorldStateProperty* GetWSProp(const EWorldStateEffect Effect);

protected:

	FEffectFlags EffectFlags;
	TArray<FWorldStateProperty> WSPropList;
};
