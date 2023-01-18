// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyProject/Public/AI/AIBase.h"

#include "AI/BaseAIController.h"
#include "AI/UI/SpeechWidget.h"
#include "AI/Utility/PatrolArea.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/RichTextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

AAIBase::AAIBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSourceComp"));
	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Hearing::StaticClass());

	SpeechWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("SpeechWidgetComp"));
	SpeechWidgetComp->SetupAttachment(GetRootComponent());
	SpeechWidgetComp->SetVisibility(false);
	//SpeechWidgetComp->SetWidgetClass(SpeechWidgetClass);

	//GameplayTagContainer.AddTag(GameplayTag);
}

void AAIBase::BeginPlay()
{
	Super::BeginPlay();

	//PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIBase::OnSeenPawn);
	//PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIBase::OnSomethingHeard);

	if(GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = PatrolMoveSpeed;
	OriginalPosition = GetActorLocation();
	
	AIController = Cast<ABaseAIController>(GetController());
	if(AIController)
	{
		AIController->RunBehaviorTree(BehaviorTree);
		
		if(AIController->GetAIBlackboardComponent() == nullptr) return;
		
		AIController->GetAIBlackboardComponent()->SetValueAsEnum(FName("AIState"), (uint8)AIState);
		AIController->GetAIBlackboardComponent()->SetValueAsBool(FName("FollowPlayer"), bFollowPlayer);
		AIController->GetAIBlackboardComponent()->SetValueAsVector(FName("OriginalPosition"), OriginalPosition);
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
		SetPlayerSeen(true);
		
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

bool AAIBase::FindNextPatrolPoint()
{
	if(PatrolArea && AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(FName("OriginalPosition"), GetActorLocation());
		PointOfInterest = PatrolArea->GetRandomPatrolCoordinates();
		if(PointOfInterest != FVector())
		{
			AIController->GetBlackboardComponent()->SetValueAsVector(FName("PointOfInterest"), PointOfInterest);
			return true;
		}
	}
	return false;
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

void AAIBase::ToggleSpeechWidget(const FString InString)
{
	ClearSpeechWidgetTimer();
	if(InString == FString() || InString == FString(""))
	{
		HideSpeechWidget();
	}
	else
	{
		SpeechWidgetComp->SetVisibility(true);
		SetSpeechWidgetTimer();
	}
	
	Cast<USpeechWidget>(SpeechWidgetComp->GetWidget())->SetBlockTextEvent(InString);
}

void AAIBase::SetSpeechWidgetTimer()
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("SetSpeechWidgetTimer"))
	const FString TempString = FString();
	FTimerDelegate SpeechTimerDelegate;
	SpeechTimerDelegate.BindUFunction(this, FName("ToggleSpeechWidget"), TempString);
	GetWorldTimerManager().SetTimer(PlayerSeenTimer,SpeechTimerDelegate, PlayerSeenTimerTime, false);
	*/
	
	GetWorldTimerManager().SetTimer(SpeechWidgetTimer, this, &AAIBase::HideSpeechWidget, SpeechWidgetShowTime);
}

void AAIBase::ClearSpeechWidgetTimer()
{
	GetWorldTimerManager().ClearTimer(SpeechWidgetTimer);
}

void AAIBase::HideSpeechWidget()
{
	SpeechWidgetComp->SetVisibility(false);
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
	SetPlayerSeen(false);
	
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

void AAIBase::SetPlayerSeen(const bool bValue)
{
	bPlayerSeen = bValue;
	
	if(bPlayerSeen)
	{
		SetAIState(EAIState::EAIS_Chasing);
		GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
	}
	else
	{
		SetAIState(EAIState::EAIS_Patrolling);
		GetCharacterMovement()->MaxWalkSpeed = PatrolMoveSpeed;
	}
}

void AAIBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIBase::SetAIState(EAIState NewState)
{
	LastAIState = AIState;
	AIState = NewState;

	if(AIController == nullptr || AIController->GetAIBlackboardComponent() == nullptr) return;
	AIController->GetAIBlackboardComponent()->SetValueAsEnum(FName("AIState"), (uint8)AIState);
	
}