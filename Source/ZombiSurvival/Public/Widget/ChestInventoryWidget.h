// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChestInventoryWidget.generated.h"

class USlotWidget;
class UWrapBox;

UCLASS()
class ZOMBISURVIVAL_API UChestInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UWrapBox* Wb_ChestInventory;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UWrapBox* Wb_PlayerInventory;

	UFUNCTION(BlueprintCallable)
	void UpdateItemsInInventoryUI(TArray<FSlot> AllItems, UWrapBox* WrapBox);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerItemsInInventoryUI(TArray<FSlot> AllItems, UWrapBox* WrapBox);

	UPROPERTY()
	AActor* ChestOwner;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<USlotWidget> ChestSlotWidgetClass;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<USlotWidget> PlayerSlotWidgetClass;
};
