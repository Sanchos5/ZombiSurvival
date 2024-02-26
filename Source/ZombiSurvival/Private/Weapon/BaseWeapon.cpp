// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseWeapon::Attack()
{
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

