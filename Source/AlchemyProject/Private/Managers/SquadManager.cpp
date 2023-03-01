// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SquadManager.h"

#include "AI/AIBase.h"
#include "AlchemyProject/AlchemyProjectGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/CombatManager.h"

void USquadManager::AddToSquad(AAIBase* AIToAdd)
{
	if(SquadMembers.Contains(AIToAdd))
	{
		UE_LOG(LogTemp, Warning, TEXT("AI already is part of the squad"))
		return;
	}
	if(AIToAdd == nullptr) return;

	//Removing the AI from a squad if it's in one //NOTE: might just be better to check if SquadID != -1
	const AAlchemyProjectGameMode* const APGM = Cast<AAlchemyProjectGameMode>(UGameplayStatics::GetGameMode(AIToAdd));
	if(APGM)
	{
		for(USquadManager* const Squad : APGM->GetCombatManager()->Squads)
		{
			if(Squad->SquadMembers.Contains(AIToAdd))
			{
				Squad->SquadMembers.Remove(AIToAdd);
			}
		}
	}
	
	SquadMembers.AddUnique(AIToAdd);
	AIToAdd->SetSquadID(Squad_ID);
}

void USquadManager::RemoveFromSquad(AAIBase* AIToRemove)
{
	const AAlchemyProjectGameMode* const APGM = Cast<AAlchemyProjectGameMode>(UGameplayStatics::GetGameMode(AIToRemove));
	if(APGM)
	{
		for(USquadManager* const Squad : APGM->GetCombatManager()->Squads)
		{
			if(Squad->SquadMembers.Contains(AIToRemove))
			{
				Squad->SquadMembers.Remove(AIToRemove);
				AIToRemove->SetSquadID(-1);
			}
		}
	}
}

