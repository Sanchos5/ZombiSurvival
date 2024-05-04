// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class ZOMBISURVIVAL_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseWeapon();

	virtual void Attack();
	float GetDamage() { return Damage; };

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Mechanics|MeleeWeapon")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Mechanics|RangeWeapon")
	float MinDamage;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Mechanics|RangeWeapon")
	float MaxDamage;

	UPROPERTY (EditDefaultsOnly, Category = "Weapon Mechanics|RangeWeapon")
	float MinDamageHead;

	UPROPERTY (EditDefaultsOnly, Category = "Weapon Mechanics|RangeWeapon")
	float MaxDamageHead;
	
	UPROPERTY (EditDefaultsOnly, Category = "Weapon Mechanics|RangeWeapon")
	float MinDamageLimbs;

	UPROPERTY (EditDefaultsOnly, Category = "Weapon Mechanics|RangeWeapon")
	float MaxDamageLimbs;
};
