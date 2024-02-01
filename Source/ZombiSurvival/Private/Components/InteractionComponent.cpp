// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"
#include "Blueprint/UserWidget.h"
#include "Interface/InteractionInterface.h"

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

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// Clear ref before trying to fill
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 0.0f);
		
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
	//DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 0.0f);
}

void UInteractionComponent::StartInteractionTimer()
{
	GetWorld()->GetTimerManager().SetTimer(InteractionTimer, this, &UInteractionComponent::FindBestInteractable, 0.05f, false);
}

void UInteractionComponent::StopInteractionTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(InteractionTimer);
	FocusedActor = nullptr;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(InteractionWidgetClass))
	{
		InteractionWidget = CreateWidget(GetWorld(), InteractionWidgetClass);
	}
	StartInteractionTimer();
}
