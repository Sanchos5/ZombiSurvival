﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum EItemType : uint8
{
	EI_Patrons UMETA(DisplayName = "Patrons"),
	EI_HealthMedicines UMETA(DisplayName = "HealthMedicines"),
	EI_InfectionMedicines UMETA(DisplayName = "InfectionMedicines"),
	EI_Drinks UMETA(DisplayName = "Drinks"),
	EI_Food UMETA(DisplayName = "Food"),
	EI_NONE UMETA(DisplayName = "NONE")
};