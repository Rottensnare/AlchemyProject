#pragma once

UENUM(BlueprintType)
enum class ETertiarySubstance : uint8
{
	ETS_Hi UMETA(DisplayName = "Substance Hi"),		//Fire
	ETS_Koo UMETA(DisplayName = "Substance Koo"),   //Ice
	ETS_Rai UMETA(DisplayName = "Substance Rai"),	//Lightning
	ETS_Do UMETA(DisplayName = "Substance Do"),		//Earth
	ETS_Fuu UMETA(DisplayName = "Substance Fuu"),	//Wind
	
	ETS_MAX UMETA(DisplayName = "DefaultMax")
};