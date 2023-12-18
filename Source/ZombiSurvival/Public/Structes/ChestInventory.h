#pragma once

#include "CoreMinimal.h"
#include "Slot.h"
#include "ChestInventory.generated.h"

USTRUCT(BlueprintType)
struct FChestInventory
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> ChestInventory;

	FChestInventory();
	~FChestInventory();
};



