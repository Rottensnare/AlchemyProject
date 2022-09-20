// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyComponent.h"


UAlchemyComponent::UAlchemyComponent()
{

	PrimaryComponentTick.bCanEverTick = false;


}



void UAlchemyComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void UAlchemyComponent::CreateAlchemyProduct(const FAlchemyPackage& AlchemyPackage)
{
	FString TempString{""};
	TArray<EPrimarySubstance> PrimarySubstances;
	TMap<ESecondarySubstance, int32> SecondarySubstances;
	for(auto& InInfo : AlchemyPackage.IngredientInfos)
	{
		if(!SecondarySubstances.Contains(InInfo.SecondarySubstance)) SecondarySubstances.Emplace(InInfo.SecondarySubstance, 1);
		else SecondarySubstances[InInfo.SecondarySubstance]++;
	}
	for(auto& InInfo : AlchemyPackage.IngredientInfos)
	{
		PrimarySubstances.AddUnique(InInfo.PrimarySubstance);
		TempString.Append(FString::Printf(TEXT("%s \n"), *UEnum::GetDisplayValueAsText(InInfo.PrimarySubstance).ToString()));
	}
	UE_LOG(LogTemp, Warning, TEXT("Selected Substances: %s"), *TempString)
}


FAlchemyPackage UAlchemyComponent::CreateAlchemyPackage(const TArray<FIngredientInfo>& InIngredientInfos,
	AAlchemyBase* InAlchemyBase)
{
	FAlchemyPackage AlchemyPackage;
	AlchemyPackage.IngredientInfos = InIngredientInfos;
	AlchemyPackage.AlchemyBase = InAlchemyBase;
	return AlchemyPackage;
}


