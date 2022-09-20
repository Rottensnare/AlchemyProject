#pragma once


UENUM(BlueprintType)
enum class ESecondarySubstance : uint8
{
	ESS_Albedo UMETA(DisplayName = "Albedo"),
	ESS_Nigredo UMETA(DisplayName = "Nigredo"),
	ESS_Citrinitas UMETA(DisplayName = "Citrinitas"),
	ESS_Rubedo UMETA(DisplayName = "Rubedo"),
	ESS_None UMETA(DisplayName = "No Secondary Substance"),
	
	EPS_MAX UMETA(DisplayName = "DefaultMax")
};