// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/PatrolArea.h"

#include "NavigationSystem.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APatrolArea::APatrolArea()
{
	PrimaryActorTick.bCanEverTick = false;

	PatrolArea = CreateDefaultSubobject<UBoxComponent>(TEXT("PatrolArea"));
	PatrolArea->SetupAttachment(GetRootComponent());
	PatrolArea->InitBoxExtent(FVector(100.f));
}

FVector APatrolArea::GetRandomPatrolCoordinates()
{

	for(int i = 0; i < 4; i++)
	{
		FVector Point =  UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), PatrolArea->GetScaledBoxExtent());
		//UE_LOG(LogTemp, Warning, TEXT("Point: %s"), *Point.ToString())
		FNavLocation NavLoc;
		bool bOnNavMesh = UNavigationSystemV1::GetNavigationSystem(GetWorld())->ProjectPointToNavigation( Point, NavLoc); // Checks if hit location has valid NavMesh Path
		if(bOnNavMesh)
		{
			i = 4;
			//UE_LOG(LogTemp, Warning, TEXT("NavLoc: "), *NavLoc.Location.ToString())
			return NavLoc.Location;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("PatrolPoint Couldn't be generated."))
	return FVector();
}


