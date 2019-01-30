// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ScrollItem_AvatarIcon.h"

#include "GlobalIntegrated.h"

#include "WidgetSwitcher.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Avatar/Item/ItemData_AvatarIcon.h"

void UUC_ScrollItem_AvatarIcon::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_ScrollItem_AvatarIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_ScrollItem_AvatarIcon::SetAvatarIconScrollItemData()
{
	if (IsValid(ItemData) == false)
		return;

	auto ScrollData = Cast<UItemData_AvatarIcon>(ItemData);
	if (IsValid(ScrollData) == false)
		return;

// 	if (Image_Hero)
// 	{
// 		auto Image = UUIFunctionLibrary::GetIconImageFromTable(ScrollData->Image_Hero);
// 		UUIFunctionLibrary::SetImageBrush(Image, Image_Hero);
// 	}

	//TableKey

// 	if (Image_Lock)
// 	{
// 		IsLock = !(ScrollData->bSelectEnable);
// 		if (IsLock)
// 		{
// 			Image_Lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		}
// 		else
// 		{
// 			Image_Lock->SetVisibility(ESlateVisibility::Collapsed);
// 		}
// 	}
// 
// 	if (Overlay_Select)
// 	{
// 
// 	}
}

void UUC_ScrollItem_AvatarIcon::SetGuildGradeIcon(int32 grade)
{
	WidgetSwitcher_GradeIcon->SetActiveWidgetIndex(grade);
}

void UUC_ScrollItem_AvatarIcon::ClickIcon()
{

}

void UUC_ScrollItem_AvatarIcon::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	SetAvatarIconScrollItemData();
}

void UUC_ScrollItem_AvatarIcon::SetSelected(bool bSelected)
{
// 	if (bSelected)
// 		Overlay_Select2->SetVisibility(ESlateVisibility::HitTestInvisible);
// 	else
// 		Overlay_Select2->SetVisibility(ESlateVisibility::Collapsed);
}
