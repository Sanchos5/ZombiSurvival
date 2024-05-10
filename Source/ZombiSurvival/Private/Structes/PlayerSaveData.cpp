#include "Structes/PlayerSaveData.h"

FPlayerSaveData::FPlayerSaveData()
{
	Health = 0.f;
	Stamina = 0.f;
	Infection = 0.f;
	Hunger = 0.f;
	Thirst = 0.f;
	PlayerTransform = FTransform(FQuat(0.f), FVector(0.f),FVector(0.f));
	ControllerRotation = FRotator(0.f);
	Inventory.Empty();
	ActiveWeapon = NONE;
	bHaveAxe = false;
	bHavePistol = false;
	bHaveShotgun = false;
	CurrentStepIndex = 0;
}

FPlayerSaveData::~FPlayerSaveData()
{
}
