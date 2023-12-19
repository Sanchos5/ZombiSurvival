// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInfectionChangeDelegate, float, Infection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHungerChangeDelegate, float, Hunger, float, MaxHunger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnThirstChangeDelegate, float, Thirst, float, MaxThirst);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBISURVIVAL_API UPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	// ����������� �� ���������
	UPlayerStatsComponent();

	// �������� �� �� ����������� �� �����
	bool Infected;

	// �������������� ������
	UFUNCTION(BlueprintCallable)
	void IncrementHunger(float Value);

	// �������������� �����
	void IncrementThirst(float Value);

	UPROPERTY(BlueprintAssignable)
	FOnInfectionChangeDelegate OnInfectionChange;

	UPROPERTY(BlueprintAssignable)
	FOnHungerChangeDelegate OnHungerChange;

	UPROPERTY(BlueprintAssignable)
	FOnThirstChangeDelegate OnThirstChange;

protected:

	// ������ ����
	virtual void BeginPlay() override; 

	// ������ ��� ��������� ������, �����, ��������
	FTimerHandle Handle;

	void HandleStats();

	// ��������
	void IncrementInfection(float Value); 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Infection; 

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float InfectionIncrementValue; 

	// �����
	void DecrementHunger(float Value); 

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxHunger; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Hunger; 

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float HungerDecrementValue; 

	// �����
	void DecrementThirst(float Value);

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxThirst;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Thirst;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float ThirstDecrementValue;

};
