// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SurvivalBaseCharacter.h"
#include "SurvZombiCharacter.generated.h"

class USkeletalMeshComponent;
class UPlayerStatsComponent;
/**
 * 
 */

UCLASS()
class ZOMBISURVIVAL_API ASurvZombiCharacter : public ASurvivalBaseCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ASurvZombiCharacter(const class FObjectInitializer& ObjectInitializer);

protected:

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;
};
