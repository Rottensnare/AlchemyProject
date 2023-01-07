// Fill out your copyright notice in the Description page of Project Settings.


#include "Time/TimeManager.h"

ATimeManager::ATimeManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeManager::Tick(float DeltaSeconds)
{
	AActor::Tick(DeltaSeconds);
	
	CalculateDateTime(DeltaSeconds);
	
}

void ATimeManager::CalculateDateTime(float DeltaSeconds)
{
	
	const float TimeToAdd = DeltaSeconds * TimeScaler;
	CurrentSecond += TimeToAdd;
	//UE_LOG(LogTemp, Warning, TEXT("CurrentSecond: %f"), CurrentSecond)
	if(CurrentSecond < 60.f) return;
	UE_LOG(LogTemp, Warning, TEXT("%0f : %0d : %0d"), CurrentSecond, CurrentMinute, CurrentHour)
	CurrentSecond -= 60;
	CurrentMinute++;
	if(CurrentMinute < 60) return;
	CurrentMinute -= 60;
	CurrentHour++;
	if(CurrentHour < 24) return;
	CurrentHour -= 24;
	DayNumber++;
	if(DayNumber < 31) return;
	DayNumber -= 30;
	MonthNumber++;
	if(MonthNumber < 13) return;
	MonthNumber -= 12;
	Year++;
}


