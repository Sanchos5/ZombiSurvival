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

	FVector Start = MeleeWeapon->GetStartLocation();
	FVector End = MeleeWeapon->GetEndLocation();
	
	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;
	Objects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	ASurvivalBaseCharacter* WeaponOwner = Cast<ASurvivalBaseCharacter>(MeleeWeapon->Owner);
	
	ActorsToIgnore.Add(WeaponOwner);
	TArray<FHitResult> SphereHitResults;
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, TraceRadius, Objects, false,
		ActorsToIgnore, DrawDebugTrace, SphereHitResults, true);

	for (FHitResult HitResult : SphereHitResults)
	{
		ASurvivalBaseCharacter* Enemy = Cast<ASurvivalBaseCharacter>(HitResult.GetActor());
		TSubclassOf<class UDamageType> DamageTypeClass;
		if (Enemy && !ActorsToIgnore.Contains(Enemy))
		{
			
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

