// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlchemyProjectGameMode.h"
#include "AlchemyProjectCharacter.h"
#include "Managers/FactionManager.h"
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

void AAlchemyProjectGameMode::BeginPlay()
{
	if(!FactionManager)
	{
		FactionManager = NewObject<UFactionManager>();
		FactionManager->AddToRoot();
		FactionManager->InitFactions();
	}
	
	Super::BeginPlay();
}
