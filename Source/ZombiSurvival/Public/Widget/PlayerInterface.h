// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInterface.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBISURVIVAL_API UPlayerInterface : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	UUserWidget* PatronsBar;
};
