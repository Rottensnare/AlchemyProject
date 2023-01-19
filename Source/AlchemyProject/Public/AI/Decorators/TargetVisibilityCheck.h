// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "TargetVisibilityCheck.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ALCHEMYPROJECT_API UTargetVisibilityCheck : public UBTDecorator_BlueprintBase
{
	GENERATED_BODY()
	
	UTargetVisibilityCheck();

protected:

	UFUNCTION(BlueprintCallable)
	virtual bool CheckVisibility(AAIController* AIController, APawn* InPawn, float DeltaSeconds);

private:

	void SetBlackboardValueToFalse(class ABaseAIController* InController);
	
};
