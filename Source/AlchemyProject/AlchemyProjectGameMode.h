// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Managers/NavigationManager.h"
#include "AlchemyProjectGameMode.generated.h"

class UFactionManager;
class UCombatManager;
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
	
	UFUNCTION(BlueprintCallable)
	UCombatManager* GetCombatManager() const;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UCombatManager* CombatManager;

public:


};



