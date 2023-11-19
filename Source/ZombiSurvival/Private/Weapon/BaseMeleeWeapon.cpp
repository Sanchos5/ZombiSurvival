// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseMeleeWeapon.h"

#include "Components/TraceComponent.h"

ABaseMeleeWeapon::ABaseMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	
	TraceComponent = CreateDefaultSubobject<UTraceComponent>(TEXT("TraceComponent"));
}

void ABaseMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ABaseMeleeWeapon::GetWeaponSocketLocation(FName SocketName)
{
	return SkeletalMesh->GetSocketLocation(SocketName);
}

