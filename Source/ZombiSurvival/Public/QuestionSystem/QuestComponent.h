// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestionSystem/QuestInfo.h"
#include "Blueprint/UserWidget.h"
#include "QuestionSystem/QuestInterface.h"
#include "QuestComponent.generated.h"

class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateWidgetChangeDelegate, FQuest, Quest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestReachedChangeDelegate, EQuestType, Type, TSubclassOf<AActor>, Object);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestItemIteratedChangeDelegate, EQuestType, Type, TSubclassOf<AActor>, Object);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestItemInteractChangeDelegate, EQuestType, Type, TSubclassOf<AActor>, Object);

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

	virtual void StartQuest_Implementation();

	virtual void SetQuestDT_Implementation(const UDataTable* QuestDT);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartQuest();

	UUserWidget* QuestWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TSubclassOf<UUserWidget> QuestWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FQuest QuestData;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	class UDataTable* QuestDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentQuestIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CompletedStepsAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 InitialStepsAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FName> QuestNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FQuest> QuestDataList;
	
	EQuestType Type;

	TSubclassOf<AActor> Object;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnUpdateWidgetChangeDelegate OnUpdateWidget;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestReachedChangeDelegate OnQuestReached;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestItemIteratedChangeDelegate OnQuestItemIterated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnQuestItemInteractChangeDelegate OnQuestItemInteract;
};
