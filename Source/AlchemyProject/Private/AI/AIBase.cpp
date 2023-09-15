// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyProject/Public/AI/AIBase.h"

#include "NavigationInvokerComponent.h"
#include "AI/BaseAIController.h"
#include "AI/UI/SpeechWidget.h"
#include "AI/Utility/PatrolArea.h"
#include "AlchemyProject/AlchemyProjectGameMode.h"
#include "AlchemyProject/HealthComponent.h"
#include "AlchemyProject/InventoryComponent.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attributes/AlcAttributeSet.h"
#include "GAS/Components/AlcAbilitySystemComponent.h"
#include "GAS/GameplayAbility/AlcGameplayAbility.h"
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
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	AbilitySystemComponent = CreateDefaultSubobject<UAlcAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UAlcAttributeSet>(TEXT("Attributes"));

	TargetProxy = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetProxy"));
	TargetProxy->SetBoxExtent(FVector(30.f, 30.f, 88.f));
	TargetProxy->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TargetProxy->SetCollisionObjectType(ECC_EngineTraceChannel1);
	TargetProxy->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetProxy->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Block);
	TargetProxy->SetGenerateOverlapEvents(false);
	
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

float AAIBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	HealthComponent->SetHealth(FMath::Clamp(HealthComponent->GetHealth() - DamageAmount, 0, HealthComponent->GetMaxHealth()));
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

bool AAIBase::TraceForTargetProxy(const FVector& InLocation, FHitResult& OutHitResult)
{
	bool bSuccess = false;

	TargetProxy->SetWorldLocation(InLocation);
	
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Visibility);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByObjectType(HitResult, GetActorLocation(), InLocation, CollisionObjectQueryParams, CollisionQueryParams);
	
	if(HitResult.bBlockingHit)
	{
		OutHitResult = HitResult;
		bSuccess = true;
	}
	
	return bSuccess;
}

void AAIBase::MovementTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	UE_LOG(LogTemp, Warning, TEXT("TagChanged: %s"), *CallbackTag.GetTagName().ToString())
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
	if(SpeechWidgetComp == nullptr) return;
	
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
	//TODO: Touch/proximity sense handling
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

bool AAIBase::UseAbility()
{
	return true;
}

void AAIBase::HandleLostEnemy(AActor* Enemy)
{
	
}

void AAIBase::HandleEnemySeen(AActor* Enemy)
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
		SetAIState(EAIState::EAIS_InCombat);
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

ABaseAIController* AAIBase::GetBaseAIController() const
{
	return AIController;
}

void AAIBase::SetBeingCareful(const bool InBool)
{
	if(AIController == nullptr || AIController->GetAIBlackboardComponent() == nullptr) return;
	
	AIController->GetAIBlackboardComponent()->SetValueAsBool(FName("BeingCareful"), InBool);
	bBeingCareful = InBool;
	if(bBeingCareful) GetCharacterMovement()->MaxWalkSpeed = PatrolMoveSpeed;
	else GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
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

UHealthComponent* AAIBase::GetHealthComp()
{
	return HealthComponent;
}

UAbilitySystemComponent* AAIBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAIBase::InitializeAttributes()
{
	if(AbilitySystemComponent && DefaultAttributeEffects)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffects, 1, EffectContextHandle);

		if(SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AAIBase::GiveAbilities()
{
	if(HasAuthority() && AbilitySystemComponent)
	{
		for(TSubclassOf<UAlcGameplayAbility>& Ability : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(*Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void AAIBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	check(AbilitySystemComponent != nullptr)

	/**	NOTE: This is the server implementation, but the current project is not ment to be multiplayer.
	 *	Just following a tutorial */
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveAbilities();
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