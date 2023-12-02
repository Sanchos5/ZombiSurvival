// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structes/Slot.h"
#include "SlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBISURVIVAL_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FSlot Item;

	UPROPERTY(BlueprintReadWrite)
	int Index;
};
