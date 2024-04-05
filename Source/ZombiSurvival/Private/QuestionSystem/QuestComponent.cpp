// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestionSystem/QuestComponent.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UQuestComponent::SetQuestDT_Implementation(const UDataTable* QuestDT)
{
	//FQuest Quest;
	for(int i = 0; i < QuestDataList.Num(); i++)
	{
		QuestDataList[i];
	}
}

void UQuestComponent::StartQuest_Implementation()
{
	OnUpdateWidget.Broadcast();
}

// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	if(QuestDataTable)
	{
		if (QuestWidgetClass)
		{
			QuestWidget = CreateWidget(GetWorld(), QuestWidgetClass);
			QuestWidget->AddToViewport();

			StartQuest_Implementation();
		}

		SetQuestDT_Implementation(QuestDataTable);
	}
}