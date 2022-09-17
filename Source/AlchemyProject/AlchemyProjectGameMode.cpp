// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlchemyProjectGameMode.h"
#include "AlchemyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAlchemyProjectGameMode::AAlchemyProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
