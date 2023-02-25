// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavLinkCustomInterface.h"
#include "Navigation/NavLinkProxy.h"
#include "DynamicNavLinkProxy.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ALCHEMYPROJECT_API ADynamicNavLinkProxy : public ANavLinkProxy, public INavLinkCustomInterface
{
	GENERATED_BODY()

public:

	//virtual void BeginPlay() override;

protected:

	//void OnNavLinkReach_Delegate( AActor* MovingActor, const FVector& DestinationPoint);
};
