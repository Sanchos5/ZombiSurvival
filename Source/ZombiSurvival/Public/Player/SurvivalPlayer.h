// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SurvivalBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "SurvivalPlayer.generated.h"

class UInteractionComponent;
class UInventoryComponent;
class UCameraComponent;
class UInputComponent;
class USurvivalInputConfig;
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
	bool bOpenInventory = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsSprinting = false;

	// Melee Weapon
	void CreateWeapon();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Melee Weapon")
	TSubclassOf<class ABaseMeleeWeapon> MeleeWeaponClass;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponSocketName;

	UPROPERTY(BlueprintReadWrite, Category="Melee Weapon")
	bool CanAttack;

	UPROPERTY(EditDefaultsOnly, Category="Melee Weapon")
	UAnimMontage* MeleeAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category="Melee Weapon")
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
