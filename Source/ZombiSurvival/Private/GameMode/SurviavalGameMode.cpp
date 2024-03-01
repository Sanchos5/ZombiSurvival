// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SurvivalGameMode.h"
#include "EngineUtils.h"
#include "Interface/SavalableObjectInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SurvivalPlayer.h"
#include "SaveSystem/BaseSaveGame.h"

ASurvivalGameMode::ASurvivalGameMode()
{
	SlotName = "Slot1";
}

void ASurvivalGameMode::SaveGameData_Implementation()
{
	ASurvivalPlayer* PlayerCharacter = Cast<ASurvivalPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0.f));
	if (PlayerCharacter)
	{
		PlayerCharacter->SavePlayerStats(CurrentSaveGame);
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
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASurvivalGameMode::LoadGameData_Implementation()
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

void ASurvivalGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	Execute_LoadGameData(this);
}

void ASurvivalGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASurvivalPlayer* PlayerCharacter = Cast<ASurvivalPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0.f));
	if (PlayerCharacter && CurrentSaveGame->PlayerSaveData.Health != 0.f)
	{
		PlayerCharacter->LoadPlayerStats(CurrentSaveGame);
	}
}
