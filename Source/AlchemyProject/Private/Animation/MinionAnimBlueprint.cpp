// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MinionAnimBlueprint.h"

#include "AI/AIBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMinionAnimBlueprint::NativeInitializeAnimation()
{
	AIBase = Cast<AAIBase>(TryGetPawnOwner());
	
	Super::NativeInitializeAnimation();
}

void UMinionAnimBlueprint::UpdateAnimationProperties(float DeltaTime)
{
	AIBase = AIBase == nullptr ? Cast<AAIBase>(TryGetPawnOwner()) : AIBase;

	if(AIBase == nullptr)
	{
		return;
	}

	if(AIBase->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
	{
		bIsAccelerating = true;
	}else
	{
		bIsAccelerating = false;
	}

	FVector Velocity{AIBase->GetVelocity()};
	Velocity.Z = 0;
	Speed = Velocity.Size();
	
	bCrouching = AIBase->bIsCrouched;
}


