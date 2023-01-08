// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ClockWidget.h"

#include "Components/RichTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Time/TimeManager.h"


void UClockWidget::UpdateWidget()
{
	const FText NewText = FText::FromString(FString::Printf(TEXT("<Header1>%02d : %02d</>"), CurrentTimeDate.CurrentHour, CurrentTimeDate.CurrentMinute));
	ClockText->SetText(NewText);
}

void UClockWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if(TimeManager)
	{
		CurrentTimeDate = TimeManager->GetTimeDate();
		UpdateWidget();
	}
	
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UClockWidget::NativeConstruct()
{
	const auto TempActor = UGameplayStatics::GetActorOfClass(this, ATimeManager::StaticClass());
	if(ATimeManager* TempTimeManager = Cast<ATimeManager>(TempActor))
	{
		TimeManager = TempTimeManager;
	}
	Super::NativeConstruct();
}
