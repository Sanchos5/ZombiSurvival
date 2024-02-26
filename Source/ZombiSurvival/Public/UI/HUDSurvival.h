// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDSurvival.generated.h"

class UPlayerInterface;
/**
 * 
 */
UCLASS()
class ZOMBISURVIVAL_API AHUDSurvival : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="UI")
	UPlayerInterface* PlayerInterface;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UPlayerInterface> PlayerInterfaceClass;
};
