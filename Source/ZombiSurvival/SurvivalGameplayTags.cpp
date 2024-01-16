// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"

FSurvivalGameplayTags FSurvivalGameplayTags::GameplayTags;

void FSurvivalGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

	Manager.DoneAddingNativeTags();
}

void FSurvivalGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InputTag_Move, "InputTag.Move", "Move input.");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	AddTag(InputTag_Jump, "InputTag.Jump", "Jump input");
	AddTag(InputTag_Sprint, "InputTag.Sprint", "Sprint input");
	AddTag(InputTag_Inventory, "InputTag.Inventory", "Inventory input");
	AddTag(InputTag_PauseGame, "InputTag.PauseGame", "PauseGame input");
	AddTag(InputTag_Interaction, "InputTag.Interaction", "Interact input");
	AddTag(InputTag_Attack, "InputTag.Attack", "Attacking input");
	AddTag(InputTag_Reload, "InputTag.Reload", "Reloading input");


}

void FSurvivalGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}