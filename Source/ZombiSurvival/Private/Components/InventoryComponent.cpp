// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
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

void UInventoryComponent::AddToInventory(FSlot Item)
{
	switch (Item.ItemType)
	{
	case EItemType::EI_MeleeWeapon:
		UE_LOG(LogTemp, Warning, TEXT("MeleeWeapon"))
		break;

	case EItemType::EI_RangeWeapon:
		break;

	case EItemType::EI_Eatables:

		int ArrayIndex = -1;
		for (FSlot EatableItem : AllItems.Eatables)
		{
			ArrayIndex += 1;
			
			// Item Name that we picked up
			FName EatableName = Item.ItemID.RowName;

			UE_LOG(LogTemp, Warning, TEXT("%d"), ArrayIndex)

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
				}
			}
		}
		break;
	}
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

