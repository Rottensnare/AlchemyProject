#pragma once

#include "CoreMinimal.h"
#include "CustomAIStructs.generated.h"

USTRUCT(BlueprintType)
struct FEnvQueryActorContainer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> ActorContainer;
	
};

