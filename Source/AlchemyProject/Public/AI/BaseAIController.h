// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DetourCrowdAIController.h"
#include "GameplayTagContainer.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Utility/CustomAIContainer.h"
#include "BaseAIController.generated.h"

UENUM(BlueprintType)
enum class EQueryType : uint8
{
	EQT_AllTags UMETA(DisplayName = "AllTags"),
	EQT_AnyTags UMETA(DisplayName = "AnyTags"),
	EQT_NoTags UMETA(DisplayName = "NoTags"),
	EQT_SingleTag UMETA(DisplayName = "SingleTag"),
	
	EQT_MAX UMETA(DisplayName = "DefaultMax")
};

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABaseAIController(const FObjectInitializer& ObjectInitializer);
	virtual void OnPossess(APawn* InPawn) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	UFUNCTION()
	void OnTargetPerceptionUpdated_Delegate(AActor* InActor, FAIStimulus Stimulus);
	UFUNCTION()
	void OnTargetPerceptionInfoUpdated_Delegate(const FActorPerceptionUpdateInfo& UpdateInfo);
	UFUNCTION()
	void OnSightStimulusExpired_Delegate();
	UFUNCTION()
	void OnHearingStimulusExpired_Delegate();
	UFUNCTION()
	void OnPerceptionUpdated_Delegate(const TArray<AActor*>& UpdatedActors);
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector>& QueryForLocations(const class UEnvQuery* const InEnvQuery, APawn* InPawn, EEnvQueryRunMode::Type QueryRunMode);
	void HandleQueryRequest_Locations(TSharedPtr<FEnvQueryResult> Result);
	TArray<FVector> QueryLocations;
	
	void ChangeAttitudeTowards();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	void ShowAIInfo();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class AAIBase* AIBase;

	/**********************
	 *	Perception
	 **********************/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMyAIPerceptionComponent> AIPerceptionComponent{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Sight> SenseConfig_Sight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Hearing> SenseConfig_Hearing{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Prediction> SenseConfig_Prediction{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	class UAISense_Sight* SightSense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	class UAISense_Hearing* HearingSense;

	FGenericTeamId TeamId{FGenericTeamId(1)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Teams", meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<ETeamAttitude::Type>, bool> TeamAttitudeMap_Sight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Teams", meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<ETeamAttitude::Type>, bool> TeamAttitudeMap_Hearing;


	UFUNCTION(BlueprintCallable)
	void QueryForActors_GameplayTags(const struct FGameplayTagContainer& InGameplayTagContainer, const EQueryType QueryType, const UEnvQuery* const InEnvQuery, APawn* InPawn, const float SearchRadius, const float MinFindRadius, const float MaxFindRadius);

	void HandleQueryRequest(TSharedPtr<struct FEnvQueryResult> Result);

	FGameplayTagContainer TagsToBeTested;
	EQueryType CurrentQueryType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCustomAIContainer* CustomAIContainer;

	class UBehaviorTree* GetBehaviorTree(const FName BehaviorTreeName) const;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Values", meta = (AllowPrivateAccess = "true"))
	float MaxSightRadius{2000.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Values", meta = (AllowPrivateAccess = "true"))
	float PeripheralVisionAngle{73.f};
	
	float CheckStimulusTimer{0.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Values", meta = (AllowPrivateAccess = "true"))
	float MaxAgeSight{30.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Values", meta = (AllowPrivateAccess = "true"))
	float MaxAgeHearing{30.f};
	
public:

	FORCEINLINE UBlackboardComponent* GetAIBlackboardComponent() const {return BlackboardComponent;}
	FORCEINLINE UBehaviorTreeComponent* GetAIBehaviorTreeComponent() const {return BehaviorTreeComponent;}
	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const override {return TeamId;}
	FORCEINLINE UCustomAIContainer* GetCustomAIContainer() const {return CustomAIContainer;}

	FORCEINLINE void AddToCustomAIContainer(AActor* ActorToAdd) const {if(CustomAIContainer) CustomAIContainer->ActorContainer.Add(ActorToAdd);}
	FORCEINLINE void ClearCustomAIContainer() const {if(CustomAIContainer) CustomAIContainer->ActorContainer.Empty();}
};
