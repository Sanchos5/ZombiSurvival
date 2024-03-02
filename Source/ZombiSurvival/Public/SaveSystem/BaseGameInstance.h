// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SaveGameInterface.h"
#include "BaseGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBISURVIVAL_API UBaseGameInstance : public UGameInstance, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	void InitPlayerSavedData();

	UFUNCTION(BlueprintCallable)
	void AddDestroyedActor(AActor* DestroyedActor);

protected:
	UBaseGameInstance();
	virtual void Init() override;

	UPROPERTY(BlueprintReadOnly)
	UBaseSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Save Game")
	FString SlotName;
	
	virtual void SaveGameData_Implementation() override;
	virtual void LoadGameData_Implementation() override;
};
