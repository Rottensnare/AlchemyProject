#pragma once
#include "AlchemyProject/Alchemy/CustomStructs/NPCStructs.h"
#include "Engine/DataTable.h"

#include "CustomDataTables.generated.h"

USTRUCT(BlueprintType)
struct FSoundEffectTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class USoundCue* SFX;
	
};

USTRUCT(BlueprintType)
struct FFactionInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 FactionID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName FactionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> MemberIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FFactionOpinion> FactionOpinions;

	//Player and NPCs can be part of multiple factions
	//This is used to make sure that they can't join factions that are mutually exclusive
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> MutuallyExclusiveFactions;

	//These are used for AI Perception.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> HostileFactions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> NeutralFactions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> FriendlyFactions;
	
};

USTRUCT(BlueprintType)
struct FBehaviorTreeTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;
	
};
