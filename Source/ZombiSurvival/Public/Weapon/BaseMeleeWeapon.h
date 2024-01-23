// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMeleeWeapon.generated.h"

class UTraceComponent;

UCLASS()
class ZOMBISURVIVAL_API ABaseMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseMeleeWeapon();
	FVector GetStartLocation();
	FVector GetEndLocation();

	UTraceComponent* GetTraceComponent() { return TraceComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Start;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* End;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess= "true"))
	UTraceComponent* TraceComponent;

};
