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

	//ULyraLocalPlayer* LocalPlayer = InputSubsystem->GetLocalPlayer<ULyraLocalPlayer>();
	//check(LocalPlayer);
}

//void USurvivalInputComponent::AddInputConfig(const FLoadedMappableConfigPair& ConfigPair, UEnhancedInputLocalPlayerSubsystem* InputSubsystem)
//{
//	check(InputSubsystem);
//	if (ensure(ConfigPair.bIsActive))
//	{
//		InputSubsystem->AddPlayerMappableConfig(ConfigPair.Config);
//	}
//}
//
//void USurvivalInputComponent::RemoveInputConfig(const FLoadedMappableConfigPair& ConfigPair, UEnhancedInputLocalPlayerSubsystem* InputSubsystem)
//{
//	check(InputSubsystem);
//	if (!ConfigPair.bIsActive)
//	{
//		InputSubsystem->AddPlayerMappableConfig(ConfigPair.Config);
//	}
//}