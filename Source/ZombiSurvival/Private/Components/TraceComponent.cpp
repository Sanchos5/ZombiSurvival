// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TraceComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISense_Damage.h"


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
	bool bPlaySoundOnce = true;
	
	if (MeleeWeapon == nullptr) return;

	FVector Start = MeleeWeapon->GetStartLocation();
	FVector End = MeleeWeapon->GetEndLocation();
	
	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;
	Objects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	Objects.Add (UEngineTypes::ConvertToObjectType (ECC_WorldStatic));
	
	ASurvivalBaseCharacter* WeaponOwner = Cast<ASurvivalBaseCharacter>(MeleeWeapon->Owner);

	if (WeaponOwner == nullptr) return;

	FVector EyeLocation;
	FRotator EyeRotation;
	WeaponOwner->GetActorEyesViewPoint (EyeLocation, EyeRotation);
	
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
			UGameplayStatics::ApplyDamage(Enemy, Damage,
				nullptr,WeaponOwner, DamageTypeClass);
			
			UAISense_Damage::ReportDamageEvent(GetWorld(), Enemy, WeaponOwner,
			Damage, WeaponOwner->GetActorLocation(), HitResult.Location);

			if (Cast<ICombatInterface>(Enemy))
			{
				Cast<ICombatInterface>(Enemy)->Execute_GetHit(Enemy, HitResult.BoneName);
			}
			
			if (bPlaySoundOnce)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitResult.Location);
				bPlaySoundOnce = false;
			}
			
			UGameplayStatics::SpawnDecalAttached (
			DecalBloodPawn, ScaleDecalBloodPawn, HitResult.Component.Get (), HitResult.BoneName,
			HitResult.ImpactPoint, EyeRotation, EAttachLocation::KeepWorldPosition);

			ActorsToIgnore.Add(Enemy);
		}
		else
		{
			UGameplayStatics::SpawnDecalAtLocation (GetWorld (), DecalMetal, ScaleDecalMetal, HitResult.Location, EyeRotation);
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

