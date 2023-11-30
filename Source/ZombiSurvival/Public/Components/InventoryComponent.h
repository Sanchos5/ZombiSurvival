// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structes/AllItem.h"
#include "InventoryComponent.generated.h"

class UUserWidget;

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

	UPROPERTY()
	UUserWidget* InventoryWidget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FAllItem AllItems;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	UDataTable* ItemInfoDataTable;

	UFUNCTION(BlueprintCallable)
	void AddToInventory(FSlot Item);
	
	template <typename T>
	static void SetArrayElement(T item, TArray<T>& item_array, int32 index)
	{
		if (index >= item_array.Num())
			item_array.SetNum(index + 1);
		item_array[index] = item;
	}
};
