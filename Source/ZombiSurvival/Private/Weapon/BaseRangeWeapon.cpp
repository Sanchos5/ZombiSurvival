// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseRangeWeapon.h"

#include "AI/SurvZombiCharacter.h"
#include "Interface/PatronsInterface.h"
#include "Perception/AISense_Damage.h"
#include "UI/HUDSurvival.h"
#include "Widget/PlayerInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

ABaseRangeWeapon::ABaseRangeWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Start = CreateDefaultSubobject<USceneComponent>(TEXT("Start"));
	Start->SetupAttachment(GetRootComponent());

	
	bImpulse = true;
	bSpawnNS = true;
	AimAssistDistance = 5000.f;
}

void ABaseRangeWeapon::Attack()
{
	Fire();
}

void ABaseRangeWeapon::Shot_Implementation()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ShotSound);
	WeaponRecoil();

	//Interface to subtract patrons in UI
	IPatronsInterface::Execute_SubtractPatron(PlayerInterface->PatronsBar);
	for (int i = NumOfShotLine; i>0; i--)
	{
		ShotLineTrace();
	}
	ShotLineTraceVFX();
	MakeNoise((1.0f), UGameplayStatics::GetPlayerPawn(GetWorld(), 0), GetActorLocation(), MaxRangeNoise);
}

void ABaseRangeWeapon::Fire()
{
	bImpulse = true;
	bSpawnNS = true;
	if (DispenserMagazine > 0.f)
	{
		DispenserMagazine -= 1.;		
		Shot();
	}
	else if (PatronsInInventory > 0.f)
	{
		ASurvivalPlayer* Player = Cast<ASurvivalPlayer>(Owner);
		if (Player)
		{
			Player->PlayReloadMontage();
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), EmptyMagazineSound);
	}
}

void ABaseRangeWeapon::GetPlayerInterface()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0.f);
	if (PlayerController)
	{
		AHUDSurvival* PlayerHUD = Cast<AHUDSurvival>(PlayerController->GetHUD());
		if (PlayerHUD)
		{
			PlayerInterface = PlayerHUD->PlayerInterface;
		}
	}
}

void ABaseRangeWeapon::BeginPlay()
{
	Super::BeginPlay();

	GetPlayerInterface();
}

void ABaseRangeWeapon::ShotLineTrace()
{
	ASurvivalBaseCharacter* SurvivalCharacter = Cast<ASurvivalBaseCharacter>(Owner);
	FVector EyeLocation;
	FRotator EyeRotation;
	
	if (SurvivalCharacter)
	{
		SurvivalCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	}
	
	const FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * AimAssistDistance);
	float SpreadX = UKismetMathLibrary::RandomFloatInRange(-Accuracy, Accuracy);
	float SpreadY = UKismetMathLibrary::RandomFloatInRange(-Accuracy, Accuracy);
	float SpreadZ = UKismetMathLibrary::RandomFloatInRange(-Accuracy, Accuracy);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(SurvivalCharacter);
	
	FHitResult HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	
	
	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), EyeLocation, TraceEnd + FVector(SpreadX, SpreadY, SpreadZ), ObjectTypes, true,
		ActorsToIgnore, DrawDebugTrace, HitResult, true);

	
	if (bHit)
	{
		
		ASurvZombiCharacter* Zombie = Cast<ASurvZombiCharacter>(HitResult.GetActor());
		if (Zombie)
		{
			float DamagetoZombie = UKismetMathLibrary::RandomFloatInRange(Damage - 2.f, Damage + 2.f);
			TSubclassOf<class UDamageType> DamageTypeClass;
			
			if(HitResult.BoneName == TEXT("head"))
			{
				DamagetoZombie = UKismetMathLibrary::RandomFloatInRange (DamageHead - 2.f, DamageHead + 2.f);
			}
			else if(HitResult.BoneName == TEXT ("spine_01")
				|| HitResult.BoneName == TEXT ("spine_02")
				|| HitResult.BoneName == TEXT ("spine_03")
				|| HitResult.BoneName == TEXT ("neck_01")
				|| HitResult.BoneName == TEXT ("pelvis")
				|| HitResult.BoneName == TEXT ("clavicle_l")
				|| HitResult.BoneName == TEXT ("clavicle_r"))
			{
				DamagetoZombie = UKismetMathLibrary::RandomFloatInRange (Damage - 2.f, Damage + 2.f);
			}
			else
			{
				DamagetoZombie = UKismetMathLibrary::RandomFloatInRange (DamageLimbs - 2.f, DamageLimbs + 2.f);
			}

			DamagetoZombie = CalculateDamage(Zombie, DamagetoZombie);

			UGameplayStatics::ApplyDamage(Zombie, DamagetoZombie,
				SurvivalCharacter->GetController(), SurvivalCharacter,DamageTypeClass );

			ShotLineTraceDecal (SpreadX, SpreadY, SpreadZ);

			if (DamagetoZombie > 0.f)
			{
				if (Cast<ICombatInterface>(Zombie))
				{
					Cast<ICombatInterface>(Zombie)->Execute_GetHit(Zombie, HitResult.PhysMaterial->GetFName());
				}
				
				// Report zombie that player damage him
				UAISense_Damage::ReportDamageEvent(GetWorld(), Zombie, SurvivalCharacter,
					DamagetoZombie, SurvivalCharacter->GetActorLocation(), HitResult.Location);

				if (Zombie->GetMesh()->IsSimulatingPhysics() == true && bImpulse == true)
				{
					Zombie->GetMesh()->AddImpulseAtLocation(-HitResult.ImpactNormal * Impulse, HitResult.Location);
					bImpulse = false;
				}
				
				UGameplayStatics::SpawnDecalAttached(DecalBloodPawn, ScaleDecalBloodPawn, HitResult.Component.Get (),
					HitResult.BoneName,HitResult.ImpactPoint, EyeRotation, EAttachLocation::KeepWorldPosition);
			}
		}
		else
		{
			UGameplayStatics::SpawnDecalAtLocation(GetWorld (), DecalMetal, ScaleDecalMetal, HitResult.Location, EyeRotation);
		}
	}
}
void ABaseRangeWeapon::ShotLineTraceDecal(float SpreadX, float SpreadY, float SpreadZ)
{
	ASurvivalBaseCharacter* SurvivalCharacter = Cast<ASurvivalBaseCharacter> (Owner);
	
	FVector EyeLocation;
	FRotator EyeRotation;

	if (SurvivalCharacter)
	{
		SurvivalCharacter->GetActorEyesViewPoint (EyeLocation, EyeRotation);
	}

	const FVector TraceEnd = EyeLocation + (EyeRotation.Vector () * AimAssistDistance/10);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add (SurvivalCharacter);
	
	FHitResult HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add (UEngineTypes::ConvertToObjectType (ECC_WorldStatic));


	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects (GetWorld (), EyeLocation, TraceEnd + FVector (SpreadX, SpreadY, SpreadZ)/10, ObjectTypes, true,
		ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	UDecalComponent* MyDecal = UGameplayStatics::SpawnDecalAtLocation (GetWorld(), DecalBlood, ScaleDecalBloodStatic, HitResult.Location, EyeRotation);

}

void ABaseRangeWeapon::ShotLineTraceVFX()
{
	ASurvivalBaseCharacter* SurvivalCharacter = Cast<ASurvivalBaseCharacter>(Owner);
	FVector EyeLocation;
	FRotator EyeRotation;
	
	if (SurvivalCharacter)
	{
		SurvivalCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	}
	
	const FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * AimAssistDistance);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(SurvivalCharacter);
	
	FHitResult HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), EyeLocation, TraceEnd, ObjectTypes, true,
		ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
	if (bHit)
	{
		if (IsValid(BloodNiagaraSystem))
		{			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodNiagaraSystem, HitResult.Location, UKismetMathLibrary::MakeRotFromZ(HitResult.Normal));
		}
	}
}

void ABaseRangeWeapon::WeaponRecoil()
{
	DoOnce = true;
	ASurvivalPlayer* Player = Cast<ASurvivalPlayer>(GetOwner());
	if (IsValid(Player) && IsValid(CharacterRecoilMontage))
	{
		OldControlPitch = UKismetMathLibrary::NormalizeAxis(Player->GetControlRotation().Pitch);
		Player->AddControllerPitchInput(FMath::RandRange(-RecoilRange/2.f, -RecoilRange));
		Player->PlayAnimMontage(CharacterRecoilMontage);
		GetWorld()->GetTimerManager().SetTimer(RecoilTimerHandle, this, &ABaseRangeWeapon::BackCameraPosition, BackPositionFrequency, true);
		GetWorld()->GetTimerManager().SetTimer(ClearTimerHandle, this, &ABaseRangeWeapon::ClearTimer, RecoilTimeClear, false);
	}
}

void ABaseRangeWeapon::BackCameraPosition()
{
	
	ASurvivalPlayer* Player = Cast<ASurvivalPlayer>(GetOwner());
	if (IsValid(Player))
	{
		if (DoOnce)
		{
			NewControlPitch = UKismetMathLibrary::NormalizeAxis(Player->GetControlRotation().Pitch);
			DoOnce = false;
		}
		float DeltaPitch = OldControlPitch - NewControlPitch;
		Player->AddControllerPitchInput(-DeltaPitch/DeltaPitchDivider);
	}
}

void ABaseRangeWeapon::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(RecoilTimerHandle);
}

float ABaseRangeWeapon::CalculateDamage(AActor* TargetActor, float FinalDamage)
{
	// Calculate Damage based on distance
	float DistanceToTarget = GetDistanceTo(TargetActor);
	if (DistanceToTarget <= MaxDamageDistance)
	{
		FinalDamage *= 1.f;
	}
	else if (DistanceToTarget >= MaxRange)
	{
		FinalDamage = 0.f;
	}
	else if (DistanceToTarget > MinDamageDistance)
	{
		FinalDamage = FinalDamage / MinDamageDivider;
	}
	else
	{
		FinalDamage = FinalDamage - ((DistanceToTarget - FinalDamage) / MaxToMinDamageDivider);
	}
	
	return FinalDamage;
}


