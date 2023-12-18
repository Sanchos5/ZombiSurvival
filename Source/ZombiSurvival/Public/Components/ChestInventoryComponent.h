// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structes/ChestInventory.h"
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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<UChestInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	float InventorySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FChestInventory ChestInventory;

	// Update Slots in Inventory Widget

	UFUNCTION(BlueprintCallable)
	void UpdateChestInventoryUI();

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerInventoryUI();

public:	


		
};
