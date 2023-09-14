#pragma once
#include "Engine/DataTable.h"
#include "Engine/UserDefinedStruct.h"

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
struct FNPCInfo : public FTableRowBase
{
	GENERATED_BODY()

	FNPCInfo();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NPC_ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> JoinedFactionIDs;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FirstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LastName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FullName;

	static bool FillData(FNPCInfo& InNPCInfo, const int32 ID);

	
};

inline FNPCInfo::FNPCInfo()
{
	NPC_ID = 0;
	Name = NAME_None;
	FirstName = NAME_None;
	LastName = NAME_None;
	FullName = NAME_None;
}

inline bool FNPCInfo::FillData(FNPCInfo& InNPCInfo, const int32 ID)
{
	const FString NPCInfoTablePath(TEXT("DataTable'/Game/Assets/Datatables/NPCInfoDataTable.NPCInfoDataTable'"));
	const UDataTable* NPCInfoTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *NPCInfoTablePath));
	if(NPCInfoTableObject)
	{
		TArray<FName> RowNames = NPCInfoTableObject->GetRowNames();
		for(const FName& RowName : RowNames)
		{
			const FNPCInfo* NPCRow = NPCInfoTableObject->FindRow<FNPCInfo>(RowName, "");
			if(NPCRow)
			{
				if(NPCRow->NPC_ID == ID)
				{
					InNPCInfo = *NPCRow;
					return true;
				}
			}
		}
	}
	return false;
}


USTRUCT(BlueprintType)
struct FRoadConnectionContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<class ARoadSpline>> RoadSplines;
	
};

//Stores all the roads in the game
USTRUCT(BlueprintType)
struct FRoadInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<ARoadSpline> RoadSpline;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRoadConnectionContainer RoadConnections;

	bool operator==(const FRoadInfo& Other) const
	{
		return RoadSpline == Other.RoadSpline;
	}

	bool operator!=(const FRoadInfo& Other) const
	{
		return RoadSpline != Other.RoadSpline;
	}
};

FORCEINLINE uint32 GetTypeHash(const FRoadInfo& Info)
{
	const uint32 Hash = UUserDefinedStruct::GetUserDefinedStructTypeHash(&Info, FRoadInfo::StaticStruct());
	return Hash;
}

//Used for road connections
USTRUCT(BlueprintType)
struct FRoadInfoContainer
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRoadInfo> RoadInfos;

	
};

/*
USTRUCT(BlueprintType)
struct FTagsToSearch
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer SearchTagsContainer;
	
};
*/
