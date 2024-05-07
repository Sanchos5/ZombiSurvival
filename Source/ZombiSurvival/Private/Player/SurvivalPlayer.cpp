// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SurvivalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "..\..\Public\SaveSystem\SaveGameSystem.h"
#include "Components/SurvivalCharMovementComponent.h"
#include "Components/PlayerStatsComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Widget/InventoryWidget.h"
#include "Components/TraceComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SaveSystem/BaseSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
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

	CameraSocketName = TEXT("CameraSocket");

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 0.f;
	CameraBoom->SetupAttachment(MeshComponent, CameraSocketName);

	// Create a CameraComponent
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPSCamera->bUsePawnControlRotation = false;
	FPSCamera->SetupAttachment(CameraBoom);

	

	PlayerStats = CreateDefaultSubobject<UPlayerStatsComponent>(TEXT("PlayerStats"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	//QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));

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
	USaveGameSystem* GameInstance = GetGameInstance()->GetSubsystem<USaveGameSystem>();
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

void ASurvivalPlayer::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	Super::OnDeath(KillingDamage, DamageEvent, PawnInstigator, DamageCauser);

	if(DeathWidgetClass)
	{
		UUserWidget* DeathWidget;
		DeathWidget = CreateWidget<UUserWidget>(GetWorld(), DeathWidgetClass);
		DeathWidget->AddToViewport();
	}
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
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Completed, this, &ASurvivalPlayer::Input_StopJumping);

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
		PlayerData.PatronsInShotgun = ShotgunDispenserMagazine;
		PlayerData.PatronsInPistol = PistolDispenserMagazine;
		
		UQuestComponent* PlayerQuestComponent = Cast<UQuestComponent>(GetComponentByClass(UQuestComponent::StaticClass()));
		if (IsValid(PlayerQuestComponent))
		{
			 PlayerData.CurrentQuestIndex  = PlayerQuestComponent->GetCurrentQuestIndex();
			 PlayerData.CurrentStepIndex  = PlayerQuestComponent->GetInitialStepsAmount();
		}

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(PlayerData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Actor's SaveGame UPROPERTIES into binary array
		this->Serialize(Ar);

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
		ShotgunDispenserMagazine = PlayerData.PatronsInShotgun;
		PistolDispenserMagazine = PlayerData.PatronsInPistol;

		UQuestComponent* PlayerQuestComponent = Cast<UQuestComponent>(GetComponentByClass(UQuestComponent::StaticClass()));
		if (IsValid(PlayerQuestComponent))
		{
			PlayerQuestComponent->SetCurrentQuestIndex(PlayerData.CurrentQuestIndex);
			PlayerQuestComponent->SetInitialStepsAmount(PlayerData.CurrentStepIndex);
		}
		
		FMemoryReader MemReader(PlayerData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		// Convert binary array back into actor's variables
		this->Serialize(Ar);
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
	if (Controller != nullptr)
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

void ASurvivalPlayer::Input_StopJumping(const FInputActionValue& InputActionValue)
{
	if(!bIsSprinting)
	{
		PlayerStats->SprintingTimer(false);
	}
}

void ASurvivalPlayer::Input_StartSprinting(const FInputActionValue& InputActionValue)
{
	if(GetCharacterMovement()->GetLastInputVector().X != 0.0f)
	{
		if (PlayerStats->GetStamina() > 10.0f)
		{
			bIsSprinting = true;
			PlayerStats->SprintingTimer(true);
		}
	}
}

void ASurvivalPlayer::Input_TriggerSprinting(const FInputActionValue& InputActionValue)
{
	if (bIsSprinting && GetCharacterMovement()->GetLastInputVector().X != 0.0f)
	{
		StaminaValue = 0.2f;
		PlayerStats->DecrementStamina(StaminaValue);
	}

	if (PlayerStats->GetStamina() == 0.0f)
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
		UInventoryWidget* InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryComponent->InventoryWidgetClass);
		if (InventoryWidget != nullptr && PlayerController != nullptr)
		{
			InventoryComponent->InventoryWidget = InventoryWidget;
			InventoryComponent->UpdateAllInventoryUI();
			InventoryComponent->InventoryWidget->AddToViewport();
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, InventoryComponent->InventoryWidget);
			PlayerController->bShowMouseCursor = true;
		}
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
			if (IsValid(PauseWidget))
			{
				PauseWidget->AddToViewport();
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC);
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				PC->bShowMouseCursor = true;
			}
		}
		else if (IsValid(PauseWidget))
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
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(ReloadStartPistol) || GetMesh()->GetAnimInstance()->Montage_IsPlaying(ReloadCyclePistol))
	{
		GetWorld()->GetTimerManager().ClearTimer(StartReloadTimer);
		GetWorld()->GetTimerManager().ClearTimer(CycleReloadTimer);
		
		PlayAnimMontage(ReloadCancelPistol);
	}
	
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
	
	if (FirstAttack != nullptr || SecondAttack != nullptr)
	{
		if (PlayerStats->GetStamina() > 20.0f)
		{
			StaminaValue = 20.0f;
			PlayerStats->DecrementStamina(StaminaValue);
			PlayerStats->SprintingTimer(false);
			
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
			
			CanAttack = false;
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
	if (ActiveWeaponref == nullptr || ActiveWeapon == AXE || ActiveWeapon == NONE) return false;
	
	ABaseRangeWeapon* RangeWeapon = Cast<ABaseRangeWeapon>(ActiveWeaponref);
	
	if (RangeWeapon->DispenserMagazine == RangeWeapon->MaxDispenserMagazine ||
		RangeWeapon->PatronsInInventory <= 0.f) return false;

	UAnimInstance* PlayerAnimInstance = GetMesh()->GetAnimInstance();
	
	
	if (ActiveWeapon == SHOTGUN && ReloadShotgun != nullptr && !PlayerAnimInstance->Montage_IsPlaying(ReloadShotgun))
	{
		PlayAnimMontage(ReloadShotgun);
		return true;
	}
	if (ActiveWeapon == PISTOL)
	{
		PistolStartReload();
		return true;
	}
	return false;
}

void ASurvivalPlayer::PistolStartReload()
{
	UAnimInstance* PlayerAnimInstance = GetMesh()->GetAnimInstance();
	
	if (ReloadStartPistol == nullptr || ReloadCyclePistol == nullptr || ReloadEndPistol == nullptr || PlayerAnimInstance->Montage_IsPlaying(ReloadStartPistol)
		|| PlayerAnimInstance->Montage_IsPlaying(ReloadCyclePistol) || PlayerAnimInstance->Montage_IsPlaying(ReloadEndPistol))
	{
		return;	
	}
	
	float AnimLength = PlayAnimMontage(ReloadStartPistol);
	GetWorld()->GetTimerManager().SetTimer(StartReloadTimer, this, &ASurvivalPlayer::PistolCycleReload, AnimLength, false);
}

void ASurvivalPlayer::PistolCycleReload()
{
	ABaseRangeWeapon* Pistol = Cast<ABaseRangeWeapon>(ActiveWeaponref);
	if (IsValid(Pistol))
	{
		float DeltaNumPatrons = Pistol->MaxDispenserMagazine - Pistol->DispenserMagazine;
		if (DeltaNumPatrons > 0.f && NaganPatronsInInventory > 0.f)
		{
			float AnimLength = PlayAnimMontage(ReloadCyclePistol);
			GetWorld()->GetTimerManager().SetTimer(CycleReloadTimer, this, &ASurvivalPlayer::PistolCycleReload, AnimLength, false);
		}
		if (DeltaNumPatrons <= 0.f || NaganPatronsInInventory <= 0.f)
		{
			PlayAnimMontage(ReloadEndPistol);
		}
	}
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
		
		if (ActiveWeapon == PISTOL )
		{
			PistolDispenserMagazine = OldDispencerMagazine;
		}
		if (ActiveWeapon == SHOTGUN)
		{
			ShotgunDispenserMagazine = OldDispencerMagazine;
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
				if (SelectedWeapon == SHOTGUN)
				{
					NewRangeWeapon->DispenserMagazine = ShotgunDispenserMagazine;
				}
				else
				{	
					NewRangeWeapon->DispenserMagazine = PistolDispenserMagazine;
				}
			}
			ActiveWeaponref->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,SocketName);
			ActiveWeaponref->Owner = this;
			ActiveWeapon = SelectedWeapon;
		}
	}
}
