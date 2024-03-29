// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Managers/NavigationManager.h"
#include "AlchemyProjectGameMode.generated.h"

class UFactionManager;
UCLASS(minimalapi)
class AAlchemyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAlchemyProjectGameMode();

	static UFactionManager* FactionManager;

	UFUNCTION(BlueprintCallable)
	static UFactionManager* GetFactionManager();

	UPROPERTY()
	UNavigationManager* NavigationManager;

	UFUNCTION(BlueprintCallable)
	UNavigationManager* GetNavigationManager();

protected:

	virtual void BeginPlay() override;
};



