// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProject/Alchemy/CustomStructs/NPCStructs.h"
#include "UObject/NoExportTypes.h"
#include "CharacterData.generated.h"

USTRUCT()
struct FMyStruct
{
	GENERATED_BODY()

	
};

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UCharacterData : public UObject
{
	GENERATED_BODY()

public:


private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FNPCInfo NPCInfo;
	
public:

	FORCEINLINE FNPCInfo& GetJoinedFactions() {return NPCInfo;}

};
