// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimeManager.generated.h"
/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API ATimeManager : public AActor
{
public:
	GENERATED_BODY()
	ATimeManager();


protected:

	virtual void Tick(float DeltaSeconds) override;
	void CalculateDateTime(float DeltaSeconds);

private:

	/*****************************************
	 *	Calendar stuff
	 *****************************************/
	int32 DayNumber{1};
	int32 MonthNumber{1};
	int32 Year{1};
	FName CurrentDayName{"Monday"};
	FName CurrentMonthName{"January"};
	TArray<FName> DayNames{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	TArray<FName> MonthNames{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


	/*******************************
	 *	Time stuff
	 *******************************/

	UPROPERTY(BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess, ClampMin = "0", ClampMax = "23", UIMin = "0", UIMax = "23"))
	uint8 CurrentHour{0};
	UPROPERTY(BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess, ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))
	uint8 CurrentMinute{0};
	UPROPERTY(BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess, ClampMin = "0.000", ClampMax = "59.999", UIMin = "0.000", UIMax = "59.999"))
	float CurrentSecond{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess))
	float TimeScaler{1.f};

public:

	FORCEINLINE int32 GetDayNumber() const {return DayNumber;}
	FORCEINLINE int32 GetMonthNumber() const {return MonthNumber;}
	FORCEINLINE int32 GetYear() const {return Year;}
	FORCEINLINE FName GetDayName() const {return CurrentDayName;}
	FORCEINLINE FName GetMonthName() const {return CurrentMonthName;}
	FORCEINLINE TArray<FName> GetDayNames() const {return DayNames;}
	FORCEINLINE TArray<FName> GetMonthNames() const {return MonthNames;}
	FORCEINLINE float GetCurrentSecond() const {return CurrentSecond;}
	FORCEINLINE uint8 GetCurrentMinute() const {return CurrentMinute;}
	FORCEINLINE uint8 GetCurrentHour() const {return CurrentHour;}
};

