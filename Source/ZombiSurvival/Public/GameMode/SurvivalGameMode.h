// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/SaveGameInterface.h"
#include "SurvivalGameMode.generated.h"

class UBaseSaveGame;

/**
 * 
 */
UCLASS()
class ZOMBISURVIVAL_API ASurvivalGameMode : public AGameModeBase, public ISaveGameInterface
{
	GENERATED_BODY()

protected:
	ASurvivalGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UPROPERTY(BlueprintReadOnly)
	UBaseSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Save Game")
	FString SlotName;
	
	virtual void SaveGameData_Implementation() override;
	virtual void LoadGameData_Implementation() override;
};
