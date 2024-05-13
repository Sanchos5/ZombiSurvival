// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGameSystem.h"
#include "EngineUtils.h"
#include "Actor/InventoryItem.h"
#include "Components/ChestInventoryComponent.h"
#include "Interface/SavalableObjectInterface.h"
#include "Interface/SaveChestInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/SurvivalPlayer.h"
#include "SaveSystem/BaseSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

USaveGameSystem::USaveGameSystem()
{
}

void USaveGameSystem::SetSlotName(FString Name)
{
	SlotName = Name;
}

void USaveGameSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CurrentSaveGame = Cast<UBaseSaveGame>(UGameplayStatics::CreateSaveGameObject(UBaseSaveGame::StaticClass()));
	LastSaveGame = Cast<UBaseSaveGame>(UGameplayStatics::CreateSaveGameObject(UBaseSaveGame::StaticClass()));

	USaveGameSystem* SaveGameSystem = GetGameInstance()->GetSubsystem<USaveGameSystem>();
	if (SaveGameSystem)
	{
		SlotName = "LastSave";
		if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
		{
			LastSaveGame = Cast<UBaseSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
			if (LastSaveGame == nullptr)
			{
				return;
			}
			FMemoryReader MemReader(LastSaveGame->ByteData);
			FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
			Ar.ArIsSaveGame = true;
			SaveGameSystem->Serialize(Ar);
		}
	}
}

void USaveGameSystem::InitPlayerSavedData()
{
	ASurvivalPlayer* PlayerCharacter = Cast<ASurvivalPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0.f));
	if (PlayerCharacter && CurrentSaveGame != nullptr)
	{
		if (CurrentSaveGame->PlayerSaveData.Health != 0.f)
		{
			PlayerCharacter->LoadPlayerStats(CurrentSaveGame);
		}
	}
}

void USaveGameSystem::AddDestroyedActor(FString DestroyedActor)
{
	CurrentSaveGame->ActorsToDestroy.Add(DestroyedActor);
}

void USaveGameSystem::SaveGameData()
{
	CurrentSaveGame->ItemSaveData.Empty();
	CurrentSaveGame->ChestSaveData.Empty();

	ASurvivalPlayer* PlayerCharacter = Cast<ASurvivalPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0.f));
	if (PlayerCharacter)
	{
		PlayerCharacter->SavePlayerStats(CurrentSaveGame);
	}

	for (FActorIterator It(GetWorld()); It; ++It)
	{		
		AActor* Actor = *It;
		
		if (!IsValid(Actor) || !UKismetSystemLibrary::DoesImplementInterface(Actor, USavalableObjectInterface::StaticClass()))
		{
			continue;
		}
		FItemSaveData ItemSaveData;
		ItemSaveData.ActorName = Actor->GetName();
		ItemSaveData.Transform = Actor->GetActorTransform();
		ItemSaveData.ActorClass = Actor->GetClass();

		AInventoryItem* ItemObject = Cast<AInventoryItem>(Actor);
		if (ItemObject)
		{
			ItemSaveData.Item = ItemObject->Item;
		}

		if (UKismetSystemLibrary::DoesImplementInterface(Actor, USaveChestInterface::StaticClass()))
		{
			UChestInventoryComponent* ChestInventory = Cast<UChestInventoryComponent>(Actor->GetComponentByClass(UChestInventoryComponent::StaticClass()));
			FSaveChest ChestSaveData;
			ChestSaveData.Inventory = ChestInventory->AllChestItem.MainInventory;
			ChestSaveData.ChestName = Actor->GetName();
			CurrentSaveGame->ChestSaveData.Add(ChestSaveData);
		}
		
		
		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ItemSaveData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->ItemSaveData.Add(ItemSaveData);
		
	}

	CurrentSaveGame->DateTime = UKismetMathLibrary::Now();
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);

	USaveGameSystem* SaveGameSystem = GetGameInstance()->GetSubsystem<USaveGameSystem>();
	if (SaveGameSystem)
	{
		SaveGameSystem->LastSaveGameName = SlotName;
		FMemoryWriter MemWriter(LastSaveGame->ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;
		SaveGameSystem->Serialize(Ar);;
		UGameplayStatics::SaveGameToSlot(LastSaveGame, "LastSave", 0);
	}
}

void USaveGameSystem::LoadGameData()
{
	
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		
		CurrentSaveGame = Cast<UBaseSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			return;
		}
		
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;			
			
			if (!IsValid(Actor) || !UKismetSystemLibrary::DoesImplementInterface(Actor, USavalableObjectInterface::StaticClass()))
			{
				continue;
			}

			for (FString DestroyedActorName : CurrentSaveGame->ActorsToDestroy)
			{
				
				if (DestroyedActorName == Actor->GetName())
				{
					Actor->Destroy();
					for (int32 i = 0; i < CurrentSaveGame->ItemSaveData.Num(); i++)
					{
						FItemSaveData ItemSaveData = CurrentSaveGame->ItemSaveData[i];
						if (DestroyedActorName == ItemSaveData.ActorName)
						{
							CurrentSaveGame->ItemSaveData.RemoveAt(i);
							break;
						}
					}
					break;
				}
			}

			for (int32 i = 0; i < CurrentSaveGame->ItemSaveData.Num(); i++)
			{
				FItemSaveData ItemSaveData = CurrentSaveGame->ItemSaveData[i];
				
				if (ItemSaveData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ItemSaveData.Transform);

					FMemoryReader MemReader(ItemSaveData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);

					ISavalableObjectInterface::Execute_OnActorLoaded(Actor);
					CurrentSaveGame->ItemSaveData.RemoveAt(i);
					break;
				}
			}

			for (int32 i = 0; i < CurrentSaveGame->ChestSaveData.Num(); i++)
			{
				FSaveChest ChestSaveData = CurrentSaveGame->ChestSaveData[i];
				if (ChestSaveData.ChestName == Actor->GetName())
				{
					if (UKismetSystemLibrary::DoesImplementInterface(Actor, USaveChestInterface::StaticClass()))
					{
						UChestInventoryComponent* ChestInventory = Cast<UChestInventoryComponent>(Actor->GetComponentByClass(UChestInventoryComponent::StaticClass()));
						ChestInventory->AllChestItem.MainInventory = CurrentSaveGame->ChestSaveData[i].Inventory;
						break;
					}
				}
			}
		}
		if (!CurrentSaveGame->ItemSaveData.IsEmpty())
		{
			for (int32 i = 0; i < CurrentSaveGame->ItemSaveData.Num(); i++)
			{
				FItemSaveData ItemSaveData = CurrentSaveGame->ItemSaveData[i];
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemSaveData.ActorClass);
				if (SpawnedActor)
				{
					SpawnedActor->SetActorTransform(ItemSaveData.Transform);

					FMemoryReader MemReader(ItemSaveData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					SpawnedActor->Serialize(Ar);

					AInventoryItem* ItemObject = Cast<AInventoryItem>(SpawnedActor);
					if (ItemObject)
					{
						ItemObject->Item = ItemSaveData.Item;
					}

					ISavalableObjectInterface::Execute_OnActorLoaded(SpawnedActor);
				}
			}
		}
		OnLoadDataEnd.Broadcast(true);
	}
	else
	{
		CurrentSaveGame = Cast<UBaseSaveGame>(UGameplayStatics::CreateSaveGameObject(UBaseSaveGame::StaticClass()));
		
	}
}
