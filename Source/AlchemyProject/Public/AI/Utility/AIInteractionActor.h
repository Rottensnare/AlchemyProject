// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "AIInteractionActor.generated.h"

UCLASS()
class ALCHEMYPROJECT_API AAIInteractionActor : public AActor
{
	GENERATED_BODY()
	
public:
	
	AAIInteractionActor();

	bool ExecuteInteraction(const FGameplayTagContainer& InContainer);
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* InteractionArea;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsTaken{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UAIInteractionComponent> AIInteractionCompClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UAIInteractionComponent* AIInteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GameplayTagContainer;
	
public:

	FORCEINLINE bool GetIsTaken() const {return bIsTaken;}
	FORCEINLINE UBoxComponent* GetInteractionArea() const {return InteractionArea;}
	FORCEINLINE UAIInteractionComponent* GetAIInteractionComponent() const {return AIInteractionComponent;}
	FORCEINLINE void SetAIInteractionComponent(UAIInteractionComponent* NewComponent) {AIInteractionComponent = NewComponent;}
};
