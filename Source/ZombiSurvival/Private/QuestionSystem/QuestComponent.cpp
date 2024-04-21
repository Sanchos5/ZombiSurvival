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
	
	//OnQuestReached.Broadcast(Type, Object);
	//OnQuestItemIterated.Broadcast(Type, Object);
	//OnQuestItemInteract.Broadcast(Type, Object);
}

void UQuestComponent::StartQuest_Implementation()
{
	OnUpdateWidget.Broadcast(QuestData);
}

// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	if(QuestDataTable)
	{
		if (QuestWidgetClass)
		{
			//QuestWidget = CreateWidget(GetWorld(), QuestWidgetClass);
			//QuestWidget->AddToViewport();
			OnUpdateWidget.Broadcast(QuestData);
		}

		//SetQuestDT_Implementation(QuestDataTable);
	}
}
