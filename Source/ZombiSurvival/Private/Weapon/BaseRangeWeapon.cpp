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
	
	//Interface to subtract patrons in UI
	IPatronsInterface::Execute_SubtractPatron(PlayerInterface->PatronsBar);
	for (int i = NumOfShotLine; i>0; i--)
	{
		ShotLineTrace();
	}
	ShotLineTraceVFX();
	WeaponRecoil();
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
	APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0.f);
	if (!IsValid(SurvivalCharacter) || !IsValid(PlayerCamera)) return;
	FVector EyeLocation;
	FRotator EyeRotation;
	
	if (PlayerCamera)
	{
		PlayerCamera->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	}
	
	const FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * AimAssistDistance);
	float SpreadX = UKismetMathLibrary::RandomFloatInRange(-Accuracy, Accuracy);
	float SpreadY = UKismetMathLibrary::RandomFloatInRange(-Accuracy, Accuracy);
	float SpreadZ = UKismetMathLibrary::RandomFloatInRange(-Accuracy, Accuracy);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(SurvivalCharacter);
	
	FHitResult HitResult;
	FHitResult HitResultCollision;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	
	
	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), EyeLocation, TraceEnd + FVector(SpreadX, SpreadY, SpreadZ), ObjectTypes, true,
		ActorsToIgnore, DrawDebugTrace, HitResult, true);

	bool bCollisionHit = UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), EyeLocation, TraceEnd + FVector(SpreadX, SpreadY, SpreadZ), FName("BoxCollision"),
		true, ActorsToIgnore, DrawDebugTrace, HitResultCollision, true);

	
	if (bCollisionHit)
	{
		
		ASurvZombiCharacter* Zombie = Cast<ASurvZombiCharacter>(HitResult.GetActor());
		if (Zombie)
		{
			float DamagetoZombie = UKismetMathLibrary::RandomFloatInRange(Damage - 2.f, Damage + 2.f);
			TSubclassOf<class UDamageType> DamageTypeClass;
			
			if(HitResultCollision.Component->GetFName() == FName("head"))
			{
				DamagetoZombie = UKismetMathLibrary::RandomFloatInRange (DamageHead - 2.f, DamageHead + 2.f);
			}
			else if(HitResultCollision.Component->GetFName() == FName("upperarm_l")
				|| HitResultCollision.Component->GetFName() == FName ("lowerarm_l")
				|| HitResultCollision.Component->GetFName() == FName ("hand_l")
				|| HitResultCollision.Component->GetFName() == FName ("hand_r")
				|| HitResultCollision.Component->GetFName() == FName ("lowerarm_r")
				|| HitResultCollision.Component->GetFName() == FName ("upperarm_r")
				|| HitResultCollision.Component->GetFName() == FName ("thigh_l")
				|| HitResultCollision.Component->GetFName() == FName ("calf_l")
				|| HitResultCollision.Component->GetFName() == FName ("foot_l")
				|| HitResultCollision.Component->GetFName() == FName ("thigh_r")
				|| HitResultCollision.Component->GetFName() == FName ("calf_r")
				|| HitResultCollision.Component->GetFName() == FName ("foot_r"))
			{
				DamagetoZombie = UKismetMathLibrary::RandomFloatInRange (DamageLimbs  - 2.f,DamageLimbs + 2.f);
			}
			else
			{
				DamagetoZombie = UKismetMathLibrary::RandomFloatInRange (Damage - 2.f, Damage + 2.f);
			}

			DamagetoZombie = CalculateDamage(Zombie, DamagetoZombie);

			UGameplayStatics::ApplyDamage(Zombie, DamagetoZombie,
				SurvivalCharacter->GetController(), SurvivalCharacter,DamageTypeClass );

			ShotLineTraceDecal (SpreadX, SpreadY, SpreadZ);
			

			if (DamagetoZombie > 0.f  && bHit)
			{
				if (Cast<ICombatInterface>(Zombie))
				{
					Cast<ICombatInterface>(Zombie)->Execute_GetHit(Zombie, HitResultCollision.Component->GetFName());
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
		else if (bHit)
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
		FinalDamage = FinalDamage * (1-((DistanceToTarget-MaxDamageDistance)/MinDamageDistance));
	}
	
	return FinalDamage;
}


