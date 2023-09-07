// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AgentBase.generated.h"

UCLASS()
class ALCHEMYPROJECT_API AAgentBase : public ACharacter
{
	GENERATED_BODY()

public:

	friend class AGOAPAIController;

	AAgentBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

private:
	

public:	

	

};
