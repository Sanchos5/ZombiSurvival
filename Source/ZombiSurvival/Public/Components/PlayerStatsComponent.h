// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/SurvivalPlayer.h"
#include "PlayerStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInfectionChangeDelegate, float, Infection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHungerChangeDelegate, float, Hunger, float, MaxHunger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnThirstChangeDelegate, float, Thirst, float, MaxThirst);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangeDelegate, float, Stamina, float, MaxStamina);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBISURVIVAL_API UPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	// Конструктор по умолчанию
	UPlayerStatsComponent();

	// Проверка на то инфецирован ли игрок
	bool Infected;

	float GetStamina() const { return Stamina; }

	// Восстановление голода
	UFUNCTION(BlueprintCallable)
	void IncrementHunger(float Value);

	// Восстановление жажды
	UFUNCTION(BlueprintCallable)
	void IncrementThirst(float Value);

	void DecrementStamina(float Value);

	void SprintingTimer(bool bIsRunning);

	// Таймер для состояний голода, жажды, инфекции
	FTimerHandle StaminaHandle;

	UPROPERTY(BlueprintAssignable)
	FOnInfectionChangeDelegate OnInfectionChange;

	UPROPERTY(BlueprintAssignable)
	FOnHungerChangeDelegate OnHungerChange;

	UPROPERTY(BlueprintAssignable)
	FOnThirstChangeDelegate OnThirstChange;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaChangeDelegate OnStaminaChange;

protected:

	// Начало игры
	virtual void BeginPlay() override; 

	// Таймер для состояний голода, жажды, инфекции
	FTimerHandle Handle;

	void HandleStats();

	void RegenerateStamina();

	// Инфекция
	void IncrementInfection(float Value); 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Infection; 

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float InfectionIncrementValue; 

	// Голод
	void DecrementHunger(float Value); 

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxHunger; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Hunger; 

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float HungerDecrementValue; 

	// Жажда
	void DecrementThirst(float Value);

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxThirst;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Thirst;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float ThirstDecrementValue;

	// Выносливость
	void IncrementStamina(float Value);
	
	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Stamina;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float StaminaIncrementValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bOneStats = false;
};
