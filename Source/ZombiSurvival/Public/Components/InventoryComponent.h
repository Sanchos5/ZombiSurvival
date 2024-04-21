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

	UFUNCTION(BlueprintPure, Category = "Inventory")
	static UInventoryComponent* FindInventoryComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UInventoryComponent>() : nullptr); }

	UInventoryComponent();
	void SetSizeForInventory();

	UPROPERTY(BlueprintReadWrite)
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FAllItem AllItems;

	// Update Slots in Inventory Widget	
	UFUNCTION(BlueprintCallable)
	void UpdateAllInventoryUI();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	float InventorySize;

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	UDataTable* ItemInfoDataTable;

	// Function for adding item to main inventory
	UFUNCTION(BlueprintCallable)
	bool AddToInventory(FSlot Item);
	bool AddItemToExcistingItem(FSlot Item, TArray<FSlot>& Array);
	bool CreateNewStack(FSlot Item,TArray<FSlot> &Array);
	// End //

	// Function for adding item to sorting inventory
	UFUNCTION(BlueprintCallable)
	void SortPatrons();

	UFUNCTION(BlueprintCallable)
	void SortMedicine();

	UFUNCTION(BlueprintCallable)
	void SortDrinks();

	UFUNCTION(BlueprintCallable)
	void SortFood();
	
	void SortItems(EItemType ItemType, TArray<FSlot>& Array);
	
	bool AddItemToExcistingItemSort(FSlot Item, TArray<FSlot>& Array);
	void CreateNewStackSort(FSlot Item,TArray<FSlot> &Array);
	// End //

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
