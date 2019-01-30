// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BattleObjectTableInfo.h"

UClass* FBattleObjectTableInfo::GetModelClass()
{
	UClass* ModelUClass = ModelClass.Get();
	if (ModelUClass == nullptr)
	{		
		ModelClass.ToSoftObjectPath().TryLoad();
		ModelUClass = ModelClass.Get();
	}
	return ModelUClass;
}