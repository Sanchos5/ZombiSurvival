// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SurvZombiCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
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

	// HitBoxes

	head = CreateDefaultSubobject<UBoxComponent>(TEXT("head"));
	head->SetupAttachment(GetMesh(), FName("head"));
	head->SetCollisionProfileName("BoxCollision");

	upperarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("upperarm_l"));
	upperarm_l->SetupAttachment(GetMesh(), FName("upperarm_l"));
	upperarm_l->SetCollisionProfileName("BoxCollision");

	upperarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("upperarm_r"));
	upperarm_r->SetupAttachment(GetMesh(), FName("upperarm_r"));
	upperarm_r->SetCollisionProfileName("BoxCollision");
	
	lowerarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_l"));
	lowerarm_l->SetupAttachment(GetMesh(), FName("lowerarm_l"));
	lowerarm_l->SetCollisionProfileName("BoxCollision");

	lowerarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_r"));
	lowerarm_r->SetupAttachment(GetMesh(), FName("lowerarm_r"));
	lowerarm_r->SetCollisionProfileName("BoxCollision");

	hand_l = CreateDefaultSubobject<UBoxComponent>(TEXT("hand_l"));
	hand_l->SetupAttachment(GetMesh(), FName("hand_l"));
	hand_l->SetCollisionProfileName("BoxCollision");
	
	hand_r = CreateDefaultSubobject<UBoxComponent>(TEXT("hand_r"));
	hand_r->SetupAttachment(GetMesh(), FName("hand_r"));
	hand_r->SetCollisionProfileName("BoxCollision");

	thigh_l = CreateDefaultSubobject<UBoxComponent>(TEXT("thigh_l"));
	thigh_l->SetupAttachment(GetMesh(), FName("thigh_l"));
	thigh_l->SetCollisionProfileName("BoxCollision");
	
	thigh_r = CreateDefaultSubobject<UBoxComponent>(TEXT("thigh_r"));
	thigh_r->SetupAttachment(GetMesh(), FName("thigh_r"));
	thigh_r->SetCollisionProfileName("BoxCollision");
	
	calf_l = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_l"));
	calf_l->SetupAttachment(GetMesh(), FName("calf_l"));
	calf_l->SetCollisionProfileName("BoxCollision");
	
	calf_r = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_r"));
	calf_r->SetupAttachment(GetMesh(), FName("calf_r"));
	calf_r->SetCollisionProfileName("BoxCollision");
	
	foot_l = CreateDefaultSubobject<UBoxComponent>(TEXT("foot_l"));
	foot_l->SetupAttachment(GetMesh(), FName("foot_l"));
	foot_l->SetCollisionProfileName("BoxCollision");

	foot_r = CreateDefaultSubobject<UBoxComponent>(TEXT("foot_r"));
	foot_r->SetupAttachment(GetMesh(), FName("foot_r"));
	foot_r->SetCollisionProfileName("BoxCollision");
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

void ASurvZombiCharacter::GetAnimHit_Implementation(FName BoxCollisionName)
{
	const auto AIController = Cast<AAIController>(Controller);
	if (!IsValid(GetHitAnim) || AIController->GetBlackboardComponent()->GetValueAsBool(FName("Damaged"))) return;

	GetMesh()->GetAnimInstance()->StopAllMontages(0.1);

	
	if (AIController && AIController->BrainComponent)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("Damaged"), true);
	}
	
	FName StartSection = FName("Default");

	if (BoxCollisionName == FName("head"))
	{
		StartSection = FName("HeadReact");
	}
	else if (BoxCollisionName == FName("upperarm_l") || BoxCollisionName == FName("lowerarm_l") || BoxCollisionName == FName("hand_l"))
	{
		StartSection = FName("LeftArmReact");
	}
	else if (BoxCollisionName == FName("hand_r") || BoxCollisionName == FName("lowerarm_r") || BoxCollisionName == FName("upperarm_r"))
	{
		StartSection = FName("RightArmReact");
	}
	else if (BoxCollisionName == FName("thigh_l") || BoxCollisionName == FName("calf_l") || BoxCollisionName == FName("foot_l"))
	{
		StartSection = FName("LeftLegReact");
	}
	else if (BoxCollisionName == FName("thigh_r") || BoxCollisionName == FName("calf_r") || BoxCollisionName == FName("foot_r"))
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

	USaveGameSystem* SaveGameSystem = GetGameInstance()->GetSubsystem<USaveGameSystem>();
	if (SaveGameSystem)
	{
		SaveGameSystem->AddDestroyedActor(this->GetName());
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

