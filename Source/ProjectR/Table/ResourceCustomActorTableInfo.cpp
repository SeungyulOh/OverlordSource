// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ResourceCustomActorTableInfo.h"

UClass* FResourceCustomActorTableInfo::GetBlueprintClass()
{
	UClass* Class = AssetClass.Get();
	if (Class == nullptr)
	{
		AssetClass.ToSoftObjectPath().TryLoad();
		Class = AssetClass.Get();
	}

	return Class;
}