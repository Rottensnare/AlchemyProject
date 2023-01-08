// Fill out your copyright notice in the Description page of Project Settings.


#include "Time/TimeManager.h"

#include "Engine/DirectionalLight.h"

ATimeManager::ATimeManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeManager::BeginPlay()
{
	CurrentHour = StartHour;
	
	Super::BeginPlay();
}



void ATimeManager::Tick(float DeltaSeconds)
{
	AActor::Tick(DeltaSeconds);
	
	CalculateDateTime(DeltaSeconds);
	RotateTheSun();
	UpdateDateTime();
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
	if(CurrentHour < 6) CurrentTimeOfDay = TimesOfDay[0];
	else if(CurrentHour < 12) CurrentTimeOfDay = TimesOfDay[1];
	else if(CurrentHour < 18) CurrentTimeOfDay = TimesOfDay[2];
	else CurrentTimeOfDay = TimesOfDay[3];
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



void ATimeManager::RotateTheSun()
{
	if(TheSun == nullptr) return;
	
	const float CurrentTimeValue = (float)(CurrentHour * 60 * 60 + CurrentMinute * 60 + CurrentSecond);
	constexpr float MaxTimeValue = 86400.f;
	const float NormalizedTimeValue = CurrentTimeValue / MaxTimeValue;
	const FRotator NewRotator = FRotator((360 * NormalizedTimeValue) + 90.f, 0.f, 0.f);
	TheSun->SetActorRotation(NewRotator);
}

void ATimeManager::UpdateDateTime()
{
	CurrentTimeDate.CurrentSecond = CurrentSecond;
	CurrentTimeDate.CurrentMinute = CurrentMinute;
	CurrentTimeDate.CurrentHour = CurrentHour;
	CurrentTimeDate.DayNumber = DayNumber;
	CurrentTimeDate.MonthNumber = MonthNumber;
	CurrentTimeDate.Year = Year;
	CurrentTimeDate.CurrentDayName = CurrentDayName;
	CurrentTimeDate.CurrentMonthName = CurrentMonthName;
}
