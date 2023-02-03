// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/AIInteractionActor.h"

#include "AI/Utility/AIInteractionComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AAIInteractionActor::AAIInteractionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionArea"));
	InteractionArea->SetupAttachment(GetRootComponent());
	
}

void AAIInteractionActor::PostInitializeComponents()
{
	if(AIInteractionCompClass)
	{
		AIInteractionComponent = NewObject<UAIInteractionComponent>(this, AIInteractionCompClass);
		AIInteractionComponent->RegisterComponent();
	}

	
	
	Super::PostInitializeComponents();
}

bool AAIInteractionActor::ExecuteInteraction(const FGameplayTagContainer& InContainer, AActor* InActor)
{
	if(bIsTaken || AIInteractionComponent == nullptr) return false;

	const FGameplayTagQuery NewQuery = FGameplayTagQuery::MakeQuery_MatchAllTags(InContainer);
	if(GameplayTagContainer.MatchesQuery(NewQuery))
	{
		bIsTaken = true;
		if(AIInteractionComponent->ExecuteFunctionality(InActor))
		{
			bIsTaken = false;
			return true;
		}
		UE_LOG(LogTemp, Warning, TEXT("ExecuteFunctionality false"))
	} else UE_LOG(LogTemp, Warning, TEXT("MatchesQuery false"))
		bIsTaken = false;
		return false;
}

FGameplayTagContainer& AAIInteractionActor::GetGameplayTagContainer()
{
	return GameplayTagContainer;
}


