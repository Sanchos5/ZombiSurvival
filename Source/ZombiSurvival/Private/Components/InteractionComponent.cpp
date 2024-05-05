// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"

#include "GameFramework/Character.h"
#include "Interface/InteractionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/InteractionWidget.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	TraceDistance = 150.0f;
	CollisionChannel = ECC_WorldDynamic;
	bHideWidget = false;
}

void UInteractionComponent::PrimaryInteract()
{
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if(IsValid(MyPawn) && IsValid(FocusedActor))
	{
		IInteractionInterface::Execute_Interact(FocusedActor, MyPawn);
	}
}

void UInteractionComponent::FindBestInteractable()
{
	GetWorld()->GetTimerManager().ClearTimer(InteractionTimer);

	FVector EyeLocation;
	FRotator EyeRotation;
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0.f);
	APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0.f);
	if (!IsValid(PlayerCharacter) || !IsValid(PlayerCamera)) return;
	
	PlayerCamera->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PlayerCharacter);

	TArray<FHitResult> HitResults;
	FHitResult HitResult;

	ETraceTypeQuery ObjectType;
	ObjectType = UEngineTypes::ConvertToTraceType(CollisionChannel);

	bool bBlock = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), EyeLocation, End, TraceRadius, ObjectType, true,
	ActorsToIgnore, DrawDebugTrace, HitResult, true);

	AActor* LastActor = FocusedActor;
	

	// Clear ref before trying to fill
	FocusedActor = nullptr;
	
	if (bBlock)
	{
		AActor* HitActor = HitResult.GetActor();
		
		if (HitActor)
		{
			if (HitActor->Implements<UInteractionInterface>())
			{
				FocusedActor = HitActor;
			}
		}
	}	
	

	if (FocusedActor)
	{
		InteractionWidget->InteractionActor = FocusedActor;
		
		if (IsValid(InteractionWidget) && !InteractionWidget->IsInViewport() && !Cast<IInteractionInterface>(FocusedActor)->Execute_HideWidget(FocusedActor) && bHideWidget == false)
		{
			InteractionWidget->AddToViewport();
		}
		else if (Cast<IInteractionInterface>(FocusedActor)->Execute_HideWidget(FocusedActor) || bHideWidget == true)
		{
			InteractionWidget->RemoveFromParent();
		}
	}
	else
	{
		if (InteractionWidget != nullptr)
		{
			InteractionWidget->RemoveFromParent();
		}
	}

	if (LastActor != FocusedActor)
	{
		if (LastActor) Cast<IInteractionInterface>(LastActor)->Execute_UnHighlightActor(LastActor);
		if (FocusedActor) Cast<IInteractionInterface>(FocusedActor)->Execute_HighlightActor(FocusedActor);
	}

	StartInteractionTimer();	
}

void UInteractionComponent::StartInteractionTimer()
{
	GetWorld()->GetTimerManager().SetTimer(InteractionTimer, this, &UInteractionComponent::FindBestInteractable, 0.05f, false);
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(InteractionWidgetClass))
	{
		InteractionWidget = Cast<UInteractionWidget>(CreateWidget(GetWorld(), InteractionWidgetClass));
	}
	StartInteractionTimer();
}
