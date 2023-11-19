// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBISURVIVAL_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTraceComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class ABaseMeleeWeapon* Weapon;

	UPROPERTY(BlueprintReadWrite)
	bool WeaponActive = false;

protected:
	virtual void BeginPlay() override;
	void TraceHit();

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float TraceRadius;

	UPROPERTY(BlueprintReadWrite, Category="TraceComponent")
	TArray<AActor*> ActorsToIgnore;

	UFUNCTION(BlueprintCallable, Category="TraceComponent")
	void ClearActorsToIgnore();

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	FName StartTraceName;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	FName EndTraceName;
};
