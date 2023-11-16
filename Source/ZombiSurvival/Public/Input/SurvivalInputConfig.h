// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SurvivalInputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * FSurvivalInputAction
 *
 *	Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FSurvivalInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * USurvivalInputConfig
 *
 *	Non-mutable data asset that contains input configuration properties.
 */

UCLASS(BlueprintType, Const)
class ZOMBISURVIVAL_API USurvivalInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:

	USurvivalInputConfig(const FObjectInitializer& ObjectInitializer);

	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;

public:
	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FSurvivalInputAction> NativeInputActions;
};
