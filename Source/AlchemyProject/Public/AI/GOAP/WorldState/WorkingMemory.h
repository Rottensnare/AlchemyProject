// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorkingMemory.generated.h"


UCLASS()
class UWMFact : public UObject
{
	GENERATED_BODY()
public:

	UWMFact();

	/**	Should be used to assign an ID, not just reading. Use PeekNextUniqueID just for reading. */
	static unsigned long long GetNextUniqueID()
	{
		return ++UniqueFactID;
	}

	/**	Only for reading. Don't use this to assign a unique ID */
	static unsigned long long PeekNextUniqueID()
	{
		return UniqueFactID;
	}

	FORCEINLINE void SetTime(const double InTime) {Time = InTime;}
	FORCEINLINE double GetTime() const {return Time;}
	
protected:


private:
	inline static unsigned long long UniqueFactID = 0;
	
	double Time = 0;
};

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UWorkingMemory : public UObject
{
	GENERATED_BODY()

	
};
