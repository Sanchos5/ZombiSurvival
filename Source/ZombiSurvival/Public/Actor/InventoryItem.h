// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseActor.h"
#include "Interface/SavalableObjectInterface.h"
#include "Structes/Slot.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBISURVIVAL_API AInventoryItem : public ABaseActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", SaveGame)
	FSlot Item;
};
