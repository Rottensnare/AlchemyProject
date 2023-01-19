// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Loop.h"
#include "BTDecorator_LoopDynamic.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ALCHEMYPROJECT_API UBTDecorator_LoopDynamic : public UBTDecorator_Loop
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Decorator, EditAnywhere)
	FBlackboardKeySelector KeySelector;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	
protected:
	
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	
};
