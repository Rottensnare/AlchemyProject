// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"

#include "MyAIPerceptionComponent.h"
#include "NavigationSystem.h"
#include "AI/AIBase.h"
#include "AlchemyProject/Enums/CustomDataTables.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryOption.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Prediction.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Prediction.h"

ABaseAIController::ABaseAIController(const FObjectInitializer& ObjectInitializer) //: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	checkf(BlackboardComponent, TEXT("BlackboardComponent was null in the constructor"));
	
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	checkf(BehaviorTreeComponent, TEXT("BehaviorTreeComponent was null in the constructor"));

	AIPerceptionComponent = CreateDefaultSubobject<UMyAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));
	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	TeamAttitudeMap_Sight.Emplace(ETeamAttitude::Hostile, true);
	TeamAttitudeMap_Sight.Emplace(ETeamAttitude::Friendly, true);
	TeamAttitudeMap_Sight.Emplace(ETeamAttitude::Neutral, true);
	SenseConfig_Sight->SightRadius = MaxSightRadius;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngle;
	SenseConfig_Sight->SetMaxAge(MaxAgeSight);
	//SenseConfig_Sight->DescribeSelfToGameplayDebugger()
	
	SenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SenseConfig_Hearing"));
	SenseConfig_Hearing->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Hearing->DetectionByAffiliation.bDetectFriendlies = true;
	SenseConfig_Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	SenseConfig_Hearing->SetMaxAge(MaxAgeHearing);
	TeamAttitudeMap_Hearing.Emplace(ETeamAttitude::Hostile, true);
	TeamAttitudeMap_Hearing.Emplace(ETeamAttitude::Friendly, true);
	TeamAttitudeMap_Hearing.Emplace(ETeamAttitude::Neutral, true);

	SenseConfig_Prediction = CreateDefaultSubobject<UAISenseConfig_Prediction>(TEXT("SenseConfig_Prediction"));
	SenseConfig_Prediction->SetMaxAge(30.f);

	AIPerceptionComponent->ConfigureSense(*SenseConfig_Sight);
	AIPerceptionComponent->ConfigureSense(*SenseConfig_Hearing);
	AIPerceptionComponent->ConfigureSense(*SenseConfig_Prediction);
	AIPerceptionComponent->SetDominantSense(SenseConfig_Sight->StaticClass());

	CustomAIContainer = CreateDefaultSubobject<UCustomAIContainer>(TEXT("CustomAIContainer"));
	
	
	ABaseAIController::SetGenericTeamId(FGenericTeamId(1));
}

void ABaseAIController::BeginPlay()
{
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetPerceptionUpdated_Delegate);
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnPerceptionUpdated_Delegate);
	AIPerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionInfoUpdated_Delegate);
	//AIPerceptionComponent->OnSightStimulusExpired.AddDynamic(this, &ABaseAIController::OnSightStimulusExpired_Delegate);
	//AIPerceptionComponent->OnHearingStimulusExpired.AddDynamic(this, &ABaseAIController::OnHearingStimulusExpired_Delegate);
	
	AIBase = Cast<AAIBase>(GetPawn());

	
	//if(GetAIBehaviorTreeComponent()) GetAIBehaviorTreeComponent()->SetDynamicSubtree(FGameplayTag::RequestGameplayTag(FName("Subtree.Work")), GetBehaviorTree("Work"));
	
	Super::BeginPlay();
}

void ABaseAIController::Tick(float DeltaSeconds)
{
	CheckStimulusTimer += DeltaSeconds; //TODO: CAN BE REMOVED. FOR DEBUGGING PURPOSES
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

TArray<FVector>& ABaseAIController::QueryForLocations(const UEnvQuery* const InEnvQuery, APawn* InPawn, EEnvQueryRunMode::Type QueryRunMode)
{
	QueryLocations.Empty();
	
	if(InEnvQuery == nullptr || InPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QueryForLocations: InEnvQuery or InPawn was nullptr"))
		return QueryLocations;
	}
	FEnvQueryRequest ActorsQueryRequest = FEnvQueryRequest(InEnvQuery, InPawn);
	const int32 ExecuteCode = ActorsQueryRequest.Execute(QueryRunMode, this, &ABaseAIController::HandleQueryRequest_Locations);
	//UE_LOG(LogTemp, Warning, TEXT("QueryForLocations: %d"), ExecuteCode)
	return QueryLocations;

	
}

void ABaseAIController::HandleQueryRequest_Locations(TSharedPtr<FEnvQueryResult> Result)
{
	//UE_LOG(LogTemp, Warning, TEXT("HandleQueryRequest_Locations"))
	TArray<FVector> OutLocations;
	if(Result->IsSuccessful())
	{
		Result->GetAllAsLocations(OutLocations);
	}
	QueryLocations = OutLocations;
}

void ABaseAIController::QueryForActors_GameplayTags(const FGameplayTagContainer& InGameplayTagContainer, const EQueryType QueryType,
                                                    const UEnvQuery* const InEnvQuery, APawn* InPawn, const float SearchRadius, const float MinFindRadius, const float MaxFindRadius)
{
	TagsToBeTested = InGameplayTagContainer;
	CurrentQueryType = QueryType;
	FEnvQueryRequest ActorsQueryRequest = FEnvQueryRequest(InEnvQuery, InPawn);

	if(SearchRadius > 0) ActorsQueryRequest.SetFloatParam(FName("SearchRadius"), SearchRadius);
	else ActorsQueryRequest.SetFloatParam(FName("SearchRadius"), 1000.f);
	
	if(MinFindRadius > 0 && MinFindRadius < MaxFindRadius) ActorsQueryRequest.SetFloatParam(FName("MinFindRadius"), MinFindRadius);
	else ActorsQueryRequest.SetFloatParam(FName("MinFindRadius"), 0.f);
	
	if(MaxFindRadius > 0 && MaxFindRadius > MinFindRadius) ActorsQueryRequest.SetFloatParam(FName("MaxFindRadius"), MaxFindRadius);
	else if(SearchRadius > 0) ActorsQueryRequest.SetFloatParam(FName("MaxFindRadius"), SearchRadius);
	else ActorsQueryRequest.SetFloatParam(FName("MaxFindRadius"), 1000.f);
	
	ActorsQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &ABaseAIController::HandleQueryRequest); //TODO: Change EEnvQueryRunMode to dynamic property
}

void ABaseAIController::HandleQueryRequest(TSharedPtr<FEnvQueryResult> Result)
{
	ClearCustomAIContainer();
	
	if(Result->IsSuccessful())
	{
		TArray<AActor*> OutActors;
		Result->GetAllAsActors(OutActors);
		if(!OutActors.IsEmpty())
		{
			for(AActor* OutActor : OutActors)
			{
				// ReSharper disable once CppTooWideScope
				IQueryable* QueryableInterface = Cast<IQueryable>(OutActor);
				if(QueryableInterface)
				{
					FGameplayTagQuery NewQuery;
					
					switch(CurrentQueryType)
					{
					case EQueryType::EQT_AllTags:
						NewQuery = FGameplayTagQuery::MakeQuery_MatchAllTags(TagsToBeTested);
						break;
					case EQueryType::EQT_AnyTags:
						NewQuery = FGameplayTagQuery::MakeQuery_MatchAnyTags(TagsToBeTested);
						break;
					case EQueryType::EQT_NoTags:
						NewQuery = FGameplayTagQuery::MakeQuery_MatchNoTags(TagsToBeTested);
						break;
					case EQueryType::EQT_SingleTag:
						NewQuery = FGameplayTagQuery::MakeQuery_MatchTag(TagsToBeTested.First());
						break;
					default:
						UE_LOG(LogTemp, Warning, TEXT("Query type not valid!"))
						break;
					}
					
					bool bMatchesQuery = QueryableInterface->GetGameplayTagContainer().MatchesQuery(NewQuery);
					if(bMatchesQuery)
					{
						AddToCustomAIContainer(OutActor);
					} else UE_LOG(LogTemp, Display, TEXT("bMatchesQuery was false"))
				}
			}
		}else UE_LOG(LogTemp, Display, TEXT("OutActors was Empty"))
	} else UE_LOG(LogTemp, Display, TEXT("Result was unsuccessful"))
	
	//UE_LOG(LogTemp, Display, TEXT("Number of Matches: %d"), CustomAIContainer->ActorContainer.Num())
	if(BlackboardComponent) BlackboardComponent->SetValueAsObject(FName("QueryActors"), CustomAIContainer);
	
	TagsToBeTested = FGameplayTagContainer::EmptyContainer;
	CurrentQueryType = EQueryType::EQT_MAX;
}

UBehaviorTree* ABaseAIController::GetBehaviorTree(const FName BehaviorTreeName) const
{
	const FString BehaviorTreeTablePath(TEXT("DataTable'/Game/Assets/Datatables/BehaviorTreeTable.BehaviorTreeTable'"));
	// ReSharper disable once CppTooWideScope
	const UDataTable* BehaviorTreeTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *BehaviorTreeTablePath));
	if(BehaviorTreeTableObject)
	{
		// ReSharper disable once CppTooWideScope
		const FBehaviorTreeTable* TableRow = BehaviorTreeTableObject->FindRow<FBehaviorTreeTable>(FName(BehaviorTreeName), TEXT(""));
		if(TableRow)
		{
			return TableRow->BehaviorTree;
		}
	}
	return nullptr;
}

void ABaseAIController::SetLastStimulusType(const ELastStimulusType InStimulusType, const AActor* const InActor)
{
	
	if(InStimulusType == ELastStimulusType::ELST_MAX)
	{
		const FActorPerceptionInfo* ActorPerceptionInfo = PerceptionComponent->GetActorInfo(*InActor);
		for(const FAIStimulus AIStimulus : ActorPerceptionInfo->LastSensedStimuli)
		{
			switch (AIStimulus.Type)
			{
			case 0:
				LastStimulusType = ELastStimulusType::ELST_Sight;
				break;
			case 1:
				LastStimulusType = ELastStimulusType::ELST_Hearing;
			default:
				LastStimulusType = ELastStimulusType::ELST_MAX;
				break;
			}
		}
	}
	else
	{
		LastStimulusType = InStimulusType;
	}

	BlackboardComponent->SetValueAsEnum(TEXT("LastStimulusType"), (uint8)LastStimulusType);
	
}

void ABaseAIController::SetHearingStimulusHasUpdated(const bool bUpdated)
{
	if(BlackboardComponent == nullptr) return;
	
	bHearingStimulusHasUpdated = bUpdated;
	BlackboardComponent->SetValueAsBool(TEXT("HearingStimulusHasUpdated"), bHearingStimulusHasUpdated);
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(InPawn == nullptr) return;
	
	if(const AAIBase* const Enemy = Cast<AAIBase>(InPawn))
	{
		if(Enemy->GetBehaviorTree())
		{
			if(BehaviorTreeComponent && BlackboardComponent)
			{
				BlackboardComponent->InitializeBlackboard(*(Enemy->GetBehaviorTree()->GetBlackboardAsset()));
				BehaviorTreeComponent->StartTree(*GetBehaviorTree("Default"), EBTExecutionMode::Looped); //TODO: Instead of "Default" make it dynamic based on Gameplay Tags
				//RunBehaviorTree(GetBehaviorTree("Default")); //BUG: RunBehaviorTree doesn't check if the tree is running already
				BehaviorTreeComponent->SetDynamicSubtree(FGameplayTag::RequestGameplayTag(FName("Subtree.Work")), GetBehaviorTree("Work"));
				BlackboardComponent->SetValueAsEnum(TEXT("LastStimulusType"), (uint8)ELastStimulusType::ELST_MAX);
				BlackboardComponent->SetValueAsBool(TEXT("HearingStimulusHasUpdated"), true);
			}
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
	//UE_LOG(LogTemp, Warning, TEXT("OnTargetPerceptionUpdated_Delegate"))
	if(InActor == nullptr || BlackboardComponent == nullptr || AIBase == nullptr) return;
	
	ETeamAttitude::Type AttitudeType = ETeamAttitude::Neutral;
	if(IBaseCharacterInfo* CharacterInterface = Cast<IBaseCharacterInfo>(InActor))
	{
		AttitudeType = AIBase->GetFactionAttitude(CharacterInterface->GetNPCInfo());
	}
	
	switch (Stimulus.Type)
	{
	case 0:
		//Sight
		if(ETeamAttitude::Hostile == AttitudeType)
		{
			
			if(Stimulus.WasSuccessfullySensed())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Enemy sighted!"))
				AIBase->ToggleSpeechWidget("Enemy Sighted");
				BlackboardComponent->SetValueAsBool(FName("PlayerSeen"), true);
				AIBase->SetPlayerSeen(true);
				BlackboardComponent->SetValueAsBool(FName("CanSeeTarget"), true);
				AIBase->SetCanSeeTarget(true);
				BlackboardComponent->ClearValue(FName("PointOfInterest"));
				BlackboardComponent->ClearValue(FName("PredictedTargetLocation"));
				AIPerceptionComponent->LastPerceivedActors_Sight.AddUnique(InActor);
				SetLastStimulusType(ELastStimulusType::ELST_Sight, InActor);
				//SetStimulusHasUpdated(true); //NOTE currently used only for hearing
				
			}
			else
			{
				AIBase->ToggleSpeechWidget("Lost sight of the Enemy");
				AIBase->SetCanSeeTarget(false);
				BlackboardComponent->SetValueAsBool(FName("CanSeeTarget"), false);
				UAISense_Prediction::RequestPawnPredictionEvent(GetPawn(), InActor, 2.f);
			}
			
			BlackboardComponent->SetValueAsObject(FName("Target"), InActor);
			BlackboardComponent->SetValueAsVector(FName("LastTargetLocation"), InActor->GetActorLocation());

			//UE_LOG(LogTemp, Warning, TEXT("Stimulus Debug: %s"), *Stimulus.GetDebugDescription())
			
		}
		else if(ETeamAttitude::Friendly == AttitudeType)
		{
			if(Stimulus.WasSuccessfullySensed())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Henlo fren."))
			}
			else
			{
				
			}
		}
		else
		{
			if(Stimulus.WasSuccessfullySensed())
			{
				//UE_LOG(LogTemp, Warning, TEXT("I have no strong feelings one way or the other."))
			}
			else
			{
				
			}
		}
		break;
	case 1:
		//Hearing
		if(ETeamAttitude::Hostile == AttitudeType)
		{
			
			if(Stimulus.WasSuccessfullySensed())
			{
				if(AIBase->GetPlayerSeen() == false)
				{
					if(AIBase->GetAIState() != EAIState::EAIS_Chasing ||
						AIBase->GetAIState() != EAIState::EAIS_InCombat ||
						AIBase->GetAIState() != EAIState::EAIS_Dead) AIBase->SetAIState(EAIState::EAIS_Alerted);
					
					AIBase->ToggleSpeechWidget("Herd sum ting");
					BlackboardComponent->SetValueAsVector(FName("PointOfInterest"), Stimulus.StimulusLocation);
					AIPerceptionComponent->LastPerceivedActors_Hearing.AddUnique(InActor);
				}
				
				SetLastStimulusType(ELastStimulusType::ELST_Hearing, InActor);
				BlackboardComponent->SetValueAsVector(FName("LastTargetLocHearing"), Stimulus.StimulusLocation);
				SetHearingStimulusHasUpdated(true);
				
			}
			//UE_LOG(LogTemp, Warning, TEXT("Heard sum ting"))
		}
		break;
	case 2:
		//Prediction
		
		//DrawDebugBox(GetWorld(), Stimulus.StimulusLocation, FVector(10.f), FColor::Emerald, false, 5.f);
		BlackboardComponent->SetValueAsVector(FName("PredictedTargetLocation"), Stimulus.StimulusLocation);
		break;
		
	default:
		//Do the default thing
		break;
	}
}

void ABaseAIController::OnTargetPerceptionInfoUpdated_Delegate(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	
	
}

void ABaseAIController::OnSightStimulusExpired_Delegate()
{
	if(BlackboardComponent == nullptr || AIBase == nullptr) return;
	
	if(AIBase->GetAIState() == EAIState::EAIS_Moving) return; //NOTE Temporary fix
	
	BlackboardComponent->SetValueAsBool(FName("PlayerSeen"), false);
	AIBase->SetPlayerSeen(false);
	AIBase->ToggleSpeechWidget("Target got away.");
	AIBase->SetAIState(AIBase->GetLastAIState());
}

void ABaseAIController::OnHearingStimulusExpired_Delegate()
{
	if(BlackboardComponent == nullptr || AIBase == nullptr) return;
	
	if(AIBase->GetAIState() == EAIState::EAIS_Moving) return; //NOTE Temporary fix
	
	if(!AIBase->GetPlayerSeen())
	{
		AIBase->ToggleSpeechWidget("Must have been wind.");
		AIBase->SetAIState(AIBase->GetLastAIState());
	}
}

void ABaseAIController::OnPerceptionUpdated_Delegate(const TArray<AActor*>& UpdatedActors)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnPerceptionUpdated_Delegate"))
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
	static const FName NAME_MaxAge_Sight = FName("MaxAgeSight");
	static const FName NAME_MaxAge_Hearing = FName("MaxAgeHearing");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PropertyChangedEvent.Property->GetName())
	if(PropertyChangedEvent.Property) //TODO: Use switch
	{
		if(PropertyChangedEvent.Property->GetFName() == NAME_TeamAttitudeMap_Sight)
		{
			ChangeAttitudeTowards();
		}
		else if(PropertyChangedEvent.Property->GetFName() == NAME_TeamAttitudeMap_Hearing)
		{
			ChangeAttitudeTowards();
		}
		else if(PropertyChangedEvent.Property->GetFName() == NAME_MaxAge_Sight)
		{
			if(PerceptionComponent == nullptr) return;
			SenseConfig_Sight->SetMaxAge(MaxAgeSight);
			PerceptionComponent->ConfigureSense(*SenseConfig_Sight);
			PerceptionComponent->PostInitProperties();
		}
		else if(PropertyChangedEvent.Property->GetFName() == NAME_MaxAge_Hearing)
		{
			if(PerceptionComponent == nullptr) return;
			SenseConfig_Hearing->SetMaxAge(MaxAgeHearing);
			PerceptionComponent->ConfigureSense(*SenseConfig_Hearing);
			PerceptionComponent->PostInitProperties();
		}
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ABaseAIController::ShowAIInfo()
{
	TArray<AActor*> ActorArrayKnown;
	TArray<AActor*> ActorArrayCurrent;
	
	if(AIPerceptionComponent == nullptr || GEngine == nullptr) return;
	
	AIPerceptionComponent->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), ActorArrayKnown);
	AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), ActorArrayCurrent);
	
	FActorPerceptionBlueprintInfo PerceptionBlueprintInfo;
	for (AActor* Acter : ActorArrayKnown)
	{
		AIPerceptionComponent->GetActorsPerception(Acter, PerceptionBlueprintInfo);
		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Blue, FString::Printf(TEXT("Known Perceived Actor: %s, Is Hostile: %d"), *Acter->GetName(), PerceptionBlueprintInfo.bIsHostile));
	}
	for(AActor* Ector : ActorArrayCurrent)
	{
		AIPerceptionComponent->GetActorsPerception(Ector, PerceptionBlueprintInfo);
		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Cyan, FString::Printf(TEXT("Currently Perceived Actor: %s, Is Hostile: %d"), *Ector->GetName(), PerceptionBlueprintInfo.bIsHostile));
	}

	
}


