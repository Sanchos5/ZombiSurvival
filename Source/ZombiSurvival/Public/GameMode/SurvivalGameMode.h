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
};
