// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlchemyProjectGameMode.h"
#include "AlchemyProjectCharacter.h"
#include "Managers/CombatManager.h"
#include "Managers/FactionManager.h"
#include "Managers/NavigationManager.h"
#include "UObject/ConstructorHelpers.h"

UFactionManager* AAlchemyProjectGameMode::FactionManager = nullptr;

AAlchemyProjectGameMode::AAlchemyProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

UFactionManager* AAlchemyProjectGameMode::GetFactionManager()
{
	return FactionManager;
}

UNavigationManager* AAlchemyProjectGameMode::GetNavigationManager()
{
	return NavigationManager;
}

UCombatManager* AAlchemyProjectGameMode::GetCombatManager() const
{
	return CombatManager;
}

void AAlchemyProjectGameMode::BeginPlay()
{
	if(!FactionManager)
	{
		FactionManager = NewObject<UFactionManager>();
		FactionManager->AddToRoot();
		FactionManager->InitFactions();
	}

	if(!NavigationManager)
	{
		NavigationManager = NewObject<UNavigationManager>();
		NavigationManager->AddToRoot();
		NavigationManager->InitRoads();
	}

	if(!CombatManager)
	{
		CombatManager = NewObject<UCombatManager>();
		CombatManager->AddToRoot();
	}

	Super::BeginPlay();
}


