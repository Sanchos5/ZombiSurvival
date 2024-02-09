// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SurvZombiCharacter.h"
#include "Components/PlayerStatsComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASurvZombiCharacter::ASurvZombiCharacter(const class FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);

	MeshComponent = GetMesh();
	MeshComponent->SetupAttachment(GetRootComponent());

}

float ASurvZombiCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	SetTargetActor();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASurvZombiCharacter::SetTargetActor_Implementation()
{
}

