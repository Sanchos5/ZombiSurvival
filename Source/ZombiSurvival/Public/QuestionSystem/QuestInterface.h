// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestInfo.h"
#include "QuestInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQuestInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IQuestInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateQuest(const EQuestType& Type, TSubclassOf<AActor> Object);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartQuest();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CompletedQuest();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetQuestDT(const UDataTable* Quest);
};
