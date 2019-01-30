// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ResourceSequenceTableInfo.h"

UClass* FResourceSequenceTableInfo::GetBlueprintClass()
{
	UClass* Class = SequenceClass.Get();
	if (Class == nullptr)
	{
		UE_LOG(LogGamePlay, Log, TEXT(" <><><><><><><> Get asset id : %s"), *(SequenceClass.ToSoftObjectPath().ToString()));
		
		SequenceClass.ToSoftObjectPath().TryLoad();
		Class = SequenceClass.Get();
	}

	return Class;
}