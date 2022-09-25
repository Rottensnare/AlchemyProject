// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PotionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMYPROJECT_API UPotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPotionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void DestroyThisComponent();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	
	UPROPERTY()
	class APlayerCharacter* CurrentCharacter;

private:

	
	
	
		
};
