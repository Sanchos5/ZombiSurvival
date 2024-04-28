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

	//Dynamicmaterial = Mesh->CreateDynamicMaterialInstance(0, Material, "AxeDecal");

	

	Dynamicmaterial = UMaterialInstanceDynamic::Create(Material, this);
	Mesh->SetMaterial(0, Dynamicmaterial);

	//Dynamicmaterial->SetScalarParameterValue("AxeDecal", 1.0f);
}

void ABaseMeleeWeapon::GetDynamicMaterial(float ParameterValue)
{
	Dynamicmaterial->SetScalarParameterValue("AxeDecal", ParameterValue);
}