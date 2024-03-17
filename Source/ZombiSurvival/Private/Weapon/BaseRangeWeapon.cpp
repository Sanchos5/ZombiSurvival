// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseRangeWeapon.h"

#include "AI/SurvZombiCharacter.h"
#include "Interface/PatronsInterface.h"
#include "Perception/AISense_Damage.h"
#include "UI/HUDSurvival.h"
#include "Widget/PlayerInterface.h"
#include "NiagaraFunctionLibrary.h"

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

void ABaseRangeWeapon::Fire()
{
	bImpulse = true;
	bSpawnNS = true;
	if (DispenserMagazine > 0.f)
	{
		DispenserMagazine -= 1.f;
		UGameplayStatics::PlaySound2D(GetWorld(), ShotSound);
		// TODO: Spawn VFX

		//Interface to subtract patrons in UI
		IPatronsInterface::Execute_SubtractPatron(PlayerInterface->PatronsBar);
		for (int i = NumOfShotLine; i>0; i--)
		{
			ShotLineTrace();
		}
		MakeNoise((1.0f), UGameplayStatics::GetPlayerPawn(GetWorld(), 0), GetActorLocation(), MaxRangeNoise);
	}
	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), EmptyMagazineSound);
		ASurvivalPlayer* Player = Cast<ASurvivalPlayer>(Owner);
		if (Player)
		{
			Player->PlayReloadMontage();
		}
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

	//FVector TraceStart = Start->GetComponentLocation();
	
	const FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * AimAssistDistance);
	float SpreadX = UKismetMathLibrary::RandomFloatInRange(-Accuracy, Accuracy);
	float SpreadY = UKismetMathLibrary::RandomFloatInRange(-Accuracy, Accuracy);
	float SpreadZ = UKismetMathLibrary::RandomFloatInRange(-Accuracy, Accuracy);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(SurvivalCharacter);
	
	FHitResult HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	
	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), EyeLocation, TraceEnd + FVector(SpreadX, SpreadY, SpreadZ), ObjectTypes, true,
		ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);


	if (bHit)
	{
		ASurvZombiCharacter* Zombie = Cast<ASurvZombiCharacter>(HitResult.GetActor());
		if (Zombie)
		{
			float DamagetoZombie = UKismetMathLibrary::RandomFloatInRange(Damage - 2.f, Damage + 2.f);
			TSubclassOf<class UDamageType> DamageTypeClass;
			
			if(HitResult.BoneName == TEXT("head"))
			{
				UGameplayStatics::ApplyDamage(Zombie, DamagetoZombie * 3.f,
					SurvivalCharacter->GetController(), SurvivalCharacter,DamageTypeClass );
			}
			else
			{
				UGameplayStatics::ApplyDamage(Zombie, DamagetoZombie,
					SurvivalCharacter->GetController(), SurvivalCharacter,DamageTypeClass );
			}

			// Report zombie that player damage him
			UAISense_Damage::ReportDamageEvent(GetWorld(), Zombie, SurvivalCharacter,
				DamagetoZombie, SurvivalCharacter->GetActorLocation(), HitResult.Location);

			if (IsValid(BloodNiagaraSystem) && bSpawnNS == true)
			{
				UKismetMathLibrary::MakeRotFromZ(HitResult.Normal);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodNiagaraSystem, HitResult.Location);
				bSpawnNS = false;
			}

			if (Zombie->GetMesh()->IsSimulatingPhysics() == true && bImpulse == true)
			{
				Zombie->GetMesh()->AddImpulseAtLocation(-HitResult.ImpactNormal * Impulse, HitResult.Location);
				bImpulse = false;
			}
		}
	}
}


