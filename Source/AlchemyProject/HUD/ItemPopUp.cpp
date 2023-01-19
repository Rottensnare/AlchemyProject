// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPopUp.h"

#include "Components/RichTextBlock.h"

void UItemPopUp::SetBlockText(const FString& ItemDescription) const
{
	RichTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s"), *ItemDescription)));
}
