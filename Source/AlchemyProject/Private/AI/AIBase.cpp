// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyProject/Public/AI/AIBase.h"

#include "AI/BaseAIController.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

AAIBase::AAIBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	
	
	
}

void AAIBase::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIBase::OnSeenPawn);

	AIController = Cast<ABaseAIController>(GetController());
	
	if(AIController)
	{
		AIController->RunBehaviorTree(BehaviorTree);
		
		if(AIController->GetAIBlackboardComponent() == nullptr) return;
		
		AIController->GetAIBlackboardComponent()->SetValueAsEnum(FName("AIState"), (uint8)EAIState::EAIS_Idle);
		
	}
}

void AAIBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIBase::OnSeenPawn(APawn* InPawn)
{
	if(Cast<APlayerCharacter>(InPawn))
	{
		if(!bPlayerSeen) UE_LOG(LogTemp, Warning, TEXT("Hello There"))
		bPlayerSeen = true;
		
		if(AIController == nullptr || AIController->GetAIBlackboardComponent() == nullptr) return;
		AIController->GetAIBlackboardComponent()->SetValueAsBool(FName("PlayerSeen"), bPlayerSeen);
		
		GetWorldTimerManager().ClearTimer(PlayerSeenTimer);
		GetWorldTimerManager().SetTimer(PlayerSeenTimer,this, &ThisClass::ResetPlayerSeen, PlayerSeenTimerTime);
	}
}

void AAIBase::ResetPlayerSeen()
{
	bPlayerSeen = false;
	
	if(AIController == nullptr || AIController->GetAIBlackboardComponent() == nullptr) return;
	AIController->GetAIBlackboardComponent()->SetValueAsBool(FName("PlayerSeen"), bPlayerSeen);
}

void AAIBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIBase::SetAIState(EAIState NewState)
{
	AIState = NewState;

	if(AIController == nullptr || AIController->GetAIBlackboardComponent() == nullptr) return;

	AIController->GetAIBlackboardComponent()->SetValueAsEnum(FName("AIState"), (uint8)AIState);
}