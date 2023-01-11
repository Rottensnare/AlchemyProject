// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"

#include "AI/AIBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ABaseAIController::ABaseAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	checkf(BlackboardComponent, TEXT("BlackboardComponent was null in the constructor"));
	
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	checkf(BehaviorTreeComponent, TEXT("BehaviorTreeComponent was null in the constructor"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));
	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	
	SenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SenseConfig_Hearing"));
	SenseConfig_Hearing->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Hearing->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig_Hearing->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*SenseConfig_Sight);
	AIPerceptionComponent->ConfigureSense(*SenseConfig_Hearing);
	AIPerceptionComponent->SetDominantSense(SenseConfig_Sight->StaticClass());
	
	ABaseAIController::SetGenericTeamId(FGenericTeamId(1));
}

void ABaseAIController::BeginPlay()
{
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetPerceptionUpdated_Delegate);
	
	Super::BeginPlay();
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

ETeamAttitude::Type ABaseAIController::GetTeamAttitudeTowards(const AActor& Other) const
{

	if(APawn const* OtherPawn = Cast<APawn>(&Other))
	{
		if(auto const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if(TeamAgent->GetGenericTeamId() == FGenericTeamId(0) || TeamAgent->GetGenericTeamId() == FGenericTeamId(1))
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}
	}
	
	return ETeamAttitude::Neutral;
}

void ABaseAIController::OnTargetPerceptionUpdated_Delegate(AActor* InActor, FAIStimulus Stimulus)
{
	switch (Stimulus.Type)
	{
	case 0:
		//Sight
		if(ETeamAttitude::Hostile == GetTeamAttitudeTowards(*InActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy sighted!"))
		}
		else if(ETeamAttitude::Friendly == GetTeamAttitudeTowards(*InActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Henlo fren."))
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("I have no strong feelings one way or the other."))
		}
		break;
	case 1:
		//Hearing
		break;
	default:
		//Do the default thing
		break;
	}
}


