#pragma once

#include "CoreMinimal.h"
#include "Slot.h"
#include "AllItem.generated.h"

USTRUCT(BlueprintType)
struct FAllItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> MainInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> Patrons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> Medicine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> Drinks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> Food;

	FAllItem();
	~FAllItem();
};
