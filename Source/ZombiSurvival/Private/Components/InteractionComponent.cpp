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
	//FCollisionObjectQueryParams ObjectQueryParams;
	//ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MyOwner);

	FHitResult HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(CollisionChannel));
	
	
	bool bBlockingHit =  UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), EyeLocation, End, ObjectTypes, true,
		ActorsToIgnore, DrawDebugTrace, HitResult, true);
	

	// Clear ref before trying to fill
	FocusedActor = nullptr;

	AActor* HitActor = HitResult.GetActor();
	if (HitActor)
	{
		if (HitActor->Implements<UInteractionInterface>())
		{
			FocusedActor = HitActor;
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FocusedActor.GetName())
		}
	}

	if (FocusedActor)
	{
		InteractionWidget->InteractionActor = FocusedActor;
		UE_LOG(LogTemp, Error, TEXT("%s"), *InteractionWidget->InteractionActor->GetName())
		
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
