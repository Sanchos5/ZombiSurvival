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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Mechanics")
	float Damage;

	UPROPERTY (EditDefaultsOnly, Category = "Weapon Mechanics")
	float DamageHead;
	
	UPROPERTY (EditDefaultsOnly, Category = "Weapon Mechanics")
	float DamageLimbs;
};
