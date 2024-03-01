#pragma once

#include "CoreMinimal.h"
#include "ItemSaveData.generated.h"

USTRUCT(BlueprintType)
struct ZOMBISURVIVAL_API FItemSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;

	FItemSaveData();
	~FItemSaveData();
};
