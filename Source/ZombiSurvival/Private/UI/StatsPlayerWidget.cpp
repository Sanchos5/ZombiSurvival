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
		if (StatsBar)
		{
			float InfectionValue = PlayerBar->GetInfectionPercentage();
			StatsBar->SetPercent(InfectionValue);
		}
		break;
	case EStatsType::Health:
		if(StatsBar)
		{
			float HealthValue = HealthBar->GetHealthPercentage();
			StatsBar->SetPercent(HealthValue);
		}
		break;
	case EStatsType::Hunger:
		if (StatsBar)
		{
			float HungerValue = PlayerBar->GetHungerPercentage();
			StatsBar->SetPercent(HungerValue);
		}
		break;
	case EStatsType::Thirst:
		if (StatsBar)
		{
			float ThirstValue = PlayerBar->GetThirstPercentage();
			StatsBar->SetPercent(ThirstValue);
		}
		break;
	default:
		break;
	}
}