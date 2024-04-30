// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadDataEnd, bool, bLoad);

class UBaseSaveGame;

UCLASS(meta=(DisplayName="SaveGameSystem"))
class ZOMBISURVIVAL_API USaveGameSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USaveGameSystem();
	void InitPlayerSavedData();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnLoadDataEnd OnLoadDataEnd;

	UFUNCTION(BlueprintCallable)
	void AddDestroyedActor(FString DestroyedActor);

	/* Initialize Subsystem, good moment to load in SaveGameSettings variables */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	virtual void SaveGameData();

	UFUNCTION(BlueprintCallable)
	virtual void LoadGameData();

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FString LastSaveGameName;

	UPROPERTY(BlueprintReadWrite)
	bool bSaveGame;

	UFUNCTION(BlueprintCallable)
	void SetSlotName(FString Name);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBaseSaveGame> CurrentSaveGame;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBaseSaveGame> LastSaveGame;

	UPROPERTY()
	TArray<AActor*> ActorsToDestroy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Save Game")
	FString SlotName;
};
