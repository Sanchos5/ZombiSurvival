#pragma once

#include "CoreMinimal.h"
#include "Slot.h"
#include "Player/SurvivalPlayer.h"
#include "PlayerSaveData.generated.h"

USTRUCT(BlueprintType)
struct ZOMBISURVIVAL_API FPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Infection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Hunger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Thirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform PlayerTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator ControllerRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EActiveWeapon> ActiveWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHaveAxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHavePistol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHaveShotgun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentQuestIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;

	FPlayerSaveData();
	~FPlayerSaveData();
};
