// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStatsComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Player/SurvivalBaseCharacter.h"

// Конструктор по умолчанию
UPlayerStatsComponent::UPlayerStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	//
	Infected = false;
	InfectionIncrementValue = 6.0f;
	
	if(Hunger > MaxHunger)
	{
		Hunger = MaxHunger;
	}

	MaxHunger = 100.0f;
	Hunger = 70.0f;
	HungerDecrementValue = 8.0f;

	if (Thirst > MaxThirst)
	{
		Thirst = MaxThirst;
	}

	MaxThirst = 100.0f;
	Thirst = 50.0f;
	ThirstDecrementValue = 9.0f;

	if (Stamina > MaxStamina)
	{
		Stamina = MaxStamina;
	}

	MaxStamina = 100.0f;
	Stamina = 50.0f;
	StaminaIncrementValue = 1.0f;
}


// Начало игры
void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	OnInfectionChange.Broadcast(Infection);
	OnHungerChange.Broadcast(Hunger, MaxHunger);
	OnThirstChange.Broadcast(Thirst, MaxThirst);
	OnStaminaChange.Broadcast(Stamina, MaxStamina);

	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UPlayerStatsComponent::HandleStats, 1.0f, true);

	GetWorld()->GetTimerManager().SetTimer(StaminaHandle, this, &UPlayerStatsComponent::RegenerateStamina, 0.2f, true);
}

void UPlayerStatsComponent::SprintingTimer(bool bIsRunning)
{
	if(bIsRunning == true)
	{
		GetWorld()->GetTimerManager().PauseTimer(StaminaHandle);
	}
	else if(bIsRunning == false)
	{
		GetWorld()->GetTimerManager().UnPauseTimer(StaminaHandle);
	}
}

// Уменьшение переменных по таймеру
void UPlayerStatsComponent::HandleStats()
{
	if(Infected == true)
	{
		IncrementInfection(InfectionIncrementValue);
	}

	DecrementHunger(HungerDecrementValue);
	DecrementThirst(ThirstDecrementValue);

	OnInfectionChange.Broadcast(Infection);
	OnHungerChange.Broadcast(Hunger, MaxHunger);
	OnThirstChange.Broadcast(Thirst, MaxThirst);
}

void UPlayerStatsComponent::RegenerateStamina()
{
	IncrementStamina(StaminaIncrementValue);

	OnStaminaChange.Broadcast(Stamina, MaxStamina);
}

// Функция увелечения инфекции
void UPlayerStatsComponent::IncrementInfection(float Value)
{
	if (Infected == true && Infection + Value < 100.0f)
	{
		Infection += Value;
	}
	else
	{
		Infection = 100.0f;
	}
}

// Функция уменьшения голода
void UPlayerStatsComponent::DecrementHunger(float Value)
{
	if (Hunger - Value > 0.0f)
	{
		Hunger -= Value;
	}
	else
	{
		Hunger = 0.0f;
	}

	
}

// Функция уменьшения жажды
void UPlayerStatsComponent::DecrementThirst(float Value)
{
	if (Thirst - Value > 0.0f)
	{
		Thirst -= Value;
	}
	else
	{
		Thirst = 0.0f;
	}
}

// Функция увеличения голода
void UPlayerStatsComponent::IncrementHunger(float Value)
{
	if (Hunger + Value < MaxHunger)
	{
		Hunger += Value;
	}
	else
	{
		Hunger = MaxHunger;
	}
}

// Функция увеличения жажды
void UPlayerStatsComponent::IncrementThirst(float Value)
{
	if (Thirst + Value < MaxThirst)
	{
		Thirst += Value;
	}
	else
	{
		Thirst = MaxThirst;
	}
}

// Функция увелечения выносливости
void UPlayerStatsComponent::IncrementStamina(float Value)
{
	if (Stamina + Value < MaxStamina)
	{
		Stamina += Value;
	}
	else
	{
		Stamina = 100.0f;
	}
}

// Функция уменьшения выносливости
void UPlayerStatsComponent::DecrementStamina(float Value)
{
	if (Stamina - Value > 0.0f)
	{
		Stamina -= Value;
		
	}
	else
	{
		Stamina = 0.0f;
	}

	OnStaminaChange.Broadcast(Stamina, MaxStamina);
}