// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyProject/Public/AI/AIBase.h"

#include "AI/BaseAIController.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/PawnSensingComponent.h"

AAIBase::AAIBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	
	
}

void AAIBase::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIBase::OnSeenPawn);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIBase::OnSomethingHeard);

	
	AIPerceptionComponent->ConfigureSense(*SenseConfig);

	AIController = Cast<ABaseAIController>(GetController());
	
	if(AIController)
	{
		AIController->RunBehaviorTree(BehaviorTree);
		
		if(AIController->GetAIBlackboardComponent() == nullptr) return;
		
		AIController->GetAIBlackboardComponent()->SetValueAsEnum(FName("AIState"), (uint8)EAIState::EAIS_Idle);
		AIController->GetAIBlackboardComponent()->SetValueAsBool(FName("FollowPlayer"), bFollowPlayer);
		
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
		AIController->GetAIBlackboardComponent()->SetValueAsObject(FName("Target"), InPawn);
		
		GetWorldTimerManager().ClearTimer(PlayerSeenTimer);
		GetWorldTimerManager().SetTimer(PlayerSeenTimer,this, &ThisClass::ResetPlayerSeen, PlayerSeenTimerTime);
	}
}

void AAIBase::OnSomethingHeard(APawn* InInstigator, const FVector& Location, float Volume)
{
	UE_LOG(LogTemp, Warning, TEXT("Volume: %f"), Volume)
	bSomethingHeard = true;
	PointOfInterest = Location;
	
	if(AIController == nullptr || AIController->GetBlackboardComponent() == nullptr) return;
	AIController->GetBlackboardComponent()->SetValueAsVector(FName("PointOfInterest"), PointOfInterest);
}

void AAIBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NAME_FollowPlayer = FName("bFollowPlayer");
	if(PropertyChangedEvent.Property)
	{
		if(PropertyChangedEvent.Property->GetFName() == NAME_FollowPlayer)
		{
			SetFollowPlayer(bFollowPlayer);
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void AAIBase::SetFollowPlayer(bool Value)
{
	bFollowPlayer = Value;
	if(AIController == nullptr || AIController->GetAIBlackboardComponent() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController was nullptr"))
	}
	else
	{
		AIController->GetAIBlackboardComponent()->SetValueAsBool(FName("FollowPlayer"), bFollowPlayer);
	}
}

void AAIBase::ResetPlayerSeen()
{
	bPlayerSeen = false;
	
	if(AIController == nullptr || AIController->GetAIBlackboardComponent() == nullptr) return;
	AIController->GetAIBlackboardComponent()->SetValueAsBool(FName("PlayerSeen"), bPlayerSeen);
	AIController->GetAIBlackboardComponent()->SetValueAsObject(FName("Target"), nullptr);
}

void AAIBase::AddToActorsOfInterest(AActor* InActor)
{
	if(InActor == nullptr || ActorsOfInterest.Contains(InActor)) return;

	ActorsOfInterest.Add(InActor);
}

void AAIBase::RemoveFromActorsOfInterest(AActor* InActor)
{
	if(InActor == nullptr || !ActorsOfInterest.Contains(InActor)) return;

	ActorsOfInterest.Remove(InActor);
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