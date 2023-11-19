// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TraceComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Weapon/BaseMeleeWeapon.h"

UTraceComponent::UTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTraceComponent::TraceHit()
{
	if (Weapon == nullptr) return;

	FVector Start = Weapon->GetWeaponSocketLocation(StartTraceName);
	FVector End = Weapon->GetWeaponSocketLocation(EndTraceName);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;
	Objects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	ActorsToIgnore.Add(GetOwner());
	TArray<FHitResult> SphereHitResults;
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, TraceRadius, Objects, false,
		ActorsToIgnore, EDrawDebugTrace::None, SphereHitResults, true);

	for (FHitResult HitResult : SphereHitResults)
	{
		AActor* Enemy = HitResult.GetActor();
		if (Enemy && !ActorsToIgnore.Contains(Enemy))
		{	
			ActorsToIgnore.Add(Enemy);
		}
	}
}

void UTraceComponent::ClearActorsToIgnore()
{
	ActorsToIgnore.Empty();
}

void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (WeaponActive)
	{
		TraceHit();
	}
}

