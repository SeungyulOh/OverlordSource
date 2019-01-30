// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroInfo_Upgrade_Material.h"

void UUC_HeroInfo_Upgrade_Material::NativeConstruct()
{
	Super::NativeConstruct();

	if (!InventoryScrollView->ScrollItemList.Num())
		InventoryScrollView->Init_NoClear(36);

}

void UUC_HeroInfo_Upgrade_Material::NativeDestruct()
{
	Super::NativeDestruct();
}