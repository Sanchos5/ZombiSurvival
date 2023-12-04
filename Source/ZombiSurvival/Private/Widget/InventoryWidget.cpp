// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InventoryWidget.h"
#include "Components/WrapBox.h"
#include "Structes/Slot.h"
#include "Widget/SlotWidget.h"

void UInventoryWidget::UpdateItemsInInventoryUI(TArray<FSlot>& AllItems, UWrapBox* WrapBox)
{
	if (!IsValid(SlotWidgetClass)) return;
	
	WrapBox->ClearChildren();

	uint32 Len = AllItems.Num();
	for (uint32 i = 0; i < Len; ++i)
	{
		USlotWidget* WidgetSlotref = Cast<USlotWidget>(CreateWidget(GetWorld(), SlotWidgetClass));
		if (IsValid(WidgetSlotref))
		{
			WrapBox->AddChildToWrapBox(WidgetSlotref);
			WidgetSlotref->Index = i;
			WidgetSlotref->Item = AllItems[i];
		}
	}
}