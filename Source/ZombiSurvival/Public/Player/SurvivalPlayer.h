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

UCLASS(config = game)
class ZOMBISURVIVAL_API ASurvivalPlayer : public ASurvivalBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivalPlayer(const class FObjectInitializer& ObjectInitializer);

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFPSComponent() const { return FPSCamera; }

	/** The input config that maps Input Actions to Input Tags*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	USurvivalInputConfig* InputConfig;

	//Input
	/** Handles moving forward/backward */
	void Input_Move(const FInputActionValue& InputActionValue);

	/** Handles mouse and stick look */
	void Input_Look(const FInputActionValue& InputActionValue);

	/** Handles Jumping */
	void Input_Jump(const FInputActionValue& InputActionValue);

	/** Open Inventory Widget */
	void Input_OpenInventory(const FInputActionValue& InputActionValue);

	/** Interact with object */
	void Input_PrimaryInteract(const FInputActionValue& InputActionValue);
	
	// End Enhanced Input Sample changes

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	UPlayerStatsComponent* PlayerStats;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	FName CameraSocketName = "CameraSocket";

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FPSCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	mutable const UInputMappingContext* InputMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Mesh", meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;
};
