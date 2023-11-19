// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMeleeWeapon.generated.h"

UCLASS()
class ZOMBISURVIVAL_API ABaseMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseMeleeWeapon();
	FVector GetWeaponSocketLocation(FName SocketName);

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float Damage;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess= "true"))
	class UTraceComponent* TraceComponent;

};
