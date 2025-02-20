// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structes/AllItem.h"
#include "Player/SurvivalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "ChestInventoryComponent.generated.h"

class UChestInventoryWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBISURVIVAL_API UChestInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChestInventoryComponent();
	void SetSizeForInventory();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	UChestInventoryWidget* ChestInventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FAllItem AllChestItem;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,  Category="Widget")
	TSubclassOf<UChestInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	float InventorySize;

	// Update Slots in Inventory Widget

	UFUNCTION(BlueprintCallable)
	void UpdateChestInventoryUI();

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerInventoryUI();
		
};
