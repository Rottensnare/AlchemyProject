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

	if(AIInteractionCompClass)
	{
		AIInteractionComponent = NewObject<UAIInteractionComponent>(this, AIInteractionCompClass);
	}
}

bool AAIInteractionActor::ExecuteInteraction(const FGameplayTagContainer& InContainer)
{
	if(bIsTaken) return false;

	const FGameplayTagQuery NewQuery = FGameplayTagQuery::MakeQuery_MatchAllTags(GameplayTagContainer);
	if(InContainer.MatchesQuery(NewQuery))
	{
		bIsTaken = true;
		if(AIInteractionComponent->ExecuteFunctionality())
		{
			bIsTaken = false;
			return true;
		}
	}
	
		return false;
}


