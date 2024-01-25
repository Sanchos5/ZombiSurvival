#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/ItemType.h"
#include "Slot.generated.h"

USTRUCT(BlueprintType)
struct ZOMBISURVIVAL_API FSlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EItemType> ItemType;

	FSlot();
	~FSlot();
};
