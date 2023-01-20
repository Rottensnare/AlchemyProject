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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 NumberOfMembers{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AAIBase*> MemberArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FFactionOpinion> FactionOpinions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FNPCInfo> MemberInfos;
};

USTRUCT(BlueprintType)
struct FBehaviorTreeTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;
	
};
