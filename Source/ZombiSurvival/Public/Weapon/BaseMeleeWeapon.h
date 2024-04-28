// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BaseMeleeWeapon.generated.h"

class UTraceComponent;
class UMaterialInstance;

UCLASS()
class ZOMBISURVIVAL_API ABaseMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	ABaseMeleeWeapon();
	FVector GetStartLocation();
	FVector GetEndLocation();

	UTraceComponent* GetTraceComponent() { return TraceComponent; }

	UMaterialInstanceDynamic* Dynamicmaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults")
	UMaterialInterface* Material;

	void GetDynamicMaterial(float ParameterValue);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Start;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* End;

	FTimerHandle DecalHandle;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess= "true"))
	UTraceComponent* TraceComponent;

};
