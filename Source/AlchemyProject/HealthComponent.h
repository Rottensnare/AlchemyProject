// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMYPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	friend class APlayerCharacter;
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

	TMap<UDamageType*, float> DamageResistanceMap;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health{100.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth{200.f};

	UPROPERTY()
	class AController* OwnerController;

public:

	FORCEINLINE float GetHealth() const {return Health;}
	FORCEINLINE void SetHealth(const float InHealth) {Health = InHealth;}
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}
	FORCEINLINE void SetMaxHealth(const float InMaxHealth) {MaxHealth = InMaxHealth;}
	
};
