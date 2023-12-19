// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SurvivalBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeDelegate, float, Health, float, MaxHealth);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZOMBISURVIVAL_API ASurvivalBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivalBaseCharacter(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeDelegate OnHealthChange;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxHealth;

	//Damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);

	bool bIsDying = false;
};
