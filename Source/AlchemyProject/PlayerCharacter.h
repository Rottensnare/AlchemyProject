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

	UPROPERTY(VisibleAnywhere)
	AActor* TracedActor;
	
	TSubclassOf<class AItem> ItemClass;

	UPROPERTY(EditAnywhere)
	float SweepRadius{200.f};

	UPROPERTY(VisibleAnywhere)
	class AAlchemyTable* CurrentAlchemyTable;

	

public:
	
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const {return InventoryComponent;}
	FORCEINLINE UAlchemyComponent* GetAlchemyComponent() const {return AlchemyComponent;}
	FORCEINLINE UHealthComponent* GetHealthComponent() const {return HealthComponent;}
	FORCEINLINE void SetAlchemyTable(AAlchemyTable* InTable) {CurrentAlchemyTable = InTable;}
	FORCEINLINE AAlchemyTable* GetAlchemyTable() const {return CurrentAlchemyTable;}
	
};



