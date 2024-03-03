// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BaseGameInstance.generated.h"

class UBaseSaveGame;

UCLASS(meta=(DisplayName="SaveGame System"))
class ZOMBISURVIVAL_API UBaseGameInstance : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void InitPlayerSavedData();

	UFUNCTION(BlueprintCallable)
	void AddDestroyedActor(AActor* DestroyedActor);

	/* Initialize Subsystem, good moment to load in SaveGameSettings variables */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	virtual void SaveGameData();

	UFUNCTION(BlueprintCallable)
	virtual void LoadGameData();

protected:
	UBaseGameInstance();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBaseSaveGame> CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Save Game")
	FString SlotName;
};
