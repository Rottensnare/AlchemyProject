// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "AlchemyProject/Alchemy/CustomStructs/NPCStructs.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/Queryable.h"
#include "UI/SpeechWidget.h"
#include "AIBase.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAIS_Idle UMETA(DisplayName = "Idle"),
	//Moving means a state where the npc tries to go from A to B in non-combat situations
	EAIS_Moving UMETA(DisplayName = "Moving"),
	EAIS_Patrolling UMETA(DisplayName = "Patrolling"),
	EAIS_Chasing UMETA(DisplayName = "Chasing"),
	EAIS_Alerted UMETA(DisplayName = "Alerted"),
	EAIS_InCombat UMETA(DisplayName = "InCombat"),
	EAIS_Unconscious UMETA(DisplayName = "Unconscious"),
	EAIS_Busy UMETA(DisplayName = "Busy"),
	EAIS_Dead UMETA(DisplayName = "Dead"),
	EAIS_Working UMETA(DisplayName = "Working"),
	EAIS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class ALCHEMYPROJECT_API AAIBase : public ACharacter, public IQueryable
{
	GENERATED_BODY()

public:
	AAIBase();

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetAIState(EAIState NewState);
	
	void SetSpeechWidgetTimer();
	void ClearSpeechWidgetTimer();
	UFUNCTION()
	void ToggleSpeechWidget(const FString InString = FString(""));
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSeenPawn(APawn* InPawn);
	UFUNCTION()
	virtual void OnSomethingHeard(APawn* InInstigator, const FVector& Location, float Volume);
	
	template<typename T>
	void SetBlackboardValue(FName InName, T InValue);

	UFUNCTION(BlueprintCallable)
	bool FindNextPatrolPoint();
	
	//Deprecated
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	//class UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;
	
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class ABaseAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	EAIState AIState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	EAIState LastAIState;

#if WITH_EDITOR
	//Only works with Editor and changing values from the editor windows
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* SpeechWidgetComp;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	TSubclassOf<USpeechWidget> SpeechWidgetClass;

	FTimerHandle SpeechWidgetTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	float SpeechWidgetShowTime{4.f};
	UFUNCTION()
	void HideSpeechWidget();
	
	
private:	

	bool bPlayerSeen{false};
	bool bCanSeeTarget{false};
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

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI|Patrol", meta = (AllowPrivateAccess = "true"))
	FVector PointOfInterest{FVector::ZeroVector};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement", meta = (AllowPrivateAccess = "true"))
	float DefaultMoveSpeed{600.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement", meta = (AllowPrivateAccess = "true"))
	float CurrentMoveSpeed{0.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Movement", meta = (AllowPrivateAccess = "true"))
	float PatrolMoveSpeed{100.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol", meta = (AllowPrivateAccess = "true"))
	class APatrolArea* PatrolArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Patrol", meta = (AllowPrivateAccess = "true"))
	FVector OriginalPosition{FVector()};

	///////////////////////////
	/// Dialogue

	//Player can interact at least in some way, meaning that when pressing the interact button the NPC can do something.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Interaction", meta = (AllowPrivateAccess = "true"))
	bool bCanBeInteractedWith;
	//Player can have a conversation, meaning that a dialogue widget will pop up.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Interaction", meta = (AllowPrivateAccess = "true"))
	bool bCanConverse;
	//Map that stores information about the opinion of other NPCs.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Opinions", meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> OpinionTable;

public:
	/*******************
	 *	FORCEINLINES
	 *****************/
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}
	FORCEINLINE EAIState GetAIState() const {return AIState;}
	FORCEINLINE bool GetPlayerSeen() const {return bPlayerSeen;}
	FORCEINLINE void SetPlayerSeen(const bool bValue);
	FORCEINLINE bool GetFollowPlayer() const {return bFollowPlayer;}
	FORCEINLINE bool GetCanSeeTarget() const {return bCanSeeTarget;}
	FORCEINLINE void SetCanSeeTarget(const bool Value) {bCanSeeTarget = Value;}

	/***********************
	 *	Public Variables
	 **********************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|GameplayTags")
	FGameplayTagContainer GameplayTagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|GameplayTags")
	FTagsToSearch TagsToSearch;


	/***********************
	 *	INTERFACE OVERRIDES
	 **********************/


};

template <typename T>
void AAIBase::SetBlackboardValue(FName InName, T InValue)
{
	if(AIController && AIController->GetAIBlackboardComponent())
	{
		AIController->GetAIBlackboardComponent()->SetValue<T>(InName, InValue);
	}
}
