// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_LevelUp.h"




void UUC_LevelUp::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAni(TEXT("LvUp_Anim"));
}
