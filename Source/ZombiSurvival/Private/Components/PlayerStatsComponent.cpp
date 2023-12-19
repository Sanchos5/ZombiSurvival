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

	MaxThirst = 100.0f;
	Thirst = 50.0f;
	ThirstDecrementValue = 9.0f;
}


// Начало игры
void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	OnInfectionChange.Broadcast(Infection);
	OnHungerChange.Broadcast(Hunger, MaxHunger);
	OnThirstChange.Broadcast(Thirst, MaxThirst);

	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UPlayerStatsComponent::HandleStats, 1.0f, true);
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

	OnHungerChange.Broadcast(Hunger, MaxHunger);
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