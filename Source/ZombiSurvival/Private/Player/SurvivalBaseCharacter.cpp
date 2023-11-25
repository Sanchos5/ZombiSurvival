// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SurvivalBaseCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASurvivalBaseCharacter::ASurvivalBaseCharacter(const class FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Health = 100.0f;
}

float ASurvivalBaseCharacter::GetHealth() const
{
	return Health;
}

float ASurvivalBaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if(Damage > 0.f)
	{
		Health -= Damage;

		if (Health <= 0.f)
		{
			Health = 0.0f;
			bIsDying = true;
			OnDeath(Damage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		}
	}
	return Damage;
}

void ASurvivalBaseCharacter::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	/*if (bIsDying)
	{
		return;
	}*/
	bIsDying = true;

	/* Disable all collision on capsule */
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	DisableInput(nullptr);

}