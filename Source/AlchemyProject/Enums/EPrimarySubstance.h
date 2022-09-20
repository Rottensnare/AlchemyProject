#pragma once

//12 Primary substances
UENUM(BlueprintType)
enum class EPrimarySubstance : uint8
{
	EPS_Alpha UMETA(DisplayName = "Alpha"),
	EPS_Beta UMETA(DisplayName = "Beta"),
	EPS_Gamma UMETA(DisplayName = "Gamma"),
	EPS_Delta UMETA(DisplayName = "Delta"),
	EPS_Epsilon UMETA(DisplayName = "Epsilon"),
	EPS_Zeta UMETA(DisplayName = "Zeta"),
	EPS_Eta UMETA(DisplayName = "Eta"),
	EPS_Theta UMETA(DisplayName = "Theta"),
	EPS_Iota UMETA(DisplayName = "Iota"),
	EPS_Kappa UMETA(DisplayName = "Kappa"),
	EPS_Lambda UMETA(DisplayName = "Lambda"),
	EPS_Mu UMETA(DisplayName = "Mu"),
	EPS_None UMETA(DisplayName = "No Primary Substance"),
	
	EPS_MAX UMETA(DisplayName = "DefaultMax"),
};