// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "AIBase.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAIS_Idle UMETA(DisplayName = "Idle"),
	EAIS_Moving UMETA(DisplayName = "Moving"),
	EAIS_Patrolling UMETA(DisplayName = "Patrolling"),
	EAIS_Alerted UMETA(DisplayName = "Alerted"),
	EAIS_InCombat UMETA(DisplayName = "InCombat"),
	EAIS_Unconscious UMETA(DisplayName = "Unconscious"),
	EAIS_Busy UMETA(DisplayName = "Busy"),
	EAIS_Dead UMETA(DisplayName = "Dead"),
	EAIS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class ALCHEMYPROJECT_API AAIBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAIBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetAIState(EAIState NewState);

	UFUNCTION()
	virtual void OnSeenPawn(APawn* InPawn);
	template<typename T>
	void SetBlackboardValue(FName InName, T InValue);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class ABaseAIController* AIController;

	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	EAIState AIState;
	
private:	

	bool bPlayerSeen{false};

	FTimerHandle PlayerSeenTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float PlayerSeenTimerTime{30.f};
	UFUNCTION()
	void ResetPlayerSeen();
	
public:

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}
	FORCEINLINE EAIState GetAIState() const {return AIState;}
	FORCEINLINE bool GetPlayerSeen() const {return bPlayerSeen;}
	FORCEINLINE void SetPlayerSeen(const bool bValue) {bPlayerSeen = bValue;}
};

template <typename T>
void AAIBase::SetBlackboardValue(FName InName, T InValue)
{
	if(AIController && AIController->GetAIBlackboardComponent())
	{
		AIController->GetAIBlackboardComponent()->SetValue<T>(InName, InValue);
	}
}
