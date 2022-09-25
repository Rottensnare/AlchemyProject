#pragma once
#include "Engine/DataTable.h"

#include "CustomDataTables.generated.h"

USTRUCT(BlueprintType)
struct FSoundEffectTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class USoundCue* SFX;
	
};
