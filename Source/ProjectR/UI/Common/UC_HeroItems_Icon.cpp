// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroItems_Icon.h"

#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "Table/ResourceTexture2DTableInfo.h"

#include "UtilFunctionIntegrated.h"
#include "UC_ItemIcon.h"


void UUC_HeroItems_Icon::SetEquipPosition(EItemEquipPositionEnum InEquipPosition)
{
	EquipPosition = InEquipPosition;

	// Update empty image
	UObject* EmptyIconTexture = UUIFunctionLibrary::GetItemEmptyTextureByEquipPosition(EquipPosition);
	UUIFunctionLibrary::SetImageBrush(EmptyIconTexture, Image_EmptyIcon);
}

void UUC_HeroItems_Icon::InvalidateData()
{
	Super::InvalidateData();

	if (ItemUD.IsEmpty())
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		// temp measure - hide all
		SetVisibility(ESlateVisibility::Visible);

		ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemIcon->RefreshEquipmentIcon(ItemUD);
	}
}
