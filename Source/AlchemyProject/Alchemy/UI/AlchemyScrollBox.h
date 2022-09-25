// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlchemyScrollBox.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYPROJECT_API UAlchemyScrollBox : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAlchemySelectionElement> SelectionElementClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAlchemySelectionElement* AlchemySelectionElement;

	void UpdateInfo(const TArray<FName>& InNames);

private:

	UPROPERTY(VisibleAnywhere)
	TArray<FName> RecipeNames;
};
