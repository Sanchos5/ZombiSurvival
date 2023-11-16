// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SurvivalAssetManager.h"
#include "ZombiSurvival/SurvivalGameplayTags.h"

USurvivalAssetManager::USurvivalAssetManager()
{

}

USurvivalAssetManager& USurvivalAssetManager::Get()
{
	check(GEngine);

	if (USurvivalAssetManager* Singleton = Cast<USurvivalAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	return *NewObject<USurvivalAssetManager>();
}

void USurvivalAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	//Load Native Tags
	FSurvivalGameplayTags::InitializeNativeTags();
}