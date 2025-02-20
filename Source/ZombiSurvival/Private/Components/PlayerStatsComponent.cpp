// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerStatsComponent.h"
#include "TimerManager.h"

UPlayerStatsComponent::UPlayerStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

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
	Thirst = 70.0f;
	ThirstDecrementValue = 9.0f;

	if (Stamina > MaxStamina)
	{
		Stamina = MaxStamina;
	}

	MaxStamina = 100.0f;
	Stamina = 50.0f;
	StaminaIncrementValue = 3.0f;
	StaminaRecoveryDelay = 3.0f;
}

void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	OnInfectionChange.Broadcast(Infection);
	OnHungerChange.Broadcast(Hunger, MaxHunger);
	OnThirstChange.Broadcast(Thirst, MaxThirst);
	OnStaminaChange.Broadcast(Stamina, MaxStamina);
	
	USaveGameSystem* SaveGameSystem = GetOwner()->GetGameInstance()->GetSubsystem<USaveGameSystem>();
	if (SaveGameSystem && SaveGameSystem->bSaveGame == true)
	{
		StartHandleStats();
	}

	//GetWorld()->GetTimerManager().SetTimer(StaminaHandle, this, &UPlayerStatsComponent::RegenerateStamina, 0.2f, true, 1.0f);
	SprintingTimer(false);
}

void UPlayerStatsComponent::SprintingTimer(bool bIsRunning)
{
	if(bIsRunning == true)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(StaminaHandle, this, &UPlayerStatsComponent::RegenerateStamina, 0.2f, true, StaminaRecoveryDelay);
	}
}

void UPlayerStatsComponent::StartHandleStats()
{
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UPlayerStatsComponent::HandleStats, 1.0f, true);
}

void UPlayerStatsComponent::StopHandleStats()
{
	GetWorld()->GetTimerManager().ClearTimer(Handle);
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

	if(ASurvivalPlayer* Player = Cast<ASurvivalPlayer>(GetOwner()))
	{
		if (Infection < 25.0f)
		{
			Player->MaxHealth = 100.0f;
		}
		else if (Infection > 25.0f && Infection < 50.0f)
		{
			Player->MaxHealth = 85.0f;
		}
		else if (Infection > 50.0f && Infection < 75.0f)
		{
			Player->MaxHealth = 65.0f;
		}
		else if (Infection > 75.0f && Infection < 100.0f)
		{
			Player->MaxHealth = 40.0f;
		}
		else if (Infection >= 100.0f)
		{
			Player->TakeDamage(0.1f, FDamageEvent(), Player->GetController(), Player);
		}

		if(Player->Health > Player->MaxHealth)
		{
			Player->Health = Player->MaxHealth;
		}

		Player->OnHealthChange.Broadcast(Player->Health, Player->MaxHealth);
	}
}

void UPlayerStatsComponent::DecrementInfection(float Value)
{
	if (Value <= 0.f) return;
	Infection = FMath::Clamp(Infection - Value, 0.f, 100.f);
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

	if(Hunger > 70.0f)
	{
		MaxStamina = 100.0f;
	}
	else if (Hunger > 35.0f && Hunger < 70.0f)
	{
		MaxStamina = 80.0f;
	}
	else MaxStamina = 60.0f;

	/*if (Hunger <= 0.0f)
	{
		if (ASurvivalPlayer* Player = Cast<ASurvivalPlayer>(GetOwner()))
		{
			Player->TakeDamage(2.0f, FDamageEvent(), Player->GetController(), Player);
		}
	}*/
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

	if (Thirst > 75.0f)
	{
		StaminaIncrementValue = 2.5f;
	}
	else if (Thirst > 35.0f && Thirst < 75.0f)
	{
		StaminaIncrementValue = 1.5f;
	}
	else StaminaIncrementValue = 1.0f;

	/*if (Thirst <= 0.0f)
	{
		if (ASurvivalPlayer* Player = Cast<ASurvivalPlayer>(GetOwner()))
		{
			Player->TakeDamage(2.0f, FDamageEvent(), Player->GetController(), Player);
		}
	}*/
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
		Stamina = MaxStamina;
	}
}

// Функция уменьшения выносливости
void UPlayerStatsComponent::DecrementStamina(float Value)
{
	if (Stamina != 0.0f && Stamina - Value > 0.0f)
	{
		Stamina -= Value;
	}
	else
	{
		Stamina = 0.0f;
	}

	OnStaminaChange.Broadcast(Stamina, MaxStamina);
}