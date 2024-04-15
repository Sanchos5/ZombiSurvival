// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SurvivalBaseCharacter.h"
#include "Weapon/BaseMeleeWeapon.h"
#include "SurvZombiCharacter.generated.h"

class USkeletalMeshComponent;
class UPlayerStatsComponent;
class ABaseMeleeWeapon;
/**
 * 
 */

UCLASS()
class ZOMBISURVIVAL_API ASurvZombiCharacter : public ASurvivalBaseCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ASurvZombiCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void GetHit_Implementation(FName BoneName) override;
	

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;

	void CreateLeftWeapon();
	void CreateRightWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<ABaseMeleeWeapon> ZombieWeaponClass;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	ABaseMeleeWeapon* RightMeleeWeaponref;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	ABaseMeleeWeapon* LeftMeleeWeaponref;
	
	UFUNCTION(BlueprintNativeEvent)
	void SetTargetActor();
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;
};
