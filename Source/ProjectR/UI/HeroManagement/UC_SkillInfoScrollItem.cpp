// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_SkillInfoScrollItem.h"
#include "GlobalIntegrated.h"

#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "UtilFunctionIntegrated.h"
#include "UC_SkillManagement.h"
#include "UI/HeroManagement/SkillInfoScrollItemData.h"
#include "UI/Common/UC_Notify.h"

void UUC_SkillInfoScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
	if(Button_Select)
		Button_Select->OnClicked.AddDynamic(this, &UUC_SkillInfoScrollItem::SelectDelegate);
}

void UUC_SkillInfoScrollItem::NativeDestruct()
{
	if (Button_Select)
		Button_Select->OnClicked.RemoveDynamic(this, &UUC_SkillInfoScrollItem::SelectDelegate);
	Super::NativeDestruct();
}

void UUC_SkillInfoScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	USkillInfoScrollItemData* SkillInfoItemData = Cast<USkillInfoScrollItemData>(item);
	if (IsValid(SkillInfoItemData))
	{
		if (SkillInfoItemData->bSpiritSkill)
			SetSpiritSkillInfo(SkillInfoItemData);
		else
			SetSkillInfo(SkillInfoItemData);
	}
}

void UUC_SkillInfoScrollItem::SetSkillInfo(USkillInfoScrollItemData* SkillInfoItemData)
{
	FSkillTableInfo_V2* SkillInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillInfoItemData->SkillNameTableId);
	if (SkillInfo)
	{
		FSkillResourceTableInfo* SkillResourceRow = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillInfo->SkillResourceID);
		if (SkillResourceRow)
		{
			if (IsValid(SkillImage))
			{
				SkillImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceRow->SkillIconRef));
			}
		}

		if (IsValid(Text_SkillLevel))
		{
			int32 level = SkillInfoItemData->SkillLevel;
			if (level == 0)
				level = 1;

			Text_SkillLevel->SetText(FText::AsNumber(level));
		}

		if (IsValid(Text_SkillType) && IsValid(Text_SkillCategory))
		{
			switch (SkillInfoItemData->SkillSlot)
			{
			case EHERO_SKILL_SLOT::ActiveSkill01:
				Text_SkillType->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Type_Info_NormalSKill")));
				Text_SkillCategory->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Category_Active1")));
				break;
			case EHERO_SKILL_SLOT::ActiveSkill02:
				Text_SkillType->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("ui_skill_type_info_CoreSkill")));
				Text_SkillCategory->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Category_Active2")));
				break;
			case EHERO_SKILL_SLOT::ActiveSkill03:
				Text_SkillType->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Type_Info_CombineSkill")));
				Text_SkillCategory->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Category_Active3")));
				break;
			case EHERO_SKILL_SLOT::PassiveSkill01:
				Text_SkillType->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Type_Info_UniqueSkill01")));
				Text_SkillCategory->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Category_Passive1")));
				break;
			case EHERO_SKILL_SLOT::PassiveSkill02:
				Text_SkillType->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Type_Info_UniqueSkill02")));
				Text_SkillCategory->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Category_Passive2")));
				break;
				// 				case EHERO_SKILL_SLOT:://spiritskill:
				// 					break;
			}
		}

		if (IsValid(Text_SkillName))
		{
			Text_SkillName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SkillInfo->SkillName));
		}

		if (IsValid(LockPanelWidget))
		{
			if (SkillInfoItemData->SkillLevel == 0)
			{
				LockPanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				LockPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUC_SkillInfoScrollItem::SetSpiritSkillInfo(USkillInfoScrollItemData* SkillInfoItemData)
{
	FSkillTableInfo_V2* SkillInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(SkillInfoItemData->SkillNameTableId);
	if (SkillInfo)
	{
		FSkillResourceTableInfo* SkillResourceRow = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillInfo->SkillResourceID);
		if (SkillResourceRow)
		{
			if (IsValid(SkillImage))
			{
				SkillImage->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceRow->SkillIconRef));
			}
		}

		if (IsValid(Text_SkillLevel))
		{
			int32 level = SkillInfoItemData->SkillLevel;
			if (level == 0)
				level = 1;

			Text_SkillLevel->SetText(FText::AsNumber(level));
		}

		if (IsValid(Text_SkillType) && IsValid(Text_SkillCategory))
		{
			Text_SkillType->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Type_Info_SpiritPassive")));
			Text_SkillCategory->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Category_SpiritPassive")));
		}

		if (IsValid(Text_SkillName))
		{
			Text_SkillName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SkillInfo->SkillName));
		}

		if (IsValid(LockPanelWidget))
		{
			if (SkillInfoItemData->SkillLevel == 0)
			{
				LockPanelWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				LockPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUC_SkillInfoScrollItem::SetSelected(bool bSelected)
{
// 	if (bSelected)
// 		Image_Select->SetVisibility(ESlateVisibility::Visible);
// 	else
// 		Image_Select->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_SkillInfoScrollItem::SelectDelegate()
{
	ManagerUI->OnSelectScrollItem(ItemData);
	Image_Select->SetVisibility(ESlateVisibility::Visible);
}
