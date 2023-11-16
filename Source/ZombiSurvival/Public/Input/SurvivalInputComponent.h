// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SurvivalInputConfig.h"
#include "GameplayTagContainer.h"
#include "SurvivalInputComponent.generated.h"

//class UPlayerMappableInputConfig;

/** A container to organize loaded player mappable configs to their CommonUI input type */
//USTRUCT(BlueprintType)
//struct FLoadedMappableConfigPair
//{
//	GENERATED_BODY()
//
//	FLoadedMappableConfigPair() = default;
//	FLoadedMappableConfigPair(const UPlayerMappableInputConfig* InConfig, const bool InIsActive)
//		: Config(InConfig)
//		, bIsActive(InIsActive)
//	{}
//
//	/** The player mappable input config that should be applied to the Enhanced Input subsystem */
//	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
//	const UPlayerMappableInputConfig* Config = nullptr;
//
//	/** The type of device that this mapping config should be applied to */
//	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
//	//ECommonInputType Type = ECommonInputType::Count;
//
//	/** If this config is currently active. A config is marked as active when it's owning GFA is active */
//	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
//	bool bIsActive = false;
//};

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
	//void RemoveInputMappings(const USurvivalInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

	//void AddInputConfig(const FLoadedMappableConfigPair& ConfigPair, UEnhancedInputLocalPlayerSubsystem* InputSubsystem);
	//void RemoveInputConfig(const FLoadedMappableConfigPair& ConfigPair, UEnhancedInputLocalPlayerSubsystem* InputSubsystem);
};

template<class UserClass, typename FuncType>
void USurvivalInputComponent::BindNativeAction(const USurvivalInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	//check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}