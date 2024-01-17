#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Slot.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EI_Patrons UMETA(DisplayName = "Patrons"),
	EI_Medicines UMETA(DisplayName = "Medicines"),
	EI_Drinks UMETA(DisplayName = "Drinks"),
	EI_Food UMETA(DisplayName = "Food"),
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
