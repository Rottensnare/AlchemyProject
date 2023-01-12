// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolArea.generated.h"

UCLASS()
class ALCHEMYPROJECT_API APatrolArea : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APatrolArea();
	FVector GetRandomPatrolCoordinates();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* PatrolArea;
	


};
