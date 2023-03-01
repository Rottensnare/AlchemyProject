// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/CombatManager.h"

#include "AI/AIBase.h"
#include "Managers/SquadManager.h"

void UCombatManager::ReportCombat(TObjectPtr<AAIBase> InActor)
{
	if(Squads.IsEmpty())
	{
		USquadManager* SquadManager = NewObject<USquadManager>(this, USquadManager::StaticClass());
		SquadManager->AddToSquad(InActor);
		Squads.AddUnique(SquadManager);
		return;
	}
	
	for(USquadManager* Squad : Squads)
	{
		for(int32 ID : Squad->SquadMembers[0]->GetNPCInfo().JoinedFactionIDs)
		{
			if(InActor->GetNPCInfo().JoinedFactionIDs.Contains(ID))
			{
				Squad->AddToSquad(InActor);
				return;
			}
		}
	}
}
