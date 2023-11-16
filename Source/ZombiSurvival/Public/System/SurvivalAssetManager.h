// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SurvivalAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBISURVIVAL_API USurvivalAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	USurvivalAssetManager();

	// Returns the AssetManager singleton object.
	static USurvivalAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
