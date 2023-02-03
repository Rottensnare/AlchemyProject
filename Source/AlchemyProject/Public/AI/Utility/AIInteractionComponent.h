// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIInteractionComponent.generated.h"

/*
 *	This component was made to be attached to the AIInteractionActor, but can be attached to other actors as well.
 *	The interaction logic will added to this component class. 
 */

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMYPROJECT_API UAIInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UAIInteractionComponent();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent)
	bool ExecuteFunctionality();

		
};
