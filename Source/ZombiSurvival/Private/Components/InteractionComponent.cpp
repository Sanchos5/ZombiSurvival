// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"
#include "Interface/InteractionInterface.h"
#include "Widget/InteractionWidget.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	TraceRadius = 20.0f;
	TraceDistance = 150.0f;
	CollisionChannel = ECC_WorldDynamic;
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
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	

	// Clear ref before trying to fill
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UInteractionInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (IsValid(InteractionWidget) && !InteractionWidget->IsInViewport())
		{
			InteractionWidget->InteractionActor = FocusedActor;
			InteractionWidget->AddToViewport();
		}
	}
	else
	{
		if (InteractionWidget != nullptr)
		{
			InteractionWidget->RemoveFromParent();
		}
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
