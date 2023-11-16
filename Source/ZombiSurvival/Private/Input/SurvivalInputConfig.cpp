// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SurvivalInputConfig.h"
#include "InputMappingContext.h"

USurvivalInputConfig::USurvivalInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* USurvivalInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FSurvivalInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	return nullptr;
}