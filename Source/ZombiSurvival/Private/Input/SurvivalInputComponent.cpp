// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SurvivalInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"

USurvivalInputComponent::USurvivalInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void USurvivalInputComponent::AddInputMappings(const USurvivalInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);
}