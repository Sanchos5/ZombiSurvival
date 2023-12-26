// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ChestInventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SurvivalPlayer.h"
#include "Widget/ChestInventoryWidget.h"

UChestInventoryComponent::UChestInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InventorySize = 10.f;
}

void UChestInventoryComponent::SetSizeForInventory()
{
	ChestInventory.ChestInventory.SetNum(InventorySize);
}

void UChestInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(InventoryWidgetClass))
	{
		ChestInventoryWidget = Cast<UChestInventoryWidget>(CreateWidget(GetWorld(), InventoryWidgetClass));
		ChestInventoryWidget->ChestOwner = GetOwner();
	}
	
	SetSizeForInventory();
	UpdateChestInventoryUI();
}

void UChestInventoryComponent::UpdateChestInventoryUI()
{
	ChestInventoryWidget->UpdateItemsInInventoryUI(ChestInventory.ChestInventory, ChestInventoryWidget->Wb_ChestInventory);
}

void UChestInventoryComponent::UpdatePlayerInventoryUI()
{
	ASurvivalPlayer* Player = Cast<ASurvivalPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		ChestInventoryWidget->UpdatePlayerItemsInInventoryUI(Player->GetInventoryComponent()->AllItems.MainInventory, ChestInventoryWidget->Wb_PlayerInventory);
	}
}

void UChestInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

