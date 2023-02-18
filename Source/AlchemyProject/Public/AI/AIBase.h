// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "AlchemyProject/Alchemy/CustomStructs/NPCStructs.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/BaseCharacterInfo.h"
#include "Interfaces/Interactable.h"
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

UCLASS(AutoCollapseCategories = ("Perception"))
class ALCHEMYPROJECT_API AAIBase : public ACharacter, public IQueryable, public IInteractable, public IBaseCharacterInfo
{
	GENERATED_BODY()

public:
	AAIBase();

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable)
	virtual void SetAIState(EAIState NewState);
	
	void SetSpeechWidgetTimer();
	void ClearSpeechWidgetTimer();
	UFUNCTION()
	void ToggleSpeechWidget(const FString InString = FString(""));
	ETeamAttitude::Type GetFactionAttitude(const FNPCInfo& DetectedNPCInfo) const;
	void DestinationReached();
	
protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Components", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UNavigationInvokerComponent* NavigationInvokerComponent;

	/** Used for detecting the player very close if AI bPlayerSeen = true */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ESPSphere;
	
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class ABaseAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	EAIState AIState;

	//LastAIState shouldn't be an AIState that is used for combat purposes, like InCombat, Chasing, Alerted
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	EAIState LastAIState;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	//class UCharacterData* CharacterData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	FNPCInfo NPCInfo;

	
	/** NAVIGATION */
	
	//Current road names that the AI will follow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Utility", meta = (AllowPrivateAccess = "true"))
	TArray<FName> RoadNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI|Utility", meta = (AllowPrivateAccess = "true"))
	class ARoadSpline* CurrentRoad;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI|Utility", meta = (AllowPrivateAccess = "true"))
	FName CurrentRoadName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI|Utility", meta = (AllowPrivateAccess = "true"))
	int32 CurrentRoadIndex{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Utility", meta = (AllowPrivateAccess = "true"))
	AActor* NavDestination{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI|Utility", meta = (AllowPrivateAccess = "true"))
	bool bRoadsFound = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI|Utility", meta = (AllowPrivateAccess = "true"))
	TArray<FRoadInfo> CurrentRoadInfos;

	//Used for EQS context
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector ClosestSplinePointLocation{FVector(0.f)};


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

	UFUNCTION()
	void OnESPBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnESPEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	bool bESPOverlapping{false};
	
private:	

	bool bPlayerSeen{false};
	bool bCanSeeTarget{false};
	bool bSomethingHeard{false};
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI|Movement", meta = (AllowPrivateAccess = "true"))
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
	bool bCanBeInteractedWith{true};
	//Player can have a conversation, meaning that a dialogue widget will pop up.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Interaction", meta = (AllowPrivateAccess = "true"))
	bool bCanConverse{true};
	//Map that stores information about the opinion of other NPCs.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Opinions", meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> OpinionTable;
	
	UFUNCTION(BlueprintCallable)
	void GetGameMode();

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
	FORCEINLINE EAIState GetLastAIState() const {return LastAIState;}
	FORCEINLINE TArray<FName>& GetRoadNames() {return RoadNames;};
	FORCEINLINE void SetRoadNames(const TArray<FName>& InRoadNames) {RoadNames = InRoadNames;};
	FORCEINLINE FName& GetCurrentRoadName() {return CurrentRoadName;}
	FORCEINLINE ARoadSpline* GetCurrentRoad() const {return CurrentRoad;}
	FORCEINLINE int32 GetCurrentRoadIndex() const {return CurrentRoadIndex;}
	FORCEINLINE void SetCurrentRoad(ARoadSpline* InRoadSpline);
	FORCEINLINE AActor* GetNavDestination() const {return NavDestination;}
	FORCEINLINE void SetNavDestination(AActor* InActor) {NavDestination = InActor;}
	FORCEINLINE void SetRoadsFound(const bool bFound) {bRoadsFound = bFound;}
	FORCEINLINE TArray<FRoadInfo>& GetCurrentRoadInfos() {return CurrentRoadInfos;}

	/***********************
	 *	Public Variables
	 **********************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|GameplayTags")
	FGameplayTagContainer GameplayTagContainer;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|GameplayTags")
	//FTagsToSearch TagsToSearch;

	UPROPERTY(EditAnywhere, Category = "AI|Dialogue")
	int32 NPC_ID{1};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AAlchemyProjectGameMode* AlchemyProjectGameMode;

	/***********************
	 *	INTERFACE OVERRIDES
	 **********************/

	virtual bool Interact(AActor* OtherActor) override;
	virtual FNPCInfo& GetNPCInfo() override;
	virtual FGameplayTagContainer& GetGameplayTagContainer() override;
	UFUNCTION(BlueprintCallable)
	virtual EPhysicalSurface GetFootStepSurfaceType() override;
};

template <typename T>
void AAIBase::SetBlackboardValue(FName InName, T InValue)
{
	if(AIController && AIController->GetAIBlackboardComponent())
	{
		AIController->GetAIBlackboardComponent()->SetValue<T>(InName, InValue);
	}
}
