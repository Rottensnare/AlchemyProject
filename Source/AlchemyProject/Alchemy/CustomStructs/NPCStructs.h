#pragma once

#include "NPCStructs.generated.h"

USTRUCT(BlueprintType)
struct FFactionOpinion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 FactionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Opinion{0};
};

USTRUCT(BlueprintType)
struct FNPCInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NPC_ID;
	
	//Which faction does the NPC belong to
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 FactionID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FirstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LastName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FullName;
};
