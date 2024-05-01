// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Player/SurvivalBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "ZombiSurvival/SurvivalGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "Input/SurvivalInputConfig.h"
#include "Input/SurvivalInputComponent.h"
#include "TimerManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "QuestionSystem/QuestComponent.h"
#include "SurvivalPlayer.generated.h"

class UBaseSaveGame;
class ABaseWeapon;

UENUM(BlueprintType)
enum EActiveWeapon : uint8
{
	NONE,
	AXE,
	PISTOL,
	SHOTGUN
};

class UInteractionComponent;
class UInventoryComponent;
class UCameraComponent;
class UInputComponent;
class USurvivalInputConfig;
class ABaseMeleeWeapon;
class ABaseRangeWeapon;
struct FInputActionValue;
class UInputMappingContext;
class USkeletalMeshComponent;
class UPlayerStatsComponent;

UCLASS(config = game)
class ZOMBISURVIVAL_API ASurvivalPlayer : public ASurvivalBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivalPlayer(const class FObjectInitializer& ObjectInitializer);
	void EquipWeaponFromSave();
	void InitPlayerSavedData();

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFPSComponent() const { return FPSCamera; }

	/** Returns InventoryComponent subobject **/
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	/** The input config that maps Input Actions to Input Tags*/
	UPROPERTY(EditDefaultsOnly, Category = "Defaults | Input")
	USurvivalInputConfig* InputConfig;

	// Управление
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
	void Input_StopJumping(const FInputActionValue& InputActionValue);
	void Input_StartSprinting(const FInputActionValue& InputActionValue);
	void Input_TriggerSprinting(const FInputActionValue& InputActionValue);
	void Input_StopSprinting(const FInputActionValue& InputActionValue);
	void Input_OpenInventory(const FInputActionValue& InputActionValue);
	void Input_PauseGame(const FInputActionValue& InputActionValue);
	void Input_Interact(const FInputActionValue& InputActionValue);
	

	/** Swap Weapon */
	void Input_SwapToAxe(const FInputActionValue& InputActionValue);
	void Input_SwapToPistol(const FInputActionValue& InputActionValue);
	void Input_SwapToShotgun(const FInputActionValue& InputActionValue);
	
	void EquipBaseWeapon(EActiveWeapon SelectedWeapon, bool bHaveWeapon,
		TSubclassOf<ABaseWeapon> SelectedWeaponClass, FName SocketName, bool bRangeWeapon);

	UFUNCTION(BlueprintNativeEvent)
	void EquipAxe();

	UFUNCTION(BlueprintNativeEvent)
	void EquipPistol();

	UFUNCTION(BlueprintNativeEvent)
	void EquipShotgun();

	/** Attack */
	void Input_Attacking(const FInputActionValue& InputActionValue);
	void MeleeAttacking();
	void RangeAttacking(UAnimMontage* ReloadMontage, UAnimMontage* RecoilAnim);

	/** Reload */
	bool PlayReloadMontage();

	/** Reload Weapon */
	UFUNCTION(BlueprintCallable)
	void Input_Reloading(const FInputActionValue& InputActionValue);

	
	// End Enhanced Input Sample changes

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults | Stats")
	UPlayerStatsComponent* PlayerStats;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TEnumAsByte<EActiveWeapon> ActiveWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsSprinting = false;

	// Sprinting
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults | Move")
	float SprintSpeed;

	// Walking
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults | Move")
	float WalkSpeed;

	// Save System
	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerStats(UBaseSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerStats(UBaseSaveGame* SaveObject);

	UPROPERTY(BlueprintReadWrite)
	int NaganPatronsInInventory;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);
	
	void PistolStartReload();
	void PistolCycleReload();

	FTimerHandle StartReloadTimer;
	FTimerHandle CycleReloadTimer;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	FName CameraSocketName;

	// Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon|Axe")
	TSubclassOf<ABaseWeapon> AxeWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon|Pistol")
	TSubclassOf<ABaseWeapon> PistolWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon|Shotgun")
	TSubclassOf<ABaseWeapon> ShotgunWeaponClass;
	
	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	ABaseWeapon* ActiveWeaponref;

	UPROPERTY(BlueprintReadWrite, Category="Weapon")
	bool bHaveAxe;

	UPROPERTY(BlueprintReadWrite, Category="Weapon")
	bool bHavePistol;

	UPROPERTY(BlueprintReadWrite, Category="Weapon")
	bool bHaveShotgun;

	UPROPERTY(BlueprintReadWrite, Category="Weapon")
	bool bIsRangeWeapon;

	UPROPERTY(BlueprintReadWrite)
	bool CanSwapWeapon;

	UPROPERTY(BlueprintReadWrite)
	int Combo;

	UPROPERTY(BlueprintReadWrite)
	int ShotgunDispenserMagazine;

	UPROPERTY(BlueprintReadWrite)
	int PistolDispenserMagazine;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Axe")
	FName AxeSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Pistol")
	FName PistolSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Shotgun")
	FName ShotgunSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Pistol")
	FName CycleSocketName;

	UPROPERTY(BlueprintReadWrite, Category="Weapon")
	bool CanAttack;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Axe")
	UAnimMontage* FirstAttack;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Axe")
	UAnimMontage* SecondAttack;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Shotgun")
	UAnimMontage* ReloadShotgun;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Pistol")
	UAnimMontage* ReloadStartPistol;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Pistol")
	UAnimMontage* ReloadCyclePistol;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon|Pistol")
	UAnimMontage* ReloadEndPistol;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Pistol")
	UAnimMontage* ReloadCancelPistol;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Shotgun")
	UAnimMontage* RecoilShotgun;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Pistol")
	UAnimMontage* RecoilPistol;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float AttackPlayRate;

	// Pause Game
	UPROPERTY(EditAnywhere, Category="Defaults | Widget")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY()
	class UUserWidget* PauseWidget;

	UPROPERTY(EditAnywhere, Category = "Defaults | Widget")
	TSubclassOf<UUserWidget> DeathWidgetClass;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FPSCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults | Input")
	mutable const UInputMappingContext* InputMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	class UQuestComponent* QuestComponent;

	float StaminaValue = 0.2f;
};
