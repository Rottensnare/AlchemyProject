// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alchemy/AlchemyItem.h"
#include "Alchemy/Components/Potion/PotionComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ALCHEMYPROJECT_API APlayerCharacter : public ACharacter
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
	
	UPROPERTY(VisibleAnywhere)
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

	/** Perception */

	UPROPERTY()
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

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
};



