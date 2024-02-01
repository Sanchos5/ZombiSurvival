// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseMeleeWeapon.h"
#include "Components/TraceComponent.h"

ABaseMeleeWeapon::ABaseMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	Start = CreateDefaultSubobject<USceneComponent>("Start");
	Start->SetupAttachment(GetRootComponent());
	
	End = CreateDefaultSubobject<USceneComponent>("End");
	End->SetupAttachment(GetRootComponent());
	
	TraceComponent = CreateDefaultSubobject<UTraceComponent>(TEXT("TraceComponent"));
}

FVector ABaseMeleeWeapon::GetStartLocation()
{
	return Start->GetComponentLocation();
}

FVector ABaseMeleeWeapon::GetEndLocation()
{
	return End->GetComponentLocation();
}

void ABaseMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}