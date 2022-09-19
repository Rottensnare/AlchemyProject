#pragma once

//12 Primary substances
UENUM(BlueprintType)
enum class EPrimarySubstance : uint8
{
	EPS_Alpha UMETA(DisplayName = "Substance Alpha"),
	EPS_Beta UMETA(DisplayName = "Substance Beta"),
	EPS_Gamma UMETA(DisplayName = "Substance Gamma"),
	EPS_Delta UMETA(DisplayName = "Substance Delta"),
	EPS_Epsilon UMETA(DisplayName = "Substance Epsilon"),
	EPS_Zeta UMETA(DisplayName = "Substance Zeta"),
	EPS_Eta UMETA(DisplayName = "Substance Eta"),
	EPS_Theta UMETA(DisplayName = "Substance Theta"),
	EPS_Iota UMETA(DisplayName = "Substance Iota"),
	EPS_Kappa UMETA(DisplayName = "Substance Kappa"),
	EPS_Lambda UMETA(DisplayName = "Substance Lambda"),
	EPS_Mu UMETA(DisplayName = "Substance Mu"),
	EPS_None UMETA(DisplayName = "No Substance"),
	
	EPS_MAX UMETA(DisplayName = "DefaultMax"),
};