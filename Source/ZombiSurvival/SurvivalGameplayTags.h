#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

/**
 * FSurvivalGameplayTags
 *
 *	Singleton containing native gameplay tags.
 */

struct FSurvivalGameplayTags
{
public:

	static const FSurvivalGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	///////////////////////////////////////
	//Input Action Tags
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Sprint;
	FGameplayTag InputTag_Inventory;
	FGameplayTag InputTag_PauseGame;
	FGameplayTag InputTag_Interaction;
	FGameplayTag InputTag_Attack;
	FGameplayTag InputTag_Reload;
	FGameplayTag InputTag_SwapToAxe;
	FGameplayTag InputTag_SwapToPistol;
	FGameplayTag InputTag_SwapToShotgun;

	//////////////////////////////////////
	//Item Tags


protected:

	//Registers all of the tags with the GameplayTags Manager
	void AddAllTags(UGameplayTagsManager& Manager);

	//Helper function used by AddAllTags to register a single tag with the GameplayTags Manager
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:

	static FSurvivalGameplayTags GameplayTags;
};

