// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Structes/ItemSaveData.h"
#include "Structes/PlayerSaveData.h"
#include "BaseSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FSaveChest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlot> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ChestName;
	
};

UCLASS()
class ZOMBISURVIVAL_API UBaseSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Save Data")
	FPlayerSaveData PlayerSaveData;
	
	UPROPERTY(BlueprintReadWrite, Category="Save Data")
	TArray<FItemSaveData> ItemSaveData;

	UPROPERTY(BlueprintReadWrite, Category="Save Data")
	TArray<FSaveChest> ChestSaveData;

	UPROPERTY(BlueprintReadWrite, Category="Save Data")
	TArray<FString> ActorsToDestroy;

	UPROPERTY(BlueprintReadWrite, Category="Save Data")
	TArray<uint8> ByteData;
	
	UPROPERTY(BlueprintReadWrite, Category="Save Data")
	FDateTime DateTime;
};
