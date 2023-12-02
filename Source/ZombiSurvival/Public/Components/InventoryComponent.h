// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structes/AllItem.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;
class UUserWidget;

USTRUCT()
struct FNewStack
{
	GENERATED_BODY()

	UPROPERTY()
	int ArrayIndex = 0;

	UPROPERTY()
	bool Success = false;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBISURVIVAL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	void SetSizeForInventory();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	UInventoryWidget* InventoryWidget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FAllItem AllItems;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	UDataTable* ItemInfoDataTable;

	UFUNCTION(BlueprintCallable)
	bool AddToInventory(FSlot Item);
	bool AddItemToExcistingItem(FSlot Item, TArray<FSlot>& Array);
	bool CreateNewStack(FSlot Item,TArray<FSlot> &Array);

	// Update Slots in Inventory Widget

	UFUNCTION(BlueprintCallable)
	void UpdateMeleeWeaponUI();
	
	UFUNCTION(BlueprintCallable)
	void UpdateEatablesUI();
	
	UFUNCTION(BlueprintCallable)
	void UpdateRangeWeaponUI();

	UPROPERTY()
	bool bSuccess;
	
	template <typename T>
	static void SetArrayElement(T item, TArray<T>& item_array, int32 index)
	{
		if (item_array.Num() - 1 < index)
			item_array.SetNum(index);
		item_array[index] = item;
	}
};
