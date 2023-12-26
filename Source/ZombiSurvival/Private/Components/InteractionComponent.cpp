// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"
#include "Blueprint/UserWidget.h"
#include "Interface/InteractionInterface.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

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
	
	//DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 0.0f);
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	/*if (ensure(InteractionWidgetClass))
	{
		InteractionWidget = CreateWidget(GetWorld(), InteractionWidgetClass);
	}*/
}


void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FindBestInteractable();
}

