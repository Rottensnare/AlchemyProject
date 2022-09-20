#pragma once

UENUM(BlueprintType)
enum class ETertiarySubstance : uint8
{
	//Fire
	ETS_Hi UMETA(DisplayName = "Hi"),		//Ice
	ETS_Koo UMETA(DisplayName = "Koo"),		//Lightning
	ETS_Rai UMETA(DisplayName = "Rai"),		//Earth
	ETS_Do UMETA(DisplayName = "Do"),		//Wind
	ETS_Fuu UMETA(DisplayName = "Fuu"),		//None
	ETS_None UMETA(DisplayName = "No Tertiary Substance"),	
	
	ETS_MAX UMETA(DisplayName = "DefaultMax")
};