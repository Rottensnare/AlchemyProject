// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionComponent.h"

#include "AlchemyProject/PlayerCharacter.h"


UPotionComponent::UPotionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	
}


void UPotionComponent::BeginPlay()
{
	if(QualityCurve == nullptr) QualityCurve = GetCurveFromDataTable();
	
	Super::BeginPlay();
}

void UPotionComponent::ExecuteFunctionality()
{
	
}

void UPotionComponent::DestroyThisComponent()
{
	DestroyComponent();
}

void UPotionComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	CurrentCharacter = CurrentCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : CurrentCharacter;
	if(CurrentCharacter)
	{
		CurrentCharacter->CurrentPotionComponents.Remove(this);
	}
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

UCurveFloat* UPotionComponent::GetCurveFromDataTable()
{
	static const FString PathName = TEXT("DataTable'/Game/Assets/Datatables/PotionQualityTable.PotionQualityTable'");
	const UDataTable* PotionDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *PathName));
	if(PotionDataTable)
	{
		const FQualityCurve* TempQualityCurve = PotionDataTable->FindRow<FQualityCurve>(PotionName, "");
		if(TempQualityCurve) return TempQualityCurve->FloatCurve;
	}
	return nullptr;
}


void UPotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

