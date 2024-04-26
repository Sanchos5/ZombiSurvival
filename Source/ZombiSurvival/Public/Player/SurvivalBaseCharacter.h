// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/BaseGameInstance.h"
#include "SurvivalBaseCharacter.generated.h"

class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeDelegate, float, Health, float, MaxHealth);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZOMBISURVIVAL_API ASurvivalBaseCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivalBaseCharacter(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable)
    void SetHealth(float NewHealth) { Health = NewHealth; }

	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Health;

	//������������ ���������� ������
	UPROPERTY(EditDefaultsOnly, Category = "Defaults|PlayerStats", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxHealth;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeDelegate OnHealthChange;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDying = false;

	//Damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

protected:
	UFUNCTION(BlueprintCallable)
	void AddHealth(float Heal);

	virtual void OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly)
	USoundBase* SoundDeath;

	UPROPERTY(EditDefaultsOnly, Category="GetHit")
	UAnimMontage* GetHitAnim;
};
