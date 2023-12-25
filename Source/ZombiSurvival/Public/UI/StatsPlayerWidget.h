// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "StatsPlayerWidget.generated.h"

class UProgressBar;
//class ASurvivalBaseCharacter;
/**
 * 
 */

UENUM(BlueprintType)
enum class EStatsType : uint8
{
	none,
	Infection,
	Health,
	Hunger,
	Thirst,
	Stamina,
};

UCLASS(Abstract)
class ZOMBISURVIVAL_API UStatsPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void UpdateProgressBar();

	TWeakObjectPtr<class ASurvivalPlayer> HealthBar;

	TWeakObjectPtr<class UPlayerStatsComponent> PlayerBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* StatsBar;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	EStatsType EStatsType = EStatsType::none;
};
