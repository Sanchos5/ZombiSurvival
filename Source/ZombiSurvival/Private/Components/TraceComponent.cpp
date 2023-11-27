// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TraceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/SurvivalBaseCharacter.h"
#include "Weapon/BaseMeleeWeapon.h"

UTraceComponent::UTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	MeleeWeapon = Cast<ABaseMeleeWeapon>(GetOwner());
}

void UTraceComponent::TraceHit()
{
	if (MeleeWeapon == nullptr) return;

	FVector Start = MeleeWeapon->GetWeaponSocketLocation(StartTraceName);
	FVector End = MeleeWeapon->GetWeaponSocketLocation(EndTraceName);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;
	Objects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	ActorsToIgnore.Add(MeleeWeapon->GetOwner());
	TArray<FHitResult> SphereHitResults;
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, TraceRadius, Objects, false,
		ActorsToIgnore, DrawDebugTrace, SphereHitResults, true);

	for (FHitResult HitResult : SphereHitResults)
	{
		ASurvivalBaseCharacter* Enemy = Cast<ASurvivalBaseCharacter>(HitResult.GetActor());
		if (Enemy && !ActorsToIgnore.Contains(Enemy))
		{
			ASurvivalBaseCharacter* WeaponOwner = Cast<ASurvivalBaseCharacter>(MeleeWeapon->GetOwner());
			UGameplayStatics::ApplyDamage(Enemy, Damage,nullptr,WeaponOwner, DamageTypeClass);
			ActorsToIgnore.Add(Enemy);
		}
	}
}

void UTraceComponent::ClearActorsToIgnore()
{
	ActorsToIgnore.Empty();
}

void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (WeaponActive)
	{
		TraceHit();
	}
}

