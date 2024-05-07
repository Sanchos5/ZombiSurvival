// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SurvivalBaseCharacter.h"
#include "Weapon/BaseMeleeWeapon.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBISURVIVAL_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTraceComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class ABaseMeleeWeapon* MeleeWeapon;

	UPROPERTY(BlueprintReadWrite)
	bool WeaponActive = false;

protected:
	virtual void BeginPlay() override;
	void TraceHit();

	UPROPERTY(EditDefaultsOnly, Category="TraceComponent")
	TSubclassOf<AActor> BloodVFX;

	UPROPERTY(EditDefaultsOnly, Category="TraceComponent")
	float TraceRadius;

	UPROPERTY(BlueprintReadWrite, Category="TraceComponent")
	TArray<AActor*> ActorsToIgnore;

	UFUNCTION(BlueprintCallable, Category="TraceComponent")
	void ClearActorsToIgnore();

	UPROPERTY(EditDefaultsOnly, Category="TraceComponent")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTrace = EDrawDebugTrace::ForDuration;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Mechanics")
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Mechanics")
	float WeaponImpulse;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Mechanics")
	UMaterialInstance* DecalBloodPawn;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon Mechanics")
	UMaterialInstance* DecalMetal;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Mechanics")
	FVector ScaleDecalBloodPawn;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon Mechanics")
	FVector ScaleDecalMetal;

};
