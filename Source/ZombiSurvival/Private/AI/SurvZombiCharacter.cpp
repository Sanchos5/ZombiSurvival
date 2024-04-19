// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SurvZombiCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/PlayerStatsComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TraceComponent.h"
#include "Weapon/BaseMeleeWeapon.h"

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

void ASurvZombiCharacter::CreateLeftWeapon()
{
	FTransform SocketTransform = GetMesh()->GetSocketTransform(TEXT("LeftWeaponSocket"));
	if (IsValid(ZombieWeaponClass))
	{
		LeftMeleeWeaponref = GetWorld()->SpawnActor<ABaseMeleeWeapon>(ZombieWeaponClass, SocketTransform);
		if (LeftMeleeWeaponref)
		{
			LeftMeleeWeaponref->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("LeftWeaponSocket"));
			LeftMeleeWeaponref->GetTraceComponent()->MeleeWeapon->SetOwner(this);
		}
	}
}

void ASurvZombiCharacter::GetHit_Implementation(FName BoneName)
{
	const auto AIController = Cast<AAIController>(Controller);
	if (!IsValid(GetHitAnim) || AIController->GetBlackboardComponent()->GetValueAsBool(FName("Damaged"))) return;

	
	if (AIController && AIController->BrainComponent)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("Damaged"), true);
	}
	
	FName StartSection = FName("Default");

	if (BoneName == FName("head"))
	{
		StartSection = FName("HeadReact");
	}
	else if (BoneName == FName("hand_l") || BoneName == FName("lowerarm_l") || BoneName == FName("upperarm_l"))
	{
		StartSection = FName("LeftArmReact");
	}
	else if (BoneName == FName("hand_r") || BoneName == FName("lowerarm_r") || BoneName == FName("upperarm_r"))
	{
		StartSection = FName("RightArmReact");
	}
	else if (BoneName == FName("thigh_l") || BoneName == FName("calf_l") || BoneName == FName("foot_l"))
	{
		StartSection = FName("LeftLegReact");
	}
	else if (BoneName == FName("thigh_r") || BoneName == FName("calf_r") || BoneName == FName("foot_r"))
	{
		StartSection = FName("RightLegReact");
	}
	
	PlayAnimMontage(GetHitAnim, 1, StartSection);
}

void ASurvZombiCharacter::CreateRightWeapon()
{
	FTransform SocketTransform = GetMesh()->GetSocketTransform(TEXT("RightWeaponSocket"));
	if (IsValid(ZombieWeaponClass))
	{
		RightMeleeWeaponref = GetWorld()->SpawnActor<ABaseMeleeWeapon>(ZombieWeaponClass, SocketTransform);
		if (RightMeleeWeaponref)
		{
			RightMeleeWeaponref->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightWeaponSocket"));
			RightMeleeWeaponref->GetTraceComponent()->MeleeWeapon->SetOwner(this);
		}
	}
}

void ASurvZombiCharacter::BeginPlay()
{
	Super::BeginPlay();

	CreateLeftWeapon();
	CreateRightWeapon();
}

float ASurvZombiCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                      AActor* DamageCauser)
{
	SetTargetActor();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASurvZombiCharacter::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator,
	AActor* DamageCauser)
{
	Super::OnDeath(KillingDamage, DamageEvent, PawnInstigator, DamageCauser);
	if (IsValid(RightMeleeWeaponref))
	{
		RightMeleeWeaponref->Destroy();
	}
	if (IsValid(LeftMeleeWeaponref))
	{
		LeftMeleeWeaponref->Destroy();
	}
	StopAnimMontage();

	UBaseGameInstance* GameInstance = GetGameInstance()->GetSubsystem<UBaseGameInstance>();
	if (GameInstance)
	{
		GameInstance->AddDestroyedActor(this);
	}

	const auto AIController = Cast<AAIController>(Controller);
	if (AIController && AIController->BrainComponent)
	{
		AIController->BrainComponent->Cleanup();
	}
}

void ASurvZombiCharacter::SetTargetActor_Implementation()
{
}

