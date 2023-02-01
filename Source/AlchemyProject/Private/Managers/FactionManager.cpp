// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/FactionManager.h"

#include "Utility/Faction.h"

bool UFactionManager::InitFactions()
{
	TArray<FName> RowNames;
	const FString FactionTablePath(TEXT("DataTable'/Game/Assets/Datatables/FactionInfoTable.FactionInfoTable'"));
	const UDataTable* FactionTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *FactionTablePath));
	if(FactionTableObject)
	{
		RowNames = FactionTableObject->GetRowNames();
		for(const FName& RowName : RowNames)
		{
			const FFactionInfo* Row = FactionTableObject->FindRow<FFactionInfo>(RowName, TEXT(""));
			if(Row)
			{
				UFaction* Faction = NewObject<UFaction>();
				Faction->GetFactionInfo().FactionID = Row->FactionID;
				Faction->GetFactionInfo().FactionName = Row->FactionName;
				Faction->GetFactionInfo().FactionOpinions = Row->FactionOpinions;
				Faction->GetFactionInfo().FriendlyFactions = Row->FriendlyFactions;
				Faction->GetFactionInfo().HostileFactions = Row->HostileFactions;
				Faction->GetFactionInfo().NeutralFactions = Row->NeutralFactions;
				Faction->GetFactionInfo().MemberIDs = Row->MemberIDs;
				Faction->GetFactionInfo().MutuallyExclusiveFactions = Row->MutuallyExclusiveFactions;
				Factions.AddUnique(Faction);
			}
		}
	}
	else return false;
	
	return true;
}

bool UFactionManager::RegisterFaction(UFaction* InFaction)
{
	InFaction->GetFactionInfo().FactionID = Factions.Last()->GetFactionInfo().FactionID + 1;
	Factions.AddUnique(InFaction);
	return true;
}
