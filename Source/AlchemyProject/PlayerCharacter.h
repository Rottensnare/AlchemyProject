// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProjectGameMode.h"
#include "Alchemy/AlchemyItem.h"
#include "Alchemy/Components/Potion/PotionComponent.h"
#include "Alchemy/CustomStructs/NPCStructs.h"
#include "GameFramework/Character.h"
#include "Interfaces/BaseCharacterInfo.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ALCHEMYPROJECT_API APlayerCharacter : public ACharacter, public IQueryable, public IBaseCharacterInfo
{
	GENERATED_BODY()

public:

	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable)
	void UpdateInventorySlotAmount(const int32 Index, const int32 Amount);

	UPROPERTY(BlueprintReadWrite)
	bool bIsDoingAlchemy{false};

	UPROPERTY(BlueprintReadWrite)
	bool bIsConversing{false};
	
	void UsePotion(const TSubclassOf<UPotionComponent> InComponentClass, const EProductQuality InProductQuality);

	UPROPERTY(VisibleAnywhere, Category = Alchemy)
	TMap<UActorComponent*, int32> CurrentPotionComponents;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//class UCharacterData* CharacterData;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//FNPCInfo CharacterInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	FNPCInfo NPCInfo;

protected:

	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void InventoryButtonPressed();
	void InventoryButtonReleased();
	void InteractButtonPressed();
	void SweepInteractButtonPressed();
	void TraceForObjects();
	void ShowInfoButtonPressed();
	void ToggleSprint();
	void EndInteraction();
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere)
	class UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UAlchemyComponent* AlchemyComponent;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = Controls)
	float MouseXSensitivity{0.75f};

	UPROPERTY(EditAnywhere, Category = Controls)
	float MouseYSensitivity{0.75f};

	UPROPERTY()
	class AMyPlayerController* MyPlayerController;

	FInputModeGameAndUI InputModeGameAndUI;
	FInputModeGameOnly InputModeGameOnly;
	
	FTimerHandle HUDInitTimer;
	void HUDInitTimerFinished();
	float HUDInitTime{0.2f};

	bool bInventoryOpen{false};

	UPROPERTY(EditAnywhere)
	float ObjectTraceRadius{1000.f};
	UPROPERTY(EditAnywhere)
	float DebugObjectTraceRadius{8000.f};
	
	UPROPERTY(VisibleAnywhere)
	AActor* TracedActor;

	UPROPERTY(VisibleAnywhere)
	AItem* TracedItemLastFrame;

	UPROPERTY(VisibleAnywhere)
	APawn* CurrentNPC;

	int32 CurrentNPC_ID{0};
	
	TSubclassOf<class AItem> ItemClass;

	UPROPERTY(EditAnywhere)
	float SweepRadius{200.f};

	UPROPERTY(VisibleAnywhere)
	class AAlchemyTable* CurrentAlchemyTable;

	UPROPERTY()
	const USkeletalMeshSocket* HeadSocket{nullptr};

	UPROPERTY(EditAnywhere, Category = "Custom Movement")
	float SprintSpeed{600.f};
	UPROPERTY(EditAnywhere, Category = "Custom Movement")
	float WalkSpeed{230.f};
	UPROPERTY(VisibleAnywhere, Category = "Custom Movement")
	float DefaultWalkSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Custom Movement")
	bool bSprinting{true};

	/** CATEGORY Perception */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception|Components", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GameplayTags;

	/** CATEGORY Navigation */

	//NOTE: When jumping, a smart link is placed from jump start to jump end
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADynamicNavLinkProxy> NavLinkProxyClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ADynamicNavLinkProxy>> CurrentNavLinkProxies;

	int32 CurrentNavLinkID = 0;

	//Saved so that it can be used when Landed function is executed.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADynamicNavLinkProxy> CurrentNavProxy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess = "true"))
	int32 MaxNavLinkCount{10};

	//Point link vertical offset from the actor location. Felt like the actor location on its own was too high.
	UPROPERTY(EditAnywhere)
	FVector PointLinkOffset{FVector(0.f, 0.f, 40.f)};

	virtual void OnJumped_Implementation() override;

	virtual void Landed(const FHitResult& Hit) override;
	
	UPROPERTY(EditDefaultsOnly)
	bool bDebugging{false};

public:
	
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const {return InventoryComponent;}
	FORCEINLINE UAlchemyComponent* GetAlchemyComponent() const {return AlchemyComponent;}
	FORCEINLINE UHealthComponent* GetHealthComponent() const {return HealthComponent;}
	FORCEINLINE void SetAlchemyTable(AAlchemyTable* InTable) {CurrentAlchemyTable = InTable;}
	FORCEINLINE AAlchemyTable* GetAlchemyTable() const {return CurrentAlchemyTable;}
	FORCEINLINE UAIPerceptionStimuliSourceComponent* GetPerceptionStimuliSourceComponent() const {return PerceptionStimuliSourceComponent;}
	FORCEINLINE APawn* GetCurrentNPC() const {return CurrentNPC;}
	FORCEINLINE void SetCurrentNPC(APawn* InPawn) {CurrentNPC = InPawn;}
	FORCEINLINE int32 GetCurrentNPC_ID() const {return CurrentNPC_ID;}
	FORCEINLINE FGameplayTagContainer& GetGameplayTags() {return GameplayTags;}

	virtual FNPCInfo& GetNPCInfo() override;

	/* * * * * * * * * * * *
	 *	INTERFACE OVERRIDES
	 * * * * * * * * * * * */
	UFUNCTION(BlueprintCallable)
	virtual EPhysicalSurface GetFootStepSurfaceType() override;
};



