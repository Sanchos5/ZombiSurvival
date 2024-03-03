// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseActor.h"

ABaseActor::ABaseActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

