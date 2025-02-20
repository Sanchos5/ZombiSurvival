// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Structes/ItemInfo.h"
#include "Widget/InventoryWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InventorySize = 18.f;
}

void UInventoryComponent::SetSizeForInventory()
{
	AllItems.MainInventory.SetNum(InventorySize);
}

void UInventoryComponent::UpdateAllInventoryUI()
{
	if (!IsValid(InventoryWidget)) return;
	InventoryWidget->UpdateItemsInInventoryUI(AllItems.MainInventory, InventoryWidget->Wb_MainInventory);
	InventoryWidget->UpdateItemsInSortInventoryUI(AllItems.Patrons, InventoryWidget->Wb_Patrons);
	InventoryWidget->UpdateItemsInSortInventoryUI(AllItems.Drinks, InventoryWidget->Wb_Drinks);
	InventoryWidget->UpdateItemsInSortInventoryUI(AllItems.Food, InventoryWidget->Wb_Food);
	InventoryWidget->UpdateItemsInSortInventoryUI(AllItems.Medicine, InventoryWidget->Wb_Medicine);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	SetSizeForInventory();
}

bool UInventoryComponent::AddToInventory(FSlot Item)
{
	bSuccess = false;
	
	AddItemToExcistingItem(Item, AllItems.MainInventory);
	if (bSuccess == false)
	{
		UUserWidget* FullInventory = CreateWidget<UUserWidget>(GetWorld(), FullInventoryWidgetClass);
		if (IsValid(FullInventory))
		{
			FullInventory->AddToViewport();
		}
	}
	return bSuccess;
}

bool UInventoryComponent::AddItemToExcistingItem(FSlot Item, TArray<FSlot>& Array)
{
	uint32 Len = Array.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		// Item Name that we picked up
		FName ItemName = Item.ItemID.RowName;

		// Item Name in inventory
		FName InventoryName = Array[i].ItemID.RowName;

		// Get Item Info in DataTable
		FItemInfo* ItemInfo = ItemInfoDataTable->FindRow<FItemInfo>(ItemName, "");

		if (ItemName == InventoryName && Array[i].Quantity < ItemInfo->StackSize)
		{
			// Add to Existing Stack of item new quantity
			int NewItemQuantity = Array[i].Quantity + Item.Quantity;
			
			if (ItemInfo->StackSize >= NewItemQuantity)
			{
				// Set Information in Slot
				FSlot ItemSlot;
				ItemSlot.ItemID = Item.ItemID;
				ItemSlot.ItemType = Item.ItemType;
				ItemSlot.Quantity = NewItemQuantity;
				SetArrayElement( ItemSlot,Array, i);
				bSuccess = true;
				return bSuccess;
			}
			else
			{
				// Set Information in Slot
				int IntToFullStuck = ItemInfo->StackSize - Array[i].Quantity;
				NewItemQuantity = Array[i].Quantity + IntToFullStuck;
				Item.Quantity -= IntToFullStuck;
				FSlot ItemSlot;
				ItemSlot.ItemID = Item.ItemID;
				ItemSlot.ItemType = Item.ItemType;
				ItemSlot.Quantity = NewItemQuantity;
				SetArrayElement( ItemSlot,Array, i);
				CreateNewStack(Item, Array);
				bSuccess = true;
				return bSuccess;
			}
		}
	}
	CreateNewStack(Item, Array);
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

void UInventoryComponent::SortPatrons()
{
	SortItems(EItemType::EI_Patrons, AllItems.Patrons);
}

void UInventoryComponent::SortMedicine()
{
	SortItems(EItemType::EI_HealthMedicines, AllItems.Medicine);
	SortItems(EItemType::EI_InfectionMedicines, AllItems.Medicine);
}

void UInventoryComponent::SortDrinks()
{
	SortItems(EItemType::EI_Drinks, AllItems.Drinks);
}

void UInventoryComponent::SortFood()
{
	SortItems(EItemType::EI_Food, AllItems.Food);
}

void UInventoryComponent::SortItems(EItemType ItemType, TArray<FSlot>& Array)
{
	uint32 Len = AllItems.MainInventory.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		if (AllItems.MainInventory[i].ItemType == ItemType)
		{
			AddItemToExcistingItemSort(AllItems.MainInventory[i], Array);
		}
	}
	UpdateAllInventoryUI();
}

bool UInventoryComponent::AddItemToExcistingItemSort(FSlot Item, TArray<FSlot>& Array)
{
	uint32 Len = Array.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		// Item Name that we picked up
		FName ItemName = Item.ItemID.RowName;

		// Item Name in inventory
		FName ItemInventoryName = Array[i].ItemID.RowName;

		if (ItemName == ItemInventoryName)
		{
			// Get Item Info in DataTable
			FItemInfo* ItemInfo = ItemInfoDataTable->FindRow<FItemInfo>(ItemName, "");
			// Add to Existing Stack of item new quantity
			int NewItemQuantity = Array[i].Quantity + Item.Quantity;
			
			if (ItemInfo->StackSize >= NewItemQuantity)
			{
				// Set Information in Slot
				FSlot ItemSlot;
				ItemSlot.ItemID = Item.ItemID;
				ItemSlot.ItemType = Item.ItemType;
				ItemSlot.Quantity = NewItemQuantity;
				SetArrayElement( ItemSlot,Array, i);
				bSuccess = true;
				return bSuccess;
			}
		}
	}
	CreateNewStackSort(Item, Array);
	return bSuccess;
}

void UInventoryComponent::CreateNewStackSort(FSlot Item, TArray<FSlot>& Array)
{
	Array.Add(Item);
}
