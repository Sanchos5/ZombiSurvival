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

	virtual void GetAnimHit_Implementation(FName BoxCollisionName) override;

	

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

	UPROPERTY(EditAnywhere)
	class UBoxComponent* head;

	UPROPERTY(EditAnywhere)
	UBoxComponent* upperarm_l;

	UPROPERTY(EditAnywhere)
	UBoxComponent* upperarm_r;

	UPROPERTY(EditAnywhere)
	UBoxComponent* lowerarm_l;

	UPROPERTY(EditAnywhere)
	UBoxComponent* lowerarm_r;

	UPROPERTY(EditAnywhere)
	UBoxComponent* hand_l;

	UPROPERTY(EditAnywhere)
	UBoxComponent* hand_r;

	UPROPERTY(EditAnywhere)
	UBoxComponent* thigh_l;

	UPROPERTY(EditAnywhere)
	UBoxComponent* thigh_r;

	UPROPERTY(EditAnywhere)
	UBoxComponent* calf_l;

	UPROPERTY(EditAnywhere)
	UBoxComponent* calf_r;

	UPROPERTY(EditAnywhere)
	UBoxComponent* foot_l;

	UPROPERTY(EditAnywhere)
	UBoxComponent* foot_r;

	UPROPERTY()
	TMap<FName, class UBoxComponent*> HitCollisionBoxes;
};
