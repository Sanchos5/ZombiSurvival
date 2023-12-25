// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Structes/ItemInfo.h"
#include "Widget/InventoryWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InventorySize = 18.f;
}

void UInventoryComponent::SetSizeForInventory()
{
	AllItems.MainInventory.SetNum(InventorySize);
}

void UInventoryComponent::UpdateMainInventoryUI()
{
	InventoryWidget->UpdateItemsInInventoryUI(AllItems.MainInventory, InventoryWidget->Wb_MainInventory);
}

void UInventoryComponent::UpdateMeleeWeaponUI()
{
	InventoryWidget->UpdateItemsInSortInventoryUI(AllItems.MeleeWeapon, InventoryWidget->Wb_MeleeWeapons);
}

void UInventoryComponent::UpdateEatablesUI()
{
	InventoryWidget->UpdateItemsInSortInventoryUI(AllItems.Eatables, InventoryWidget->Wb_Eatables);
}

void UInventoryComponent::UpdateRangeWeaponUI()
{
	InventoryWidget->UpdateItemsInSortInventoryUI(AllItems.RangeWeapon, InventoryWidget->Wb_RangeWeapons);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(InventoryWidgetClass))
	{
		InventoryWidget = Cast<UInventoryWidget>(CreateWidget(GetWorld(), InventoryWidgetClass));
	}
	
	SetSizeForInventory();

	UpdateMainInventoryUI();
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
		AddItemToExcistingItem(Item, AllItems.MainInventory);
		return bSuccess;

	case EItemType::EI_RangeWeapon:
		AddItemToExcistingItem(Item, AllItems.MainInventory);
		return bSuccess;

	case EItemType::EI_Eatables:
		AddItemToExcistingItem(Item, AllItems.MainInventory);
		return bSuccess;
	
	case EItemType::NONE:
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
		FName ItemName = Item.ItemID.RowName;

		// Item Name in inventory
		FName EatableInventoryName = Array[i].ItemID.RowName;

		if (ItemName == EatableInventoryName)
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

void UInventoryComponent::SortMeleeItem()
{
	uint32 Len = AllItems.MainInventory.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		if (AllItems.MainInventory[i].ItemType == EItemType::EI_MeleeWeapon)
		{
			CreateNewStackSort(AllItems.MainInventory[i], AllItems.MeleeWeapon);
			UE_LOG(LogTemp, Warning, TEXT("Add TO Melee"))
		}
		UpdateEatablesUI();
		UpdateRangeWeaponUI();
		UpdateMeleeWeaponUI();
	}
}

void UInventoryComponent::SortRangeItem()
{
	uint32 Len = AllItems.MainInventory.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		if (AllItems.MainInventory[i].ItemType == EItemType::EI_RangeWeapon)
		{
			CreateNewStackSort(AllItems.MainInventory[i], AllItems.RangeWeapon);
			UE_LOG(LogTemp, Warning, TEXT("Add TO Range"))
		}
		UpdateEatablesUI();
		UpdateRangeWeaponUI();
		UpdateMeleeWeaponUI();
	}
}

void UInventoryComponent::SortEatablesItem()
{
	uint32 Len = AllItems.MainInventory.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		if (AllItems.MainInventory[i].ItemType == EItemType::EI_Eatables)
		{
			AddItemToExcistingItemSort(AllItems.MainInventory[i], AllItems.Eatables);
			UE_LOG(LogTemp, Warning, TEXT("Add TO Eatables"))
		}
		UpdateEatablesUI();
		UpdateRangeWeaponUI();
		UpdateMeleeWeaponUI();
	}
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

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

