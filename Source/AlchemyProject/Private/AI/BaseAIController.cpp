// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"

#include "AI/AIBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ABaseAIController::ABaseAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	checkf(BlackboardComponent, TEXT("BlackboardComponent was null in the constructor"));
	
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	checkf(BehaviorTreeComponent, TEXT("BehaviorTreeComponent was null in the constructor"));
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(InPawn == nullptr) return;
	
	if(AAIBase* Enemy = Cast<AAIBase>(InPawn))
	{
		if(Enemy->GetBehaviorTree())
		{
			BlackboardComponent->InitializeBlackboard(*(Enemy->GetBehaviorTree()->GetBlackboardAsset()));
		}
	}
}
