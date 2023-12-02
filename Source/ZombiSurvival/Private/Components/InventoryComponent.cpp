// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Structes/ItemInfo.h"
#include "Widget/InventoryWidget.h"

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

void UInventoryComponent::UpdateMeleeWeaponUI()
{
	InventoryWidget->UpdateItemsInInventoryUI(AllItems.MeleeWeapon, InventoryWidget->Wb_MeleeWeapons);
}

void UInventoryComponent::UpdateEatablesUI()
{
	InventoryWidget->UpdateItemsInInventoryUI(AllItems.Eatables, InventoryWidget->Wb_Eatables);
}

void UInventoryComponent::UpdateRangeWeaponUI()
{
	InventoryWidget->UpdateItemsInInventoryUI(AllItems.RangeWeapon, InventoryWidget->Wb_RangeWeapons);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(InventoryWidgetClass))
	{
		InventoryWidget = Cast<UInventoryWidget>(CreateWidget(GetWorld(), InventoryWidgetClass));
	}
	SetSizeForInventory();

	UpdateEatablesUI();
	UpdateRangeWeaponUI();
	UpdateMeleeWeaponUI();
}

bool UInventoryComponent::AddToInventory(FSlot Item)
{
	bSuccess = false;
	
	switch (Item.ItemType)
	{
	case EItemType::EI_MeleeWeapon:
		CreateNewStack(Item,AllItems.MeleeWeapon);
		return bSuccess;

	case EItemType::EI_RangeWeapon:
		CreateNewStack(Item,AllItems.RangeWeapon);
		return bSuccess;

	case EItemType::EI_Eatables:
		AddItemToExcistingItem(Item, AllItems.Eatables);
		return bSuccess;
	}
	return bSuccess;
}

bool UInventoryComponent::AddItemToExcistingItem(FSlot Item, TArray<FSlot>& Array)
{
	uint32 Len = Array.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		// Item Name that we picked up
		FName EatableName = Item.ItemID.RowName;

		// Item Name in inventory
		FName EatableInventoryName = Array[i].ItemID.RowName;

		if (EatableName == EatableInventoryName)
		{
			// Get Item Info in DataTable
			FItemInfo* ItemInfo = ItemInfoDataTable->FindRow<FItemInfo>(EatableName, "");
			// Add to Existing Stack of item new quantity
			int NewEatableQuantity = Array[i].Quantity + Item.Quantity;
			
			if (ItemInfo->StackSize >= NewEatableQuantity)
			{
				// Set Information in Slot
				FSlot ItemSlot;
				ItemSlot.ItemID = Item.ItemID;
				ItemSlot.ItemType = Item.ItemType;
				ItemSlot.Quantity = NewEatableQuantity;
				SetArrayElement( ItemSlot,AllItems.Eatables, i);
				bSuccess = true;
				return bSuccess;
			}
		}
	}
	CreateNewStack(Item, AllItems.Eatables);
	return bSuccess;
}

bool UInventoryComponent::CreateNewStack(FSlot Item, TArray<FSlot>& Array)
{
	uint32 Len = Array.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		if (Array[i].Quantity == 0)
		{
			SetArrayElement(Item, Array, i);
			bSuccess = true;
			return bSuccess;
		}
	}
	return bSuccess;
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

