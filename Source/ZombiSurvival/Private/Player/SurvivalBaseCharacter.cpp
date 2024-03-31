// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SurvivalBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASurvivalBaseCharacter::ASurvivalBaseCharacter(const class FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if(Health > MaxHealth)
	{
		Health = MaxHealth;
	}

	MaxHealth = 100.0f;
	Health = 100.0f;
}



float ASurvivalBaseCharacter::GetHealth() const
{
	return Health;
}

float ASurvivalBaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (bIsDead == true) return 0.f;
	
	if(Damage > 0.f)
	{
		Health -= Damage;

		if (Health <= 0.f)
		{
			Health = 0.0f;
			OnDeath(Damage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		}
		OnHealthChange.Broadcast(Health, MaxHealth);
	}
	return Damage;
}

void ASurvivalBaseCharacter::AddHealth(float Heal)
{
	if (Heal <= 0.f) return;
	Health = FMath::Clamp(GetHealth() + Heal, 0.f, MaxHealth);
}

void ASurvivalBaseCharacter::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	bIsDead = true;

	if(SoundDeath)
	{
		FVector SpawnLocation = GetActorLocation();
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundDeath, SpawnLocation);
	}

	/* Disable all collision on capsule */
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	USkeletalMeshComponent* Mesh1P = GetMesh();
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh1P->SetSimulatePhysics(true);
	Mesh1P->SetAllBodiesSimulatePhysics(true);

	DisableInput(nullptr);
}
