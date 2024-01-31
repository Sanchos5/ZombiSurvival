// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseRangeWeapon.generated.h"

UCLASS()
class ZOMBISURVIVAL_API ABaseRangeWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseRangeWeapon();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int DispenserMagazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int MaxDispenserMagazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int PatronsInInventory;

protected:
	virtual void BeginPlay() override;
	
};
