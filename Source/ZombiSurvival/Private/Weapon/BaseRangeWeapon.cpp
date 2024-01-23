// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseRangeWeapon.h"

ABaseRangeWeapon::ABaseRangeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseRangeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseRangeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

