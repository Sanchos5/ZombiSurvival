﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Structes/ItemInfo.h"

FItemInfo::FItemInfo()
{
	StackSize = 1;
	NumbertoRestore = 0.f;
	NumbertoSubtract = 0.f;
	ItemType = EI_NONE;
	ScaleItem = FVector(1.f);
}

FItemInfo::~FItemInfo()
{
}
