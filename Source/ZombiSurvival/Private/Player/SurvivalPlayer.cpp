// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SurvivalPlayer.h"

#include "Components/SurvivalCharMovementComponent.h"
#include "Components/PlayerStatsComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Widget/InventoryWidget.h"
#include "Components/TraceComponent.h"
#include "Weapon/BaseMeleeWeapon.h"
#include "GameMode/SurvivalGameMode.h"
#include "SaveSystem/BaseSaveGame.h"
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

	Combo = 0;

	SprintSpeed = 400.0f;
	WalkSpeed = 200.0f;
}

void ASurvivalPlayer::EquipWeaponFromSave()
{
	switch (ActiveWeapon)
	{
	case AXE:
		EquipAxe();
		break;
	case PISTOL:
		EquipPistol();
		break;
	case SHOTGUN:
		EquipShotgun();
		break;
	case NONE:
		
		break;
	}
}

void ASurvivalPlayer::InitPlayerSavedData()
{
	UBaseGameInstance* GameInstance = GetGameInstance()->GetSubsystem<UBaseGameInstance>();
	if (GameInstance)
	{
		GameInstance->InitPlayerSavedData();
	}
	EquipWeaponFromSave();
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

	InitPlayerSavedData();

	PlayerStats->Infected = true;

	OnHealthChange.Broadcast(Health, MaxHealth);
}

void ASurvivalPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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

	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_StartSprinting);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Triggered, this, &ASurvivalPlayer::Input_TriggerSprinting);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Completed, this, &ASurvivalPlayer::Input_StopSprinting);


	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Inventory, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_OpenInventory);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_PauseGame, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_PauseGame);

	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Interaction, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_Interact);
	
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Attack, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_Attacking);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Reload, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_Reloading);
	
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_SwapToAxe, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_SwapToAxe);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_SwapToPistol, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_SwapToPistol);
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_SwapToShotgun, ETriggerEvent::Started, this, &ASurvivalPlayer::Input_SwapToShotgun);
	
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Triggered, this, &ASurvivalPlayer::Input_Jump);
}

void ASurvivalPlayer::SavePlayerStats_Implementation(UBaseSaveGame* SaveObject)
{
	if (SaveObject)
	{
		FPlayerSaveData PlayerData;
		PlayerData.Health = GetHealth();
		PlayerData.Stamina = PlayerStats->GetStamina();
		PlayerData.Infection = PlayerStats->GetInfection();
		PlayerData.Hunger = PlayerStats->GetHunger();
		PlayerData.Thirst = PlayerStats->GetThirst();
		PlayerData.PlayerTransform = GetActorTransform();
		PlayerData.ControllerRotation = GetController()->GetControlRotation();
		PlayerData.Inventory = InventoryComponent->AllItems.MainInventory;
		PlayerData.ActiveWeapon = ActiveWeapon;
		PlayerData.bHaveAxe = bHaveAxe;
		PlayerData.bHavePistol = bHavePistol;
		PlayerData.bHaveShotgun = bHaveShotgun;
		SaveObject->PlayerSaveData = PlayerData;
	}
}

void ASurvivalPlayer::LoadPlayerStats_Implementation(UBaseSaveGame* SaveObject)
{
	if (SaveObject)
	{
		FPlayerSaveData PlayerData = SaveObject->PlayerSaveData;
		SetHealth(PlayerData.Health);
		PlayerStats->SetStamina(PlayerData.Stamina);
		PlayerStats->SetInfection(PlayerData.Infection);
		PlayerStats->SetHunger(PlayerData.Hunger );
		PlayerStats->SetThirst(PlayerData.Thirst);
		SetActorTransform(PlayerData.PlayerTransform);
		GetController()->SetControlRotation(PlayerData.ControllerRotation);
		InventoryComponent->AllItems.MainInventory = PlayerData.Inventory;
		ActiveWeapon = PlayerData.ActiveWeapon;
		bHaveAxe = PlayerData.bHaveAxe;
		bHavePistol = PlayerData.bHavePistol;
		bHaveShotgun = PlayerData.bHaveShotgun;
	}
}

void ASurvivalPlayer::Input_Move(const FInputActionValue& InputActionValue)
{
	if (Controller != nullptr)
	{
		const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		if (MoveValue.IsNearlyZero(0.1))
		{
			return;
		}

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
	if (Controller != nullptr && bRecoil == false)
	{
		const FVector2D LookValue = InputActionValue.Get<FVector2D>();
		if (LookValue.IsNearlyZero(0.1))
		{
			return;
		}

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
	if (PlayerStats->GetStamina() > 10.0f)
	{
		bIsSprinting = true;
		PlayerStats->SprintingTimer(true);
	}
}

void ASurvivalPlayer::Input_TriggerSprinting(const FInputActionValue& InputActionValue)
{
	if (bIsSprinting == true)
	{
		StaminaValue = 0.2f;
		PlayerStats->DecrementStamina(StaminaValue);
	}

	if(PlayerStats->GetStamina() == 0.0f)
	{
		bIsSprinting = false;
	}
}

void ASurvivalPlayer::Input_StopSprinting(const FInputActionValue& InputActionValue)
{
	bIsSprinting = false;
	PlayerStats->SprintingTimer(false);
}

void ASurvivalPlayer::Input_OpenInventory(const FInputActionValue& InputActionValue)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) 
	{
		if (InventoryComponent->InventoryWidget != nullptr && PlayerController != nullptr)
		{
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, InventoryComponent->InventoryWidget);
			InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			PlayerController->bShowMouseCursor = true;
		}
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, InventoryComponent->InventoryWidget);
		InventoryComponent->UpdateAllInventoryUI();
		InventoryComponent->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		PlayerController->bShowMouseCursor = true;
	}	
}

void ASurvivalPlayer::Input_PauseGame(const FInputActionValue& InputActionValue)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	
	if (IsValid(PauseWidgetClass))
	{
		if (!PC->IsPaused())
		{
			PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
			PauseWidget->AddToViewport();
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC);
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			PC->bShowMouseCursor = true;
		}
		else
		{
			PauseWidget->RemoveFromParent();
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			PC->bShowMouseCursor = false;
		}
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
		MeleeAttacking();
		break;
		
	case PISTOL:
		RangeAttacking(ReloadShotgun, RecoilShotgun);
		break;
		
	case SHOTGUN:
		RangeAttacking(ReloadShotgun, RecoilShotgun);
		break;
		
	default:
		// No Weapon
		break;
	}
}

void ASurvivalPlayer::MeleeAttacking()
{
	if (CanAttack == false) return;
	CanAttack = false;
	
	if (FirstAttack != nullptr || SecondAttack != nullptr)
	{
		switch (Combo)
		{
		case 0:
			PlayAnimMontage(FirstAttack, AttackPlayRate);
			Combo = 1;
			break;
		case 1:
			PlayAnimMontage(SecondAttack, AttackPlayRate);
			Combo = 0;
			break;
		default:
			break;
		}
	}
}

void ASurvivalPlayer::RangeAttacking(UAnimMontage* ReloadMontage, UAnimMontage* RecoilMontage)
{
	if (ActiveWeaponref != nullptr && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(ReloadMontage)
		&& !GetMesh()->GetAnimInstance()->Montage_IsPlaying(RecoilMontage))
	{
		ActiveWeaponref->Attack();
	}
}

bool ASurvivalPlayer::PlayReloadMontage()
{
	
	if (ActiveWeaponref == nullptr || ActiveWeapon == AXE || ActiveWeapon == NONE ||
		GetMesh()->GetAnimInstance()->Montage_IsPlaying(ReloadShotgun)) return false;
	
	ABaseRangeWeapon* RangeWeapon = Cast<ABaseRangeWeapon>(ActiveWeaponref);
	
	if (RangeWeapon->DispenserMagazine == RangeWeapon->MaxDispenserMagazine ||
		RangeWeapon->PatronsInInventory <= 0.f) return false;

	UAnimInstance* PlayerAnimInstance = GetMesh()->GetAnimInstance();
	
	
	if (ActiveWeapon == SHOTGUN && ReloadShotgun != nullptr && !PlayerAnimInstance->Montage_IsPlaying(ReloadShotgun))
	{
		PlayAnimMontage(ReloadShotgun);
		return true;
	}
	if (ActiveWeapon == PISTOL && ReloadPistol != nullptr  && !PlayerAnimInstance->Montage_IsPlaying(ReloadPistol))
	{
		PlayAnimMontage(ReloadPistol);
		return true;
	}
	return false;
}

void ASurvivalPlayer::Input_Reloading(const FInputActionValue& InputActionValue)
{
	PlayReloadMontage();
}

void ASurvivalPlayer::EquipAxe_Implementation()
{
	EquipBaseWeapon(AXE, bHaveAxe, AxeWeaponClass, AxeSocketName, false);
}

void ASurvivalPlayer::Input_SwapToAxe(const FInputActionValue& InputActionValue)
{
	if (ActiveWeapon == AXE) return;
	EquipAxe();
}

void ASurvivalPlayer::EquipPistol_Implementation()
{
	EquipBaseWeapon(PISTOL, bHavePistol, PistolWeaponClass, PistolSocketName, true);
}

void ASurvivalPlayer::Input_SwapToPistol(const FInputActionValue& InputActionValue)
{
	if (ActiveWeapon == PISTOL) return;
	EquipPistol();
}

void ASurvivalPlayer::EquipShotgun_Implementation()
{
	EquipBaseWeapon(SHOTGUN, bHaveShotgun, ShotgunWeaponClass, ShotgunSocketName, true);
}

void ASurvivalPlayer::Input_SwapToShotgun(const FInputActionValue& InputActionValue)
{
	if (ActiveWeapon == SHOTGUN) return;
	EquipShotgun();
}

void ASurvivalPlayer::EquipBaseWeapon(EActiveWeapon SelectedWeapon, bool bHaveWeapon,
                                      TSubclassOf<ABaseWeapon> SelectedWeaponClass, FName SocketName, bool bRangeWeapon)
{
	if (CanSwapWeapon == false || bHaveWeapon == false) return;

	float OldDispencerMagazine = 0.f;

	if (ActiveWeaponref != nullptr)
	{
		if (bIsRangeWeapon)
		{
			ABaseRangeWeapon* OldRangeWeapon = Cast<ABaseRangeWeapon>(ActiveWeaponref);
	
			if (OldRangeWeapon)
			{
				OldDispencerMagazine = OldRangeWeapon->DispenserMagazine;
				ActiveWeaponref->Destroy();
			}
		}
		else
		{
			ActiveWeaponref->Destroy();
		}
	}

	
	FTransform SocketTransform = GetMesh()->GetSocketTransform(SocketName);
	if (IsValid(SelectedWeaponClass))
	{
		ActiveWeaponref = GetWorld()->SpawnActor<ABaseWeapon>(SelectedWeaponClass, SocketTransform);
		bIsRangeWeapon = bRangeWeapon;
		
		if (ActiveWeaponref)
		{
			if (bIsRangeWeapon)
			{
				ABaseRangeWeapon* NewRangeWeapon = Cast<ABaseRangeWeapon>(ActiveWeaponref);
				NewRangeWeapon->DispenserMagazine = DispenserMagazine;
				DispenserMagazine = OldDispencerMagazine;
				
			}
			ActiveWeaponref->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,SocketName);
			ActiveWeaponref->Owner = this;
			ActiveWeapon = SelectedWeapon;
		}
	}
}
