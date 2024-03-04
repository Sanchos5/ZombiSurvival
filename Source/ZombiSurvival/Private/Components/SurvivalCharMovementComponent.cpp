// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SurvivalCharMovementComponent.h"


float USurvivalCharMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (const ASurvivalPlayer* PlayerOwner = Cast<ASurvivalPlayer>(GetOwner()))
	{
		if (PlayerOwner->bIsSprinting == true && !IsCrouching())
		{
			MaxSpeed = PlayerOwner->SprintSpeed;
		}
		else if (PlayerOwner->bIsSprinting == false)
		{
			MaxSpeed = PlayerOwner->WalkSpeed;
		}
	}

	return MaxSpeed;
}