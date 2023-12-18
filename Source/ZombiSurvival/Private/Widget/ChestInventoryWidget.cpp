// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ChestInventoryWidget.h"

#include "Components/WrapBox.h"
#include "Widget/SlotWidget.h"

void UChestInventoryWidget::UpdateItemsInInventoryUI(TArray<FSlot> AllItems, UWrapBox* WrapBox)
{
	if (!IsValid(ChestSlotWidgetClass)) return;
	
	WrapBox->ClearChildren();

	uint32 Len = AllItems.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		USlotWidget* WidgetSlotref = Cast<USlotWidget>(CreateWidget(GetWorld(), ChestSlotWidgetClass));
		if (IsValid(WidgetSlotref))
		{
			WidgetSlotref->Index = i;
			WidgetSlotref->Item = AllItems[i];
			WidgetSlotref->Owner = ChestOwner;
			WrapBox->AddChildToWrapBox(WidgetSlotref);
		}
	}
}

void UChestInventoryWidget::UpdatePlayerItemsInInventoryUI(TArray<FSlot> AllItems, UWrapBox* WrapBox)
{
	if (!IsValid(PlayerSlotWidgetClass)) return;
	
	WrapBox->ClearChildren();

	uint32 Len = AllItems.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		USlotWidget* WidgetSlotref = Cast<USlotWidget>(CreateWidget(GetWorld(), PlayerSlotWidgetClass));
		if (IsValid(WidgetSlotref))
		{
			WidgetSlotref->Index = i;
			WidgetSlotref->Item = AllItems[i];
			WidgetSlotref->Owner = ChestOwner;
			WrapBox->AddChildToWrapBox(WidgetSlotref);
		}
	}
}
