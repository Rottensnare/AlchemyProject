// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlchemyTable.generated.h"

UCLASS()
class ALCHEMYPROJECT_API AAlchemyTable : public AActor
{
	GENERATED_BODY()
	
public:	
	AAlchemyTable();

protected:
	virtual void BeginPlay() override;

private:

	//The character using this alchemy table
	UPROPERTY()
	class APlayerCharacter* CurrentCharacter;

	//stores item references which can be viewed in the alchemy table UI
	UPROPERTY(EditAnywhere)
	class UInventoryComponent* AlchemyInventory;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* TableMesh;

	//Collision for determining which character is in range of the table
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;

};
