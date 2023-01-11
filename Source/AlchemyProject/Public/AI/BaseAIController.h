// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABaseAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	UFUNCTION()
	void OnTargetPerceptionUpdated_Delegate(AActor* InActor, FAIStimulus Stimulus);
	
	void ChangeAttitudeTowards();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	/**********************
	 *	Perception
	 **********************/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Sight> SenseConfig_Sight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Hearing> SenseConfig_Hearing{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	class UAISense_Sight* SightSense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	class UAISense_Hearing* HearingSense;

	FGenericTeamId TeamId{FGenericTeamId(1)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Teams", meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<ETeamAttitude::Type>, bool> TeamAttitudeMap_Sight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Teams", meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<ETeamAttitude::Type>, bool> TeamAttitudeMap_Hearing;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Values", meta = (AllowPrivateAccess = "true"))
	float MaxSightRadius{2000.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Values", meta = (AllowPrivateAccess = "true"))
	float PeripheralVisionAngle{73.f};
	
	float CheckStimulusTimer{0.f};
public:

	FORCEINLINE UBlackboardComponent* GetAIBlackboardComponent() const {return BlackboardComponent;}
	FORCEINLINE UBehaviorTreeComponent* GetAIBehaviorTreeComponent() const {return BehaviorTreeComponent;}
	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const override {return TeamId;}
	
};
