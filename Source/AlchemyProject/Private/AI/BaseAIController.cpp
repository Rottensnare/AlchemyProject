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
	TeamAttitudeMap_Sight.Emplace(ETeamAttitude::Hostile, true);
	TeamAttitudeMap_Sight.Emplace(ETeamAttitude::Friendly, false);
	TeamAttitudeMap_Sight.Emplace(ETeamAttitude::Neutral, true);
	SenseConfig_Sight->SightRadius = MaxSightRadius;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngle;
	//SenseConfig_Sight->DescribeSelfToGameplayDebugger()
	
	SenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SenseConfig_Hearing"));
	SenseConfig_Hearing->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Hearing->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig_Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	TeamAttitudeMap_Hearing.Emplace(ETeamAttitude::Hostile, true);
	TeamAttitudeMap_Hearing.Emplace(ETeamAttitude::Friendly, false);
	TeamAttitudeMap_Hearing.Emplace(ETeamAttitude::Neutral, true);

	AIPerceptionComponent->ConfigureSense(*SenseConfig_Sight);
	AIPerceptionComponent->ConfigureSense(*SenseConfig_Hearing);
	AIPerceptionComponent->SetDominantSense(SenseConfig_Sight->StaticClass());
	
	ABaseAIController::SetGenericTeamId(FGenericTeamId(1));
}

void ABaseAIController::BeginPlay()
{
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetPerceptionUpdated_Delegate);
	AIBase = Cast<AAIBase>(GetPawn());
	Super::BeginPlay();
}

void ABaseAIController::Tick(float DeltaSeconds)
{
	CheckStimulusTimer += DeltaSeconds;
	if(CheckStimulusTimer >= 2.f)
	{
		CheckStimulusTimer = 0.f;
		if(AIPerceptionComponent == nullptr || BlackboardComponent == nullptr) return;
		const bool bHasActiveStimulus = AIPerceptionComponent->HasActiveStimulus(*Cast<AActor>(BlackboardComponent->GetValueAsObject(FName("Target"))), AIPerceptionComponent->GetDominantSenseID());
		if(!bHasActiveStimulus)
		{
			//UE_LOG(LogTemp, Warning, TEXT("HasActiveStimulus: %d"), bHasActiveStimulus)
			//BlackboardComponent->SetValueAsObject(FName("Target"), nullptr);
			//BlackboardComponent->SetValueAsBool(FName("PlayerSeen"), false);
		}
	}
	
	Super::Tick(DeltaSeconds);
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
			if(TeamAgent->GetGenericTeamId() == FGenericTeamId(0) || TeamAgent->GetGenericTeamId() == FGenericTeamId(1)) //TODO: Need to remove this hardcoded stuff later
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
	if(InActor == nullptr || BlackboardComponent == nullptr) return;
	switch (Stimulus.Type)
	{
	case 0:
		//Sight
		if(ETeamAttitude::Hostile == GetTeamAttitudeTowards(*InActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy sighted!"))
			AIBase->ToggleSpeechWidget("Enemy Sighted");
			BlackboardComponent->SetValueAsBool(FName("PlayerSeen"), true);
			if(AIBase) AIBase->SetPlayerSeen(true);
			BlackboardComponent->SetValueAsObject(FName("Target"), InActor);
			BlackboardComponent->SetValueAsVector(FName("LastTargetLocation"), InActor->GetActorLocation());
			BlackboardComponent->ClearValue(FName("PointOfInterest"));
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
		if(!(ETeamAttitude::Friendly == GetTeamAttitudeTowards(*InActor)))
		{
			UE_LOG(LogTemp, Warning, TEXT("Heard sum ting"))
			AIBase->ToggleSpeechWidget("Herd sum ting");
			BlackboardComponent->SetValueAsVector(FName("PointOfInterest"), Stimulus.StimulusLocation);
		}
		break;
	default:
		//Do the default thing
		break;
	}
}

void ABaseAIController::ChangeAttitudeTowards()
{
	//UE_LOG(LogTemp, Warning, TEXT("TeamAttitudeMap_Sight: %d"), TeamAttitudeMap_Sight[ETeamAttitude::Hostile])
	
	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = TeamAttitudeMap_Sight[ETeamAttitude::Hostile];
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = TeamAttitudeMap_Sight[ETeamAttitude::Friendly];
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = TeamAttitudeMap_Sight[ETeamAttitude::Neutral];
	if(AIPerceptionComponent) AIPerceptionComponent->ConfigureSense(*SenseConfig_Sight);
	
	SenseConfig_Hearing->DetectionByAffiliation.bDetectEnemies = TeamAttitudeMap_Hearing[ETeamAttitude::Hostile];
	SenseConfig_Hearing->DetectionByAffiliation.bDetectFriendlies = TeamAttitudeMap_Hearing[ETeamAttitude::Friendly];
	SenseConfig_Hearing->DetectionByAffiliation.bDetectNeutrals = TeamAttitudeMap_Hearing[ETeamAttitude::Neutral];
	if(AIPerceptionComponent) AIPerceptionComponent->ConfigureSense(*SenseConfig_Hearing);
}

void ABaseAIController::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NAME_TeamAttitudeMap_Sight = FName("TeamAttitudeMap_Sight");
	static const FName NAME_TeamAttitudeMap_Hearing = FName("TeamAttitudeMap_Hearing");

	if(PropertyChangedEvent.Property)
	{
		if(PropertyChangedEvent.Property->GetFName() == NAME_TeamAttitudeMap_Sight)
		{
			ChangeAttitudeTowards();
		}
		else if(PropertyChangedEvent.Property->GetFName() == NAME_TeamAttitudeMap_Hearing)
		{
			ChangeAttitudeTowards();
		}
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}


