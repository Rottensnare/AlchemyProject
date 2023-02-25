// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ListInventoryBox.h"

#include "Components/ListView.h"
#include "Components/RichTextBlock.h"
#include "HUD/ListInventoryElement.h"

void UListInventoryBox::SortList(const ESortingType SortingType, const bool bInverse)
{
	switch (SortingType)
	{
	case ESortingType::EST_Name:
		if(bInverse)
		{
			Algo::SortBy(InventoryElements, [](const UListInventoryElement* A)
			{
				if(A && A->ItemName) return A->ItemName->GetText().ToString();
				else return FString();
			},TGreater<>());
		}
		else
		{
			Algo::SortBy(InventoryElements, [](const UListInventoryElement* A)
			{
				if(A && A->ItemName) return A->ItemName->GetText().ToString();
				else return FString();
			});
		}
		break;
	case ESortingType::EST_Amount:
		if(bInverse)
		{
			Algo::SortBy(InventoryElements, [](const UListInventoryElement* A)
			{
				if(A && A->ItemCount) return FCString::Atoi(*A->ItemCount->GetText().ToString());
				else return 0;
			},TGreater<>());
		}
		else
		{
			Algo::SortBy(InventoryElements, [](const UListInventoryElement* A)
			{
				if(A && A->ItemCount) return FCString::Atoi(*A->ItemCount->GetText().ToString());
				else return 0;
			});
		}
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Selected SortintType hasn't been implemented."))
		break;
	}

	InventoryList->ClearListItems();
	InventoryList->SetListItems(InventoryElements);
}
