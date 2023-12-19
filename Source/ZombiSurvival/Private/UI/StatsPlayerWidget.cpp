// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatsPlayerWidget.h"
#include "Components/PlayerStatsComponent.h"
#include "Player/SurvivalPlayer.h"


void UStatsPlayerWidget::UpdateProgressBar()
{
	switch (EStatsType)
	{
	case EStatsType::none:
		break;
	case EStatsType::Infection:
		break;
	case EStatsType::Health:
		break;
	case EStatsType::Hunger:
		break;
	case EStatsType::Thirst:
		break;
	default:
		break;
	}
}