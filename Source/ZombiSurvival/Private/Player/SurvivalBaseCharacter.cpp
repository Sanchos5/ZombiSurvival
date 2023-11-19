// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SurvivalBaseCharacter.h"

// Sets default values
ASurvivalBaseCharacter::ASurvivalBaseCharacter(const class FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASurvivalBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASurvivalBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}