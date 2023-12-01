// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "PrimitiveSceneInfo.h"
#include "Blueprint/UserWidget.h"
#include "Structes/ItemInfo.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInventoryComponent::SetSizeForInventory()
{
	AllItems.MeleeWeapon.SetNum(4);
	AllItems.RangeWeapon.SetNum(3);
	AllItems.Eatables.SetNum(15);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(InventoryWidgetClass))
	{
		InventoryWidget = CreateWidget(GetWorld(), InventoryWidgetClass);
	}
	SetSizeForInventory();
}

bool UInventoryComponent::AddToInventory(FSlot Item)
{
	bool Success = false;
	
	switch (Item.ItemType)
	{
	case EItemType::EI_MeleeWeapon:
		UE_LOG(LogTemp, Warning, TEXT("MeleeWeapon"))
		return Success;

	case EItemType::EI_RangeWeapon:
		return Success;

	case EItemType::EI_Eatables:
		AddItemToExcistingItem(Success, Item);
		return true;
	}
	return Success;
}

bool UInventoryComponent::AddItemToExcistingItem(bool Success, FSlot Item)
{
	int ArrayIndex = -1;
	for (FSlot EatableItem : AllItems.Eatables)
	{
		ArrayIndex += 1;
			
		// Item Name that we picked up
		FName EatableName = Item.ItemID.RowName;

		// Item Name in inventory
		FName EatableInventoryName = EatableItem.ItemID.RowName;

		if (EatableName == EatableInventoryName)
		{
			// Get Item Info in DataTable
			FItemInfo* ItemInfo = ItemInfoDataTable->FindRow<FItemInfo>(EatableName, "");
			// Add to Existing Stack of item new quantity
			int NewEatableQuantity = EatableItem.Quantity + Item.Quantity;
			
			if (ItemInfo->StackSize >= NewEatableQuantity)
			{
				FSlot ItemSlot;
				ItemSlot.ItemID = Item.ItemID;
				ItemSlot.ItemType = Item.ItemType;
				ItemSlot.Quantity = NewEatableQuantity;
				SetArrayElement( ItemSlot,AllItems.Eatables, ArrayIndex);
				Success = true;
				return Success;
			}
		}
	}
	CreateNewStack(Success, Item);
	return Success;
}

bool UInventoryComponent::CreateNewStack(bool Success, FSlot Item)
{
	for (FSlot EatableItem : AllItems.Eatables)
	{
		if (EatableItem.Quantity == 0 && ArraySlotIndex < AllItems.Eatables.Num())
		{
			ArraySlotIndex += 1;
			SetArrayElement(Item,AllItems.Eatables, ArraySlotIndex);
			Success = true;
			return Success;
		}
	}
	Success = false;
	return Success;
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

