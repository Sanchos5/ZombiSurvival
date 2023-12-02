// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class USlotWidget;
class UWrapBox;
/**
 * 
 */
UCLASS()
class ZOMBISURVIVAL_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UWrapBox* Wb_MeleeWeapons;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UWrapBox* Wb_RangeWeapons;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UWrapBox* Wb_Eatables;

	UFUNCTION(BlueprintCallable)
	void UpdateItemsInInventoryUI(TArray<FSlot>& AllItems, UWrapBox* WrapBox);

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<USlotWidget> SlotWidgetClass;
};
