// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSystem.generated.h"

class UBaseSaveGame;

UCLASS(meta=(DisplayName="SaveGameSystem"))
class ZOMBISURVIVAL_API USaveGameSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USaveGameSystem();
	void InitPlayerSavedData();

	UFUNCTION(BlueprintCallable)
	void AddDestroyedActor(AActor* DestroyedActor);

	/* Initialize Subsystem, good moment to load in SaveGameSettings variables */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	virtual void SaveGameData();

	UFUNCTION(BlueprintCallable)
	virtual void LoadGameData();

	UPROPERTY(BlueprintReadWrite)
	FString LastSaveGame;

	UFUNCTION(BlueprintCallable)
	void SetSlotName(FString Name);

	UFUNCTION(Exec, BlueprintCallable)
	static void TakeScreenShot(bool bCaptureUI, bool bAddSuffix);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBaseSaveGame> CurrentSaveGame;

	UPROPERTY()
	TArray<AActor*> ActorsToDestroy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Save Game")
	FString SlotName;
};
