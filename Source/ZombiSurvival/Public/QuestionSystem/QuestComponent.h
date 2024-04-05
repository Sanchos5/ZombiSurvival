// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestionSystem/QuestInfo.h"
#include "Blueprint/UserWidget.h"
#include "QuestionSystem/QuestInterface.h"
#include "QuestComponent.generated.h"

class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateWidgetChangeDelegate);

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent) )
class ZOMBISURVIVAL_API UQuestComponent : public UActorComponent, public IQuestInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Quest")
	static UQuestComponent* FindQuestComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UQuestComponent>() : nullptr); }

	UUserWidget* QuestWidget;

	virtual void StartQuest_Implementation();

	void SetQuestDT_Implementation(const UDataTable* QuestDT) override;

	UPROPERTY(VisibleAnywhere)
	TArray<FName> QuestNames;

	UPROPERTY(VisibleAnywhere)
	TArray<FQuest> QuestDataList;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartQuest();

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TSubclassOf<UUserWidget> QuestWidgetClass;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	class UDataTable* QuestDataTable;
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnUpdateWidgetChangeDelegate OnUpdateWidget;
};
