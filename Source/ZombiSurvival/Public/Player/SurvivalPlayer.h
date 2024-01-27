// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SurvivalBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "SurvivalPlayer.generated.h"

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
class UUserWidget;

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
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	USurvivalInputConfig* InputConfig;

	/** Handles moving forward/backward */
	void Input_Move(const FInputActionValue& InputActionValue);

	/** Handles mouse and stick look */
	void Input_Look(const FInputActionValue& InputActionValue);

	/** Handles Jumping */
	void Input_Jump(const FInputActionValue& InputActionValue);
	
	/** ������ ���� */
	void Input_StartSprinting(const FInputActionValue& InputActionValue);

	/** ��� ���������� */
	void Input_StopSprinting(const FInputActionValue& InputActionValue);

	/** Open Inventory */
	void Input_OpenInventory(const FInputActionValue& InputActionValue);

	/** Open PauseWidget */
	void Input_PauseGame(const FInputActionValue& InputActionValue);

	/** Interact with objects */
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
	void Input_MeleeAttacking();

	/** ������ ����������� */
	void Input_StartReloading(const FInputActionValue& InputActionValue);

	/** ����� ����������� */
	void Input_StopReloading(const FInputActionValue& InputActionValue);
	
	// End Enhanced Input Sample changes

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	UPlayerStatsComponent* PlayerStats;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage() const { return Health / MaxHealth; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EActiveWeapon> ActiveWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	FName CameraSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsSprinting = false;


	// Weapon
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ABaseMeleeWeapon> AxeWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ABaseRangeWeapon> PistolWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ABaseRangeWeapon> ShotgunWeaponClass;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	ABaseMeleeWeapon* MeleeWeaponref;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	ABaseRangeWeapon* RangeWeaponref;

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
	int ShotgubDispenserMagazine;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName AxeSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName PistolSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName ShotgunSocketName;

	UPROPERTY(BlueprintReadWrite, Category="Weapon")
	bool CanAttack;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UAnimMontage* ComboAttack1;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UAnimMontage* ComboAttack2;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float AttackPlayRate;

	// Pause Game
	void CreatePauseWidget();
	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* PauseWidget;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FPSCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	mutable const UInputMappingContext* InputMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;

	float StaminaValue;
};
