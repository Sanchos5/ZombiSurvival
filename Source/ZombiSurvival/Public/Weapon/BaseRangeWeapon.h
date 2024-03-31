// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/SurvivalPlayer.h"
#include "BaseRangeWeapon.generated.h"

class UNiagaraSystem;
class UPlayerInterface;

UCLASS()
class ZOMBISURVIVAL_API ABaseRangeWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	ABaseRangeWeapon();
	virtual void Attack() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	int DispenserMagazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	int MaxDispenserMagazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	int PatronsInInventory;

	virtual void Fire();
	void GetPlayerInterface();

protected:
	virtual void BeginPlay() override;
	void ShotLineTrace();
	void ShotLineTraceDecal(float SpreadX, float SpreadY, float SpreadZ);
	void ShotLineTraceVFX();
	void WeaponRecoil();
	void BackCameraPosition();
	void ClearTimer();

	UPROPERTY()
	bool bImpulse;

	UPROPERTY()
	bool bSpawnNS;

	UPROPERTY()
	UPlayerInterface* PlayerInterface;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	int NumOfShotLine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	float Accuracy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	float Impulse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	float AimAssistDistance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	USoundBase* ShotSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	USoundBase* EmptyMagazineSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	UNiagaraSystem* BloodNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	float RecoilRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Mechanics")
	UAnimMontage* CharacterRecoilMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Start;

	UPROPERTY(EditDefaultsOnly)
	float MaxRangeNoise = 0.0f;

	//ссылки на декали
	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Mechanics")
	class UMaterial* DecalBlood;
	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Mechanics")
	class UMaterialInstance* DecalMetal;
	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Mechanics")
	class UMaterialInstance* DecalBloodPawn;

	//размер декалей
	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Mechanics")
	FVector ScaleDecalBloodStatic;
	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Mechanics")
	FVector ScaleDecalMetal;
	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Mechanics")
	FVector ScaleDecalBloodPawn;

private:
	FRotator PlayerControlRotation;

	FTimerHandle RecoilTimerHandle;
	FTimerHandle ClearTimerHandle;
};
