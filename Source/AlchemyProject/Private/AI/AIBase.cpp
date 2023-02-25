// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyProject/Public/AI/AIBase.h"

#include "NavigationInvokerComponent.h"
#include "AI/BaseAIController.h"
#include "AI/UI/SpeechWidget.h"
#include "AI/Utility/PatrolArea.h"
#include "AlchemyProject/AlchemyProjectGameMode.h"
#include "AlchemyProject/InventoryComponent.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/FactionManager.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Utility/Faction.h"

AAIBase::AAIBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSourceComp"));
	PerceptionStimuliSourceComponent->bAutoRegister = true;

	NavigationInvokerComponent = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvokerComp"));

	SpeechWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("SpeechWidgetComp"));
	SpeechWidgetComp->SetupAttachment(GetRootComponent());
	SpeechWidgetComp->SetVisibility(false);

	ESPSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ESPSphere"));
	ESPSphere->SetupAttachment(GetRootComponent());
	ESPSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ESPSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ESPSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	//CharacterData = CreateDefaultSubobject<UCharacterData>(TEXT("CharacterData"));
	
}

void AAIBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if(GetMesh())
	{
		OutLocation = GetMesh()->GetSocketLocation("leftEyeSocket");
		OutRotation = GetMesh()->GetSocketRotation("leftEyeSocket");
	}
}

void AAIBase::BeginPlay()
{
	Super::BeginPlay();

	//PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIBase::OnSeenPawn);
	//PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIBase::OnSomethingHeard);

	ESPSphere->OnComponentBeginOverlap.AddDynamic(this, &AAIBase::OnESPBeginOverlap);
	ESPSphere->OnComponentEndOverlap.AddDynamic(this, &AAIBase::OnESPEndOverlap);

	if(GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = PatrolMoveSpeed;
	OriginalPosition = GetActorLocation();
	
	AIController = Cast<ABaseAIController>(GetController());
	if(AIController)
	{
		//BUG: Use BehaviorTreeComponent->StartTree Instead
		//AIController->RunBehaviorTree(BehaviorTree); //BUG: When using RunBehaviorTree in BeginPlay it won't add the instance to the UBehaviorTreeComponent::InstanceStack 
		
		if(AIController->GetAIBlackboardComponent() == nullptr) return;
		
		AIController->GetAIBlackboardComponent()->SetValueAsEnum(FName("AIState"), static_cast<uint8>(AIState));
		AIController->GetAIBlackboardComponent()->SetValueAsBool(FName("FollowPlayer"), bFollowPlayer);
		AIController->GetAIBlackboardComponent()->SetValueAsVector(FName("OriginalPosition"), OriginalPosition);

		if(AIState != EAIState::EAIS_Alerted && AIState != EAIState::EAIS_Chasing && AIState != EAIState::EAIS_InCombat)
		{
			LastAIState = AIState;
		}
		else LastAIState = EAIState::EAIS_Idle;
		
	}

	IQueryable::InitializeGameplayTagContainer(GameplayTagContainer);
	FNPCInfo::FillData(NPCInfo, NPC_ID);
}

void AAIBase::PostInitializeComponents()
{
	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Hearing::StaticClass());
	PerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
	
	Super::PostInitializeComponents();
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
		// Original position is not currently used and should be reworked. This is not the intended use.
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

ETeamAttitude::Type AAIBase::GetFactionAttitude(const FNPCInfo& DetectedNPCInfo) const
{
	AAlchemyProjectGameMode* AlchemyGameMode = Cast<AAlchemyProjectGameMode>(UGameplayStatics::GetGameMode(this));
	if(AlchemyGameMode)
	{
		for(UFaction* const TempFaction : AlchemyGameMode->FactionManager->GetFactions())
		{
			if(!(&TempFaction->GetFactionInfo()))
			{
				continue;
			}
			if(TempFaction->GetFactionInfo().MemberIDs.Contains(NPCInfo.NPC_ID))
			{
				//First check if are part of any hostile factions, return Hostile if true.
				//This is first because it is higher priority than friendly factions.
				//TODO: Might need to create a system where it is not allowed to be part of 2 different factions that are hostile at each other
				//TODO: This isn't an easy task, needs a lot of thought put into it, since player can be part of 2 factions that become hostile \
				// after the player has joined them.
				for(const int32 FactionID : DetectedNPCInfo.JoinedFactionIDs)
				{
					if(TempFaction->GetFactionInfo().HostileFactions.Contains(FactionID)) return ETeamAttitude::Hostile;
				}
				for(const int32 FactionID : DetectedNPCInfo.JoinedFactionIDs)
				{
					//if(NPCInfo.JoinedFactionIDs.IsValidIndex(0)) UE_LOG(LogTemp, Warning, TEXT("Target FactionID: %d, Perceiver FactionID: %d"), FactionID, NPCInfo.JoinedFactionIDs[0])
					//else  UE_LOG(LogTemp, Warning, TEXT("NPCInfo.JoinedFactionIDs.IsValidIndex(0) NOT A VALID INDEX"))
					
					//If part of a friendly faction, return Friendly
					if(TempFaction->GetFactionInfo().FriendlyFactions.Contains(FactionID)) return ETeamAttitude::Friendly;
					
					//If are part of the same faction, return Friendly
					if(NPCInfo.JoinedFactionIDs.Contains(FactionID)) return ETeamAttitude::Friendly;
				}
			}
		}
	}
	return ETeamAttitude::Neutral;
	
}

void AAIBase::DestinationReached()
{
	NavDestination = nullptr;
	bRoadsFound = false;
	CurrentRoad = nullptr;
	CurrentRoadIndex = -1;
	CurrentRoadName = NAME_None;
	RoadNames.Empty();
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

void AAIBase::OnESPBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bESPOverlapping = true;
}

void AAIBase::OnESPEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bESPOverlapping = false;
}

void AAIBase::CalculateAbility()
{
	
}

void AAIBase::Attack()
{
	
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

void AAIBase::GetGameMode()
{
	AlchemyProjectGameMode = Cast<AAlchemyProjectGameMode>(GetWorld()->GetAuthGameMode());
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
		SetAIState(GetLastAIState());
		GetCharacterMovement()->MaxWalkSpeed = PatrolMoveSpeed;
	}
}

void AAIBase::SetCurrentRoad(ARoadSpline* InRoadSpline)
{
	CurrentRoad = InRoadSpline;
	CurrentRoadIndex++;
	if(GetRoadNames().IsValidIndex(CurrentRoadIndex)) CurrentRoadName = GetRoadNames()[CurrentRoadIndex];
}

bool AAIBase::Interact(AActor* OtherActor)
{
	if(!bCanBeInteractedWith) return false;
	if(bCanConverse)
	{
		return true;
	}
	
	return IInteractable::Interact(OtherActor);
}

FNPCInfo& AAIBase::GetNPCInfo()
{
	return NPCInfo;
}

FGameplayTagContainer& AAIBase::GetGameplayTagContainer()
{
	return GameplayTagContainer;
}

EPhysicalSurface AAIBase::GetFootStepSurfaceType()
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(this);
	const FVector Start{GetActorLocation()};
	const FVector End{Start + FVector(0.f, 0.f, -400.f)};
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, QueryParams);
	return UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
}

void AAIBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIBase::SetAIState(EAIState NewState)
{
	//UE_LOG(LogTemp, Warning, TEXT("SetAIState"))
	if(NewState != EAIState::EAIS_Alerted && NewState != EAIState::EAIS_Chasing && NewState != EAIState::EAIS_InCombat)
	{
		LastAIState = NewState;
	}
	
	AIState = NewState;

	if(AIState == EAIState::EAIS_Moving || AIState == EAIState::EAIS_Chasing)
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = PatrolMoveSpeed;
	}

	if(AIController == nullptr || AIController->GetAIBlackboardComponent() == nullptr) return;
	AIController->GetAIBlackboardComponent()->SetValueAsEnum(FName("AIState"), (uint8)AIState);
	
}