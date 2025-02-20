// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/SurvivalPlayer.h"
#include "SurvivalCharMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBISURVIVAL_API USurvivalCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	virtual float GetMaxSpeed() const override;
};
