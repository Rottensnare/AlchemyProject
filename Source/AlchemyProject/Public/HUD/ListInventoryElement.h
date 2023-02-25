// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "ListInventoryElement.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UListInventoryElement : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class URichTextBlock* ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	URichTextBlock* ItemCount;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	URichTextBlock* ItemType;

	
};
