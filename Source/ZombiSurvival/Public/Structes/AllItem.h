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
	TArray<FSlot> MeleeWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> RangeWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> Eatables;

	FAllItem();
	~FAllItem();
};
