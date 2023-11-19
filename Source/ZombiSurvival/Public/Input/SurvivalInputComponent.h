// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SurvivalInputConfig.h"
#include "GameplayTagContainer.h"
#include "SurvivalInputComponent.generated.h"

/**
 * 
 */
UCLASS(config = input)
class ZOMBISURVIVAL_API USurvivalInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	USurvivalInputComponent(const FObjectInitializer& ObjectInitializer);

	template<class UserClass, typename FuncType>
	void BindNativeAction(const USurvivalInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	void AddInputMappings(const USurvivalInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
};

template<class UserClass, typename FuncType>
void USurvivalInputComponent::BindNativeAction(const USurvivalInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}