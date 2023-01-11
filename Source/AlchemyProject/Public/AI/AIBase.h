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
	UFUNCTION()
	virtual void OnSomethingHeard(APawn* InInstigator, const FVector& Location, float Volume);
	
	template<typename T>
	void SetBlackboardValue(FName InName, T InValue);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;
	
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class ABaseAIController* AIController;

	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	EAIState AIState;

	//Only works with Editor and changing values from the editor windows
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
private:	

	bool bPlayerSeen{false};
	bool bSomethingHeard{false};
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI|Movement", meta = (AllowPrivateAccess = "true") )
	bool bFollowPlayer{false};
	void SetFollowPlayer(bool Value);
	
	FTimerHandle PlayerSeenTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float PlayerSeenTimerTime{30.f};
	UFUNCTION()
	void ResetPlayerSeen();

	//Array of all the actors that the AI might care about
	//Not meant for keeping track of the other NPCs
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI|Containers", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> ActorsOfInterest;

	//Current actor of interest
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AActor* CurrentAOI;

	UFUNCTION(BlueprintCallable)
	void AddToActorsOfInterest(AActor* InActor);
	UFUNCTION(BlueprintCallable)
	void RemoveFromActorsOfInterest(AActor* InActor);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI|Navigation", meta = (AllowPrivateAccess = "true"))
	FVector PointOfInterest{FVector::ZeroVector};
	
public:

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}
	FORCEINLINE EAIState GetAIState() const {return AIState;}
	FORCEINLINE bool GetPlayerSeen() const {return bPlayerSeen;}
	FORCEINLINE void SetPlayerSeen(const bool bValue) {bPlayerSeen = bValue;}
	FORCEINLINE bool GetFollowPlayer() const {return bFollowPlayer;}
};

template <typename T>
void AAIBase::SetBlackboardValue(FName InName, T InValue)
{
	if(AIController && AIController->GetAIBlackboardComponent())
	{
		AIController->GetAIBlackboardComponent()->SetValue<T>(InName, InValue);
	}
}
