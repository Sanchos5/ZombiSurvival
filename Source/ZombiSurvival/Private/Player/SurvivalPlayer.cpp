// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SurvivalPlayer.h"
#include "EnhancedInputComponent.h"
#include "Input/SurvivalInputConfig.h"
#include "Input/SurvivalInputComponent.h"
#include "ZombiSurvival/SurvivalGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "Components/SurvivalCharMovementComponent.h"
#include "Components/PlayerStatsComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Widgets/SWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Weapon/BaseMeleeWeapon.h"
#include "Widget/InventoryWidget.h"
#include "Components/TraceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/BaseRangeWeapon.h"

// Sets default values
ASurvivalPlayer::ASurvivalPlayer(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USurvivalCharMovementComponent>(ASurvivalBaseCharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);

	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent->SetupAttachment(GetRootComponent());

	// Create a CameraComponent
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPSCamera->bUsePawnControlRotation = true;
	CameraSocketName = TEXT("CameraSocket");
	FPSCamera->SetupAttachment(MeshComponent, CameraSocketName);

	PlayerStats = CreateDefaultSubobject<UPlayerStatsComponent>(TEXT("PlayerStats"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	AxeSocketName = TEXT("MeleeWeaponSocket");
	CanAttack = true;
	CanSwapWeapon = true;

	bHaveAxe = false;
	bHavePistol = false;
	bHaveShotgun = false;
}

void ASurvivalPlayer::CreatePauseWidget()
{
	if (IsValid(PauseWidgetClass))
	{
		PauseWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), PauseWidgetClass));

		if (PauseWidget)
		{
			PauseWidget->AddToViewport();
			PauseWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ASurvivalPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	CreatePauseWidget();
	
	PlayerStats->Infected = true;

	OnHealthChange.Broadcast(Health, MaxHealth);
}

// Called every frame
void ASurvivalPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASurvivalPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();

	USurvivalInputComponent* SurvivalInputComponent = CastChecked<USurvivalInputComponent>(PlayerInputComponent);
	SurvivalInputComponent->AddInputMappings(InputConfig, Subsystem);

	const FSurvivalGameplayTags& GameplayTags = FSurvivalGameplayTags::Get();
	
	//Bind Input actions by tag
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ASurvivalPlayer::Input_Move);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ASurvivalPlayer::Input_Look);

	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_Jump);

	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Triggered, this, &ASurvivalPlayer::Input_StartSprinting);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Completed, this, &ASurvivalPlayer::Input_StopSprinting);


	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Inventory, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_OpenInventory);
	//SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Inventory, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_ClosedInventory);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_PauseGame, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_PauseGame);

	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Interaction, ETriggerEvent::Triggered, this, &ASurvivalPlayer::Input_Interact);
	
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Attack, ETriggerEvent::Triggered, this, &ASurvivalPlayer::Input_Attacking);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Reload, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_StartReloading);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Reload, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_StopReloading);
	
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_SwapToAxe, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_SwapToAxe);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_SwapToPistol, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_SwapToPistol);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_SwapToShotgun, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_SwapToShotgun);
	
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Triggered, this, &ASurvivalPlayer::Input_Jump);
}

void ASurvivalPlayer::Input_Move(const FInputActionValue& InputActionValue)
{
	if (Controller != nullptr)
	{
		const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (MoveValue.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, MoveValue.X);
		}

		if (MoveValue.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, MoveValue.Y);
		}
	}
}

void ASurvivalPlayer::Input_Look(const FInputActionValue& InputActionValue)
{
	if (Controller != nullptr)
	{
		const FVector2D LookValue = InputActionValue.Get<FVector2D>();

		if (LookValue.X != 0.0f)
		{
			AddControllerYawInput(LookValue.X);
		}

		if (LookValue.Y != 0.0f)
		{
			AddControllerPitchInput(LookValue.Y);
		}
	}
}

void ASurvivalPlayer::Input_Jump(const FInputActionValue& InputActionValue)
{
	if (PlayerStats->GetStamina() - StaminaValue > 0.0f && !GetCharacterMovement()->IsFalling())
	{
		StaminaValue = 10.0f;
		PlayerStats->DecrementStamina(StaminaValue);
		Jump();
	}
}

void ASurvivalPlayer::Input_StartSprinting(const FInputActionValue& InputActionValue)
{
	if(PlayerStats->GetStamina() > 0.0f)
	{
		StaminaValue = 0.2f;
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		
		PlayerStats->DecrementStamina(StaminaValue);

		PlayerStats->SprintingTimer(true);
	}
	else if(PlayerStats->GetStamina() <= 0.0f)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		PlayerStats->SprintingTimer(false);
	}
}

void ASurvivalPlayer::Input_StopSprinting(const FInputActionValue& InputActionValue)
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	PlayerStats->SprintingTimer(false);
}

void ASurvivalPlayer::Input_OpenInventory(const FInputActionValue& InputActionValue)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (InventoryComponent->InventoryWidget != nullptr && PlayerController != nullptr && bOpenInventory == false)
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, InventoryComponent->InventoryWidget);
		InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bOpenInventory = true;
		PlayerController->bShowMouseCursor = true;
	}	
}

void ASurvivalPlayer::Input_PauseGame(const FInputActionValue& InputActionValue)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PauseWidget != nullptr && PlayerController != nullptr)
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, PauseWidget);
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
		PlayerController->bShowMouseCursor = true;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void ASurvivalPlayer::Input_Interact(const FInputActionValue& InputActionValue)
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

void ASurvivalPlayer::Input_Attacking(const FInputActionValue& InputActionValue)
{
	switch (ActiveWeapon)
	{
	case AXE:
		Input_MeleeAttacking();
		break;
		
	case PISTOL:
		// Shoot
		break;
		
	case SHOTGUN:
		// Shoot
		break;
		
	default:
		// No Weapon
		break;
	}
}

void ASurvivalPlayer::Input_MeleeAttacking()
{
	if (CanAttack == false) return;
	
	if (MeleeAttackMontage != nullptr)
	{
		PlayAnimMontage(MeleeAttackMontage, AttackPlayRate);
	}
	
	CanAttack = false;
}

void ASurvivalPlayer::Input_StartReloading(const FInputActionValue& InputActionValue)
{
	//reload
}

void ASurvivalPlayer::Input_StopReloading(const FInputActionValue& InputActionValue)
{
	//Stop reload
}

void ASurvivalPlayer::Input_SwapToAxe_Implementation(const FInputActionValue& InputActionValue)
{
	if (ActiveWeapon == AXE || CanSwapWeapon == false || bHaveAxe == false) return;

	if (RangeWeaponref != nullptr)
	{
		if (ActiveWeapon == PISTOL)
		{
			PistolDispenserMagazine = RangeWeaponref->DispenserMagazine;
		}
		else if (ActiveWeapon == SHOTGUN)
		{
			ShotgubDispenserMagazine = RangeWeaponref->DispenserMagazine;
		}		
		RangeWeaponref->Destroy();
	}
	
	FTransform SocketTransform = GetMesh()->GetSocketTransform(AxeSocketName);
	if (IsValid(AxeWeaponClass))
	{
		MeleeWeaponref = GetWorld()->SpawnActor<ABaseMeleeWeapon>(AxeWeaponClass, SocketTransform);
		if (MeleeWeaponref)
		{
			MeleeWeaponref->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AxeSocketName);
			MeleeWeaponref->GetTraceComponent()->MeleeWeapon->SetOwner(this);
		}
		ActiveWeapon = AXE;
	}
}

void ASurvivalPlayer::Input_SwapToPistol_Implementation(const FInputActionValue& InputActionValue)
{
	if (ActiveWeapon == PISTOL || CanSwapWeapon == false || bHavePistol == false) return;

	if (MeleeWeaponref != nullptr)
	{
		MeleeWeaponref->Destroy();
	}
	
	if (RangeWeaponref != nullptr && ActiveWeapon == SHOTGUN)
	{
		ShotgubDispenserMagazine = RangeWeaponref->DispenserMagazine;
		RangeWeaponref->Destroy();
	}

	FTransform SocketTransform = GetMesh()->GetSocketTransform(PistolSocketName);
	if (IsValid(PistolWeaponClass))
	{
		RangeWeaponref = GetWorld()->SpawnActor<ABaseRangeWeapon>(PistolWeaponClass, SocketTransform);
		if (RangeWeaponref)
		{
			RangeWeaponref->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, PistolSocketName);
			RangeWeaponref->DispenserMagazine = PistolDispenserMagazine;
		}
		ActiveWeapon = PISTOL;
	}
}

void ASurvivalPlayer::Input_SwapToShotgun_Implementation(const FInputActionValue& InputActionValue)
{
	if (ActiveWeapon == SHOTGUN || CanSwapWeapon == false || bHaveShotgun == false) return;

	if (MeleeWeaponref != nullptr)
	{
		MeleeWeaponref->Destroy();
	}
	
	if (RangeWeaponref != nullptr && ActiveWeapon == PISTOL)
	{
		PistolDispenserMagazine = RangeWeaponref->DispenserMagazine;
		RangeWeaponref->Destroy();
	}
	
	FTransform SocketTransform = GetMesh()->GetSocketTransform(ShotgunSocketName);
	if (IsValid(ShotgunWeaponClass))
	{
		RangeWeaponref = GetWorld()->SpawnActor<ABaseRangeWeapon>(ShotgunWeaponClass, SocketTransform);
		if (RangeWeaponref)
		{
			RangeWeaponref->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, ShotgunSocketName);
			RangeWeaponref->DispenserMagazine = ShotgubDispenserMagazine;
		}
		ActiveWeapon = SHOTGUN;
	}
}
