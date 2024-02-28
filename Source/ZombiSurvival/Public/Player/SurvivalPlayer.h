// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SurvivalBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "SurvivalPlayer.generated.h"

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
	void Input_StartSprinting(const FInputActionValue& InputActionValue);
	void Input_StopSprinting(const FInputActionValue& InputActionValue);
	void Input_OpenInventory(const FInputActionValue& InputActionValue);
	void Input_PauseGame(const FInputActionValue& InputActionValue);
	void Input_Interact(const FInputActionValue& InputActionValue);
	

	/** Swap Weapon */
	UFUNCTION(BlueprintNativeEvent)
	void Input_SwapToAxe(const FInputActionValue& InputActionValue);
	
	UFUNCTION(BlueprintNativeEvent)
	void Input_SwapToPistol(const FInputActionValue& InputActionValue);
	
	UFUNCTION(BlueprintNativeEvent)
	void Input_SwapToShotgun(const FInputActionValue& InputActionValue);

	/** Attack */
	void Input_Attacking(const FInputActionValue& InputActionValue);
	void MeleeAttacking();
	void RangeAttacking(UAnimMontage* ReloadMontage);
	bool PlayReloadMontage();

	/** Reload Weapon */
	UFUNCTION(BlueprintCallable)
	void Input_Reloading(const FInputActionValue& InputActionValue);
	// End Enhanced Input Sample changes

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults | Stats")
	UPlayerStatsComponent* PlayerStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EActiveWeapon> ActiveWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsSprinting = false;

	// Sprinting
	UPROPERTY(EditDefaultsOnly, Category = "Defaults | Move")
	float SprintSpeed;

	// Walking
	UPROPERTY(EditDefaultsOnly, Category = "Defaults | Move")
	float WalkSpeed;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	FName CameraSocketName;

	// Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ABaseWeapon> AxeWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ABaseWeapon> PistolWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ABaseWeapon> ShotgunWeaponClass;

	// TODO: One Weapon(Weapon in hand)

	
	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	ABaseWeapon* ActiveWeaponref;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	bool bHaveAxe;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	bool bHavePistol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	bool bHaveShotgun;

	UPROPERTY(BlueprintReadWrite)
	bool CanSwapWeapon;

	UPROPERTY(BlueprintReadWrite)
	int Combo;
	
	int PistolDispenserMagazine;
	int ShotgunDispenserMagazine;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName AxeSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName PistolSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName ShotgunSocketName;

	UPROPERTY(BlueprintReadWrite, Category="Weapon")
	bool CanAttack;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UAnimMontage* FirstAttack;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UAnimMontage* SecondAttack;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UAnimMontage* ReloadShotgun;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UAnimMontage* ReloadPistol;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float AttackPlayRate;

	// Pause Game
	UPROPERTY(EditAnywhere, Category="Defaults | Widget")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	class UUserWidget* PauseWidget;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FPSCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Defaults | Input")
	mutable const UInputMappingContext* InputMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;

	float StaminaValue;
};
