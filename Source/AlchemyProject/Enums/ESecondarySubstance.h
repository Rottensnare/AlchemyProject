#pragma once


UENUM(BlueprintType)
enum class ESecondarySubstance : uint8
{
	ESS_Albedo UMETA(DisplayName = "Substance Albedo"),
	ESS_Nigredo UMETA(DisplayName = "Substance Nigredo"),
	ESS_Citrinitas UMETA(DisplayName = "Substance Citrinitas"),
	ESS_Rubedo UMETA(DisplayName = "Substance Rubedo"),

	
	EPS_MAX UMETA(DisplayName = "DefaultMax"),
};