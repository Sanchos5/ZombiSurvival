// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/BaseGameInstance.h"

#include "EngineUtils.h"
#include "Actor/InventoryItem.h"
#include "Interface/SavalableObjectInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SurvivalPlayer.h"
#include "SaveSystem/BaseSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

UBaseGameInstance::UBaseGameInstance()
{
	SlotName = "Slot1";
}

void UBaseGameInstance::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UBaseGameInstance::InitPlayerSavedData()
{
	ASurvivalPlayer* PlayerCharacter = Cast<ASurvivalPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0.f));
	if (PlayerCharacter)
	{
		if (CurrentSaveGame->PlayerSaveData.Health != 0.f)
		{
			PlayerCharacter->LoadPlayerStats(CurrentSaveGame);
		}
	}
}

void UBaseGameInstance::AddDestroyedActor(AActor* DestroyedActor)
{
	CurrentSaveGame->ActorsToDestroy.Add(DestroyedActor);
}

void UBaseGameInstance::SaveGameData()
{
	ASurvivalPlayer* PlayerCharacter = Cast<ASurvivalPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0.f));
	if (PlayerCharacter)
	{
		PlayerCharacter->SavePlayerStats(CurrentSaveGame);
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Save Player Stats in subsystem"));
	}

	CurrentSaveGame->ItemSaveData.Empty();

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
		

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ItemSaveData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->ItemSaveData.Add(ItemSaveData);
	}
	
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Save"));
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void UBaseGameInstance::LoadGameData()
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Load Start"));
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

			for (AActor* DestroyedActor: CurrentSaveGame->ActorsToDestroy)
			{
				if (DestroyedActor->GetName() == Actor->GetName())
				{
					Actor->Destroy();
					for (int32 i = 0; i < CurrentSaveGame->ItemSaveData.Num(); i++)
					{
						FItemSaveData ItemSaveData = CurrentSaveGame->ItemSaveData[i];
						if (DestroyedActor->GetName() == ItemSaveData.ActorName)
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

					const FString Msg = FString::Printf(TEXT("Actor Name: %s"), *ItemSaveData.ActorName);
					if(GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow, Msg);

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
		}
		if (!CurrentSaveGame->ItemSaveData.IsEmpty())
		{
			for (int32 i = 0; i < CurrentSaveGame->ItemSaveData.Num(); i++)
			{
				FItemSaveData ItemSaveData = CurrentSaveGame->ItemSaveData[i];
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemSaveData.ActorClass);
				if (SpawnedActor)
				{
					SpawnedActor->Rename(*ItemSaveData.ActorName);
					//const FString Msg = FString::Printf(TEXT("Actor Name: %s"), *ItemSaveData.ActorName);
					//if(GEngine)
					//	GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, Msg);
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
	}
	else
	{
		CurrentSaveGame = Cast<UBaseSaveGame>(UGameplayStatics::CreateSaveGameObject(UBaseSaveGame::StaticClass()));
	}
	
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Load"));
}
