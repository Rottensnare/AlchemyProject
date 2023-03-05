// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemyProject/Alchemy/CustomStructs/NPCStructs.h"
#include "UObject/Interface.h"
#include "BaseCharacterInfo.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UBaseCharacterInfo : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALCHEMYPROJECT_API IBaseCharacterInfo
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	
	virtual FNPCInfo& GetNPCInfo();
	virtual EPhysicalSurface GetFootStepSurfaceType();
	virtual class UHealthComponent* GetHealthComp();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAnimMontage* GetMeleeMontage();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FName> GetMeleeSections();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FName> GetHitReactSections();
	

private:

	FNPCInfo DummyInfo;
};
