#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Actor/InventoryItem.h"
#include "Structes/ItemInfo.h"
#include "QuestInfo.generated.h"

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	None,
	Iterate,
	Interact,
	Reach,
	Consume,
};

USTRUCT(BlueprintType)
struct FQuestReach
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Objects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCompleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType Type = EQuestType::Reach;
};

USTRUCT(BlueprintType)
struct FQuestIterate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInventoryItem> Objects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCompleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType Type = EQuestType::Iterate;
};

USTRUCT(BlueprintType)
struct FQuestInteract : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInventoryItem> Objects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCompleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType Type = EQuestType::Interact;
};

USTRUCT(BlueprintType)
struct FQuestSteps
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestReach> Reach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestIterate> Iterate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestInteract> Interact;
};

// Main quests for the data table
USTRUCT(BlueprintType)
struct FQuest : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuestSteps Steps;
};
//

USTRUCT(BlueprintType)
struct FQuestProgression
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CompletedSteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentQuestIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuest CurrentQuestData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGoNextChapter = false;
};