// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SkillIcon.h"

#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"

#include "UtilFunctionIntegrated.h"
#include "UI/Common/UC_Notify.h"
#include "UI/HeroManagement/UC_Skill.h"
#include "Table/CharacterTableInfo.h"
#include "Table/SkillResourceTableInfo.h"
#include "Table/SkillLevelUpCostTableInfo.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"

void USkillIcon::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillIcon::InvalidateData_SkillInfo(FString& InHeroUD, FHERO_SKILL& InHeroSkillInfo, FSkillResourceTableInfo& InSkillResourceInfo, FWorkedSkillTableInfo& InWorkedSkillTableInfo)
{
	OwnerHeroUD = InHeroUD;
	HeroSkillInfo = InHeroSkillInfo;
	SkillResourceTable		= InSkillResourceInfo;
	WorkedSkillTableInfo	= InWorkedSkillTableInfo;

	Refresh();
}

void USkillIcon::Refresh()
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;

	bDisplay = false;
	UUC_Skill* SkillWidget = FindParentRBaseWidget<UUC_Skill>(this);
	if (IsValid(SkillWidget))
	{
		if (SkillWidget->CurUISkillType == EUISkillTypeEnum::VE_Display)
		{
			bDisplay = true;
		}
	}

	if (IsValid(Image_SkillIcon))
	{
		Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceTable.SkillIconRef));
	}

	if (IsValid(Image_SkillCircle))
	{
		auto Texture = UUIFunctionLibrary::GetTextureSkillCircle(HeroSkillInfo.level);
		Image_SkillCircle->SetBrushFromTexture(Texture);
	}
	
// 	if (IsValid(PanelUnlock))
// 	{
// 		ESlateVisibility PanelUnlockVisible = ESlateVisibility::SelfHitTestInvisible;
// 		if (0 < HeroSkillInfo.level)
// 		{
// 			PanelUnlockVisible = ESlateVisibility::Collapsed;
// 		}
// 		else
// 		{
// 			if (IsValid(Text_Unlocked_SkillLevel))
// 			{
// 				int32  SkillUnlockLevel = 0;
// 				FString UnlockLevelString;
// 				EHERO_SKILL_SLOT SkillSlot = (EHERO_SKILL_SLOT)HeroSkillInfo.slotNo;
// 				switch (SkillSlot)
// 				{
// 				case EHERO_SKILL_SLOT::ActiveSkill01:		UnlockLevelString = "ActiveSkill01";	break;
// 				case EHERO_SKILL_SLOT::ActiveSkill02:		UnlockLevelString = "ActiveSkill02";	break;
// 				case EHERO_SKILL_SLOT::ActiveSkill03:		UnlockLevelString = "ActiveSkill03";	break;
// 				case EHERO_SKILL_SLOT::PassiveSkill01:	UnlockLevelString = "PassiveSkill01";	break;
// 				case EHERO_SKILL_SLOT::PassiveSkill02:	UnlockLevelString = "PassiveSkill02";	break;
// 				}
// 
// 				FName UnlockLevelKey = FName(*UnlockLevelString);
// 				auto UnlockLevelRow = UTableManager::GetInstancePtr()->GetSkillUnlockLevelRow(UnlockLevelKey);
// 				if (UnlockLevelRow)
// 				{
// 					SkillUnlockLevel = UnlockLevelRow->HeroLevel;
// 				}
// 
// 				Text_Unlocked_SkillLevel->SetText(FText::AsNumber(SkillUnlockLevel));
// 			}
// 		}
// 			
// 		PanelUnlock->SetVisibility(PanelUnlockVisible);
// 	}

	int32 DisplayLevel = HeroSkillInfo.level;
	if (1 > DisplayLevel)
		DisplayLevel = 1;

	if (IsValid(Text_SkillLevel))
	{
		Text_SkillLevel->SetText(FText::AsNumber(DisplayLevel));
	}

	if (IsValid(Text_SkillName))
	{
		Text_SkillName->SetText(UUIFunctionLibrary::GetSkillNameText(WorkedSkillTableInfo));
	}

	if (IsValid(Switcher_SkillType))
	{
		

		if (WorkedSkillTableInfo.SkillTableInfo_V2)
		{
			switch (WorkedSkillTableInfo.SkillTableInfo_V2->SkillControlType)
			{
			case ESkillControlTypeEnum::VE_Active:
			Switcher_SkillType->SetActiveWidgetIndex(0);
			break;
			case ESkillControlTypeEnum::VE_Passive:
			Switcher_SkillType->SetActiveWidgetIndex(1);
			break;
			case ESkillControlTypeEnum::VE_Aura:	
			Switcher_SkillType->SetActiveWidgetIndex(2);
			break;
			case ESkillControlTypeEnum::VE_Auto:
			Switcher_SkillType->SetActiveWidgetIndex(3);
			break;
			default:
			Switcher_SkillType->SetActiveWidgetIndex(0);
			break;
			}
// 			if (WorkedSkillTableInfo.SkillTableInfo_V2->SkillControlType == ESkillControlTypeEnum::VE_Passive)
// 				Switcher_SkillType->SetActiveWidgetIndex(1);
// 			else
// 				Switcher_SkillType->SetActiveWidgetIndex(0);
		}
	}
	
	// Currency
	ESlateVisibility CurrencyVisible_1 = ESlateVisibility::Collapsed;
	ESlateVisibility CurrencyVisible_2 = ESlateVisibility::Collapsed;
	ESlateVisibility NotifyVisible = ESlateVisibility::Collapsed;

// 	if (false == URenewal_BattleFunctionLibrary::IsMaxSkillLevel(DisplayLevel))
// 	{
// 		FName SkillLevelUpKey = UUIFunctionLibrary::GetSkillLevelUpKey(this->GetWorld(), OwnerHeroUD, DisplayLevel + 1);
// 		auto SkillLevelUpRow = UTableManager::GetInstancePtr()->GetSkillLevelUpCostRow(SkillLevelUpKey);
// 		if (SkillLevelUpRow  && SkillLevelUpRow->RequireItemQty.IsValidIndex(HeroSkillInfo.slotNo))
// 		{
// 			int32 NeedGold = SkillLevelUpRow->Gold[HeroSkillInfo.slotNo];
// 			FName NeedRequireItem = SkillLevelUpRow->RequireItem;
// 			int32 NeedRequireItemQty = SkillLevelUpRow->RequireItemQty[HeroSkillInfo.slotNo];
// 
// 			bool CheckNotify = true;
// 
// 			// CheckNotify Gold
// 			if (NeedGold > RGameInstance->RInventory->GetGold())
// 			{
// 				CheckNotify = false;
// 			}
// 
// 			if (false == NeedRequireItem.IsNone() && 0 < NeedRequireItemQty)
// 			{
// 				CurrencyVisible_2 = ESlateVisibility::SelfHitTestInvisible;
// 
// 				if (IsValid(Image_NeedItem))
// 				{
// 					auto ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(NeedRequireItem);
// 					if (ItemTableInfo)
// 					{
// 						Image_NeedItem->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(ItemTableInfo->ItemIcon));
// 					}
// 				}
// 				if (IsValid(Text_NeedItemCount))
// 				{
// 					Text_NeedItemCount->SetText(FText::AsNumber(NeedRequireItemQty));
// 				}
// 				// CheckNotify RequireItem
// 				//int32 CurrentRequireItemCount = RGameInstance->RInventory->GetItemDataCount(NeedRequireItem);
// 				int32 CurrentRequireItemCount = 0;
// 				if (NeedRequireItemQty > CurrentRequireItemCount)
// 				{
// 					CheckNotify = false;
// 				}
// 			}
// 
// 			if (CheckNotify)
// 			{
// 				NotifyVisible = ESlateVisibility::SelfHitTestInvisible;
// 			}
// 		}
// 	}

	if (IsValid(PanelNeedCost_1))
	{
		PanelNeedCost_1->SetVisibility(CurrencyVisible_1);
	}
	if (IsValid(PanelNeedCost_2))
	{
		PanelNeedCost_2->SetVisibility(CurrencyVisible_2);
	}

	if (bDisplay)
	{
		if (IsValid(Notify_SkillLevelUp))
		{
			Notify_SkillLevelUp->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IsValid(LevelUpBtn))
		{
			LevelUpBtn->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USkillIcon::OnClick_SkillDetail()
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;

	ESkillCategoryTypeEnum SkillCategoryType = ESkillCategoryTypeEnum::VE_None;
	EHERO_SKILL_SLOT SkillSlot = (EHERO_SKILL_SLOT)HeroSkillInfo.slotNo;
	switch (SkillSlot)
	{
	case EHERO_SKILL_SLOT::ActiveSkill01:		SkillCategoryType = ESkillCategoryTypeEnum::VE_ActiveSkill01;	break;
	case EHERO_SKILL_SLOT::ActiveSkill02:		SkillCategoryType = ESkillCategoryTypeEnum::VE_ActiveSkill02;	break;
	case EHERO_SKILL_SLOT::ActiveSkill03:		SkillCategoryType = ESkillCategoryTypeEnum::VE_ActiveSkill03;	break;
	case EHERO_SKILL_SLOT::PassiveSkill01:	SkillCategoryType = ESkillCategoryTypeEnum::VE_PassiveSkill01;	break;
	case EHERO_SKILL_SLOT::PassiveSkill02:	SkillCategoryType = ESkillCategoryTypeEnum::VE_PassiveSkill02;	break;
	}

	int32 SkillCategory = (int32)SkillCategoryType;
	RGameInstance->EventManager->OnClick_SkillDetail_HeroManagement.Broadcast(SkillCategory);
}

void USkillIcon::SetNotifyState(ENotifyStateFlagEnum InNotifyState)
{
	if (bDisplay)
		return;

	if (IsValid(Notify_SkillLevelUp))
	{
		Notify_SkillLevelUp->NotifyState = InNotifyState;
	}
}
