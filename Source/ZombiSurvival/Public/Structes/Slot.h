#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Slot.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EI_MeleeWeapon UMETA(DisplayName = "Melee Weapon"),
	EI_RangeWeapon UMETA(DisplayName = "Range Weapon"),
	EI_Eatables UMETA(DisplayName = "Eatables"),
	NONE UMETA(DisplayName = "NONE")
};

USTRUCT(BlueprintType)
struct ZOMBISURVIVAL_API FSlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	FSlot();
	~FSlot();
};
