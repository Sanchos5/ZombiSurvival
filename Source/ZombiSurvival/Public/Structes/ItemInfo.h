// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)	
struct ZOMBISURVIVAL_API FItemInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	FItemInfo();
	~FItemInfo();
};
