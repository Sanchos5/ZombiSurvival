// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SurvivalBaseCharacter.generated.h"

UCLASS()
class ZOMBISURVIVAL_API ASurvivalBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivalBaseCharacter(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Default Stats")
	float GetHealth() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Default Stats")
	float Health;

	//Damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

};
