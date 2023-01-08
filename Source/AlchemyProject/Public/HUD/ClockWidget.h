// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Time/TimeManager.h"
#include "ClockWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UClockWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void UpdateWidget();
	
protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	
private:
	
	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* ClockText;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* DateText;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FTimeDate CurrentTimeDate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ATimeManager* TimeManager;

public:

	FORCEINLINE void SetTimeDate(const FTimeDate& InTimeDate) {CurrentTimeDate = InTimeDate;}
	
};
