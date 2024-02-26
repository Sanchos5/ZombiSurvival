// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDSurvival.h"
#include "Widget/PlayerInterface.h"

void AHUDSurvival::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerInterfaceClass != nullptr)
	{
		PlayerInterface = Cast<UPlayerInterface>(CreateWidget(GetWorld(), PlayerInterfaceClass));
		if (PlayerInterface)
		{
			PlayerInterface->AddToViewport();
		}
	}
}
