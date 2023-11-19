// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SurvivalPlayer.h"
#include "EnhancedInputComponent.h"
#include "Input/SurvivalInputConfig.h"
#include "Input/SurvivalInputComponent.h"
#include "ZombiSurvival/SurvivalGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "Components/SurvivalCharMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASurvivalPlayer::ASurvivalPlayer(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USurvivalCharMovementComponent>(ASurvivalBaseCharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);

	// Create a CameraComponent	
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPSCamera->bUsePawnControlRotation = true;
	FPSCamera->SetupAttachment(GetMesh());
}

void ASurvivalPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	FPSCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, CameraSocketName);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
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
	SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Triggered, this, &ASurvivalPlayer::Input_Jump);
	//SurvivalInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Fire, ETriggerEvent::Triggered, this, &ASurvivalPlayer::Input_Fire);

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
	Jump();
}