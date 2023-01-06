// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABaseAIController();
	virtual void OnPossess(APawn* InPawn) override;

protected:

	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

private:

public:

	FORCEINLINE UBlackboardComponent* GetAIBlackboardComponent() const {return BlackboardComponent;}
	FORCEINLINE UBehaviorTreeComponent* GetAIBehaviorTreeComponent() const {return BehaviorTreeComponent;}
	
};
