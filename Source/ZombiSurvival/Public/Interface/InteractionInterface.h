// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBISURVIVAL_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool HideWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HighlightActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnHighlightActor();
};
