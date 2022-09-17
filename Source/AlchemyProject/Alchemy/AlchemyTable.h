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
	
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
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
