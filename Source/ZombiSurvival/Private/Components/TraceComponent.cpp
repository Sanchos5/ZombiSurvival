// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TraceComponent.h"

#include "AIController.h"
#include "GameplayLibrary.h"
#include "AI/SurvZombiCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	bool bDoOnce = true;
	
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
		
		if (IsValid(Enemy) && !ActorsToIgnore.Contains(Enemy) && UGameplayLibrary::IsNotFriend(Enemy, WeaponOwner))
		{
			ASurvZombiCharacter* Zombie = Cast<ASurvZombiCharacter>(Enemy);
			if (Zombie)
			{
				const AAIController* AIController = Cast<AAIController>(Zombie->Controller);
				if (IsValid(AIController) && AIController->GetBlackboardComponent()->GetValueAsObject(FName("Player")) == nullptr)
				{
					UGameplayStatics::ApplyDamage(Enemy, Enemy->Health + 1000.f,
						nullptr, WeaponOwner, DamageTypeClass);
				}
			}
			else
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage,
				nullptr,WeaponOwner, DamageTypeClass);
			}

			if (Zombie)
			{
				UAISense_Damage::ReportDamageEvent(GetWorld(), Enemy, WeaponOwner,
				Damage, WeaponOwner->GetActorLocation(), HitResult.Location);
			}
			
			if (Cast<ICombatInterface>(Enemy))
			{
				Cast<ICombatInterface>(Enemy)->Execute_GetHit(Enemy, HitResult.BoneName);
			}
			
			if (bDoOnce)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitResult.Location);
				/*if (Zombie->GetMesh()->IsSimulatingPhysics() == true && Zombie)
				{
					Zombie->GetMesh()->AddImpulseAtLocation (-HitResult.ImpactNormal * WeaponImpulse, HitResult.Location);
				}*/
				bDoOnce = false;
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

