// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GOAPAIController.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API AGOAPAIController : public AAIController
{
	GENERATED_BODY()

public:
	
	
protected:
	
	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	
	
private:


public:
	
};
