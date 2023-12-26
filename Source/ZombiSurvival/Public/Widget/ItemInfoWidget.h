// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structes/Slot.h"
#include "ItemInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBISURVIVAL_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Item")
	FSlot Item;
};
