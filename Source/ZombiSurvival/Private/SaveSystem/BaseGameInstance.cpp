// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/BaseGameInstance.h"

#include "EngineUtils.h"
#include "Interface/SavalableObjectInterface.h"
#include "Kismet/GameplayStatics.h"

UBaseGameInstance::UBaseGameInstance()
{
	SlotName = "Slot1";
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
		else
		{
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Health == 0"));
		}
	}
	else
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No player"));
	}
}

void UBaseGameInstance::Init()
{
	Super::Init();

	Execute_LoadGameData(this);
}

void UBaseGameInstance::AddDestroyedActor(AActor* DestroyedActor)
{
	CurrentSaveGame->ActorsToDestroy.Add(DestroyedActor);
}

void UBaseGameInstance::SaveGameData_Implementation()
{
	ASurvivalPlayer* PlayerCharacter = Cast<ASurvivalPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0.f));
	if (PlayerCharacter)
	{
		PlayerCharacter->SavePlayerStats(CurrentSaveGame);
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Save Player Stats"));
	}

	CurrentSaveGame->ItemSaveData.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{		
		AActor* Actor = *It;
		
		if (!IsValid(Actor) || !UKismetSystemLibrary::DoesImplementInterface(Actor, USavalableObjectInterface::StaticClass()))
		{
			//if(GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("!ActorIsValid or DontHaveInterface"));
			continue;
		}
		FItemSaveData ItemSaveData;
		ItemSaveData.ActorName = Actor->GetName();
		ItemSaveData.Transform = Actor->GetActorTransform();

		CurrentSaveGame->ItemSaveData.Add(ItemSaveData);
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Save Item"));
	}
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Save"));
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void UBaseGameInstance::LoadGameData_Implementation()
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("InitGame"));
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
					if(GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Destroy Item"));
					Actor->Destroy();
					break;
				}
			}

			for (FItemSaveData ItemSaveData : CurrentSaveGame->ItemSaveData)
			{
				if (ItemSaveData.ActorName == Actor->GetName())
				{
					if(GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Load Item"));
					Actor->SetActorTransform(ItemSaveData.Transform);
					break;
				}
			}
		}

		
	}
	else
	{
		CurrentSaveGame = Cast<UBaseSaveGame>(UGameplayStatics::CreateSaveGameObject(UBaseSaveGame::StaticClass()));
	}
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Close Init"));
}
