// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AlchemyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMYPROJECT_API UAlchemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	friend class APlayerCharacter;
	UAlchemyComponent();

protected:
	
	virtual void BeginPlay() override;

private:
	
	

		
};
