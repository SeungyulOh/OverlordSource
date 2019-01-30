// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_SkillDetail.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"


#include "GameInfo/RInventory.h"

#include "UI/HeroManagement/UC_SkillManagement.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Common/TopBar.h"
#include "UI/SHop/UP_Shop.h"
#include "UI/UI_Common//UP_CommonPopup.h"

#include "UtilFunctionIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "Network/PacketFunctionLibrary.h"

const int32 Active3ReqGrade = 4;
const int32 Passive1ReqGrade = 5;
const int32 Passive2ReqGrade = 10;

void UUC_SkillDetail::NativeConstruct()
{
	Super::NativeConstruct();

	SkillLevelUpBtn->OnClicked.RemoveDynamic(this, &UUC_SkillDetail::OnClick_SkillLevelUp);
	SkillLevelUpBtn->OnClicked.AddDynamic(this, &UUC_SkillDetail::OnClick_SkillLevelUp);

	Button_Close->OnClicked.Clear();
	Button_Close->OnClicked.AddUniqueDynamic(this, &UUC_SkillDetail::OnClick_Close);

	ColorTextArray.Emplace(Text_SkillLevel);
	ColorTextArray.Emplace(Text_Lv);
	ColorTextArray.Emplace(Text_SkillName);
	ColorTextArray.Emplace(Text_CoolTime);
	ColorTextArray.Emplace(TextC);
	ColorTextArray.Emplace(TextSkillRefreshTime);
	ColorTextArray.Emplace(TextSecond);
	ColorTextArray.Emplace(Text_SkillDesc);

	for (size_t i = 0; i < ColorTextArray.Num(); ++i)
		ColorArray.Emplace(ColorTextArray[i]->ColorAndOpacity);
}

void UUC_SkillDetail::InvalidateSkillDetail(const FString& InHeroUD, const FHERO_SKILL& InHeroSkillInfo, const FSkillResourceTableInfo& InSkillResourceInfo, const FSkillTableInfo_V2& InSkillInfoV2)
{
	auto RGameInstance = RGAMEINSTANCE(this);

	bSpiritSkill = false;

	CurHeroUD						= InHeroUD;
	HeroSkillInfo					= InHeroSkillInfo;
	SkillResourceTableInfo			= InSkillResourceInfo;	
	SkillTableInfo_V2				= InSkillInfoV2;
	
	auto HeroInvenData = RGameInstance->RInventory->GetHeroWithUD(CurHeroUD);
	if (HeroInvenData)
	{
		CurHeroID = HeroInvenData->heroId;
		bHeroActive = true;
	}

	SkillCategoryType = static_cast<ESkillCategoryTypeEnum>(InHeroSkillInfo.slotNo);
	CurrentSkillLevel = HeroSkillInfo.level;
	if (MAX_Skill_LEVEL < CurrentSkillLevel)
		CurrentSkillLevel = MAX_Skill_LEVEL;

	CurHeroDisplayName = "";

	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CurHeroID);

	FString strkey;
	if (CharacterTableInfo->Class == EClassTypeEnum::VE_Tanker)
		strkey = TEXT("VE_Tanker_") + FString::FromInt(CurrentSkillLevel+1);
	else if (CharacterTableInfo->Class == EClassTypeEnum::VE_MeleeDealer)
		strkey = TEXT("VE_MeleeDealer_") + FString::FromInt(CurrentSkillLevel+1);
	else if (CharacterTableInfo->Class == EClassTypeEnum::VE_MagicDealer)
		strkey = TEXT("VE_MagicDealer_") + FString::FromInt(CurrentSkillLevel+1);
	else if (CharacterTableInfo->Class == EClassTypeEnum::VE_RangeDealer)
		strkey = TEXT("VE_RangeDealer_") + FString::FromInt(CurrentSkillLevel+1);
	else if (CharacterTableInfo->Class == EClassTypeEnum::VE_Supporter)
		strkey = TEXT("VE_Supporter_") + FString::FromInt(CurrentSkillLevel+1);

	UTableManager* TableMgr = UTableManager::GetInstancePtr();
	if (!TableMgr)
		return;

	FSkillLevelUpCostTableInfo* SkillLvUpCostTable = TableMgr->GetSkillLevelUpCostRow(FName(*strkey));
	if (SkillLvUpCostTable)
	{
		// RequireData
		NeedSkillUpgradeGold = SkillLvUpCostTable->Gold[HeroSkillInfo.slotNo];
		NeedRequireItem = SkillLvUpCostTable->RequireWalletId;
		NeedRequireItemCount = SkillLvUpCostTable->RequireWalletQty[HeroSkillInfo.slotNo];

		CurrentRequireItemCount = RGameInstance->RInventory->GetCurrencyWithKey(NeedRequireItem);

		if (CurrentRequireItemCount > NeedRequireItemCount)
			CurrentRequireItemCount = NeedRequireItemCount;

	}
	RefreshUI();
}




void UUC_SkillDetail::InvalidateSpiritSkillDetail(const FString& InHeroUD, const FHERO_SKILL& InHeroSkillInfo, const FSkillResourceTableInfo& InSkillResourceInfo, const FSpiritSkillTableInfo& InSkillInfoV2)
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 	{
// 		return;
// 	}

	bSpiritSkill = true;

	CurHeroUD = InHeroUD;
	HeroSkillInfo = InHeroSkillInfo;
	SkillResourceTableInfo = InSkillResourceInfo;
	SpiritSkillTableInfo = InSkillInfoV2;

	auto HeroInvenData = RGameInstance->RInventory->GetHeroWithUD(CurHeroUD);
	if (HeroInvenData)
	{
		CurHeroID = HeroInvenData->heroId;
		bHeroActive = true;
	}

	CurrentSkillLevel = HeroSkillInfo.level;
	if (MAX_Skill_LEVEL < CurrentSkillLevel)
		CurrentSkillLevel = MAX_Skill_LEVEL;

	CurHeroDisplayName = "";

	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CurHeroID);

	FString strkey;
	if (CharacterTableInfo->Class == EClassTypeEnum::VE_Tanker)
		strkey = TEXT("VE_Tanker_") + FString::FromInt(CurrentSkillLevel + 1);
	else if (CharacterTableInfo->Class == EClassTypeEnum::VE_MeleeDealer)
		strkey = TEXT("VE_MeleeDealer_") + FString::FromInt(CurrentSkillLevel + 1);
	else if (CharacterTableInfo->Class == EClassTypeEnum::VE_MagicDealer)
		strkey = TEXT("VE_MagicDealer_") + FString::FromInt(CurrentSkillLevel + 1);
	else if (CharacterTableInfo->Class == EClassTypeEnum::VE_RangeDealer)
		strkey = TEXT("VE_RangeDealer_") + FString::FromInt(CurrentSkillLevel + 1);
	else if (CharacterTableInfo->Class == EClassTypeEnum::VE_Supporter)
		strkey = TEXT("VE_Supporter_") + FString::FromInt(CurrentSkillLevel + 1);

	UTableManager* TableMgr = UTableManager::GetInstancePtr();
	if (!TableMgr)
		return;

	FSkillLevelUpCostTableInfo* SkillLvUpCostTable = TableMgr->GetSkillLevelUpCostRow(FName(*strkey));
	if (SkillLvUpCostTable)
	{
		// RequireData
		NeedSkillUpgradeGold = SkillLvUpCostTable->Gold[HeroSkillInfo.slotNo];
		NeedRequireItem = SkillLvUpCostTable->RequireWalletId;
		NeedRequireItemCount = SkillLvUpCostTable->RequireWalletQty[HeroSkillInfo.slotNo];

		CurrentRequireItemCount = RGameInstance->RInventory->GetCurrencyWithKey(NeedRequireItem);
	}

	RefreshUI();
}

void UUC_SkillDetail::RefreshUI()
{
	if (IsValid(Text_SkillName))
	{
		if (bSpiritSkill)
			Text_SkillName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkillTableInfo.SkillName));
		else
			Text_SkillName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SkillTableInfo_V2.SkillName));
	}

	if (IsValid(Text_SkillLevel))
	{
		int32 level = HeroSkillInfo.level;
		if (level == 0)
			level = 1;
		Text_SkillLevel->SetText(FText::AsNumber(level));
	}

	if (IsValid(Text_CurrentLevelText))
	{
		FText desc = UUIFunctionLibrary::GetSkillDescTextHeroID(CurHeroID, FName(*HeroSkillInfo.skillId), HeroSkillInfo.level == 0 ? 1 : HeroSkillInfo.level);
		Text_CurrentLevelText->SetRichText(desc);
	}

	if (IsValid(Text_NextLevelText))
	{
		FText desc = UUIFunctionLibrary::GetSkillDescTextHeroID(CurHeroID, FName(*HeroSkillInfo.skillId), HeroSkillInfo.level == 0 ? 2 : HeroSkillInfo.level + 1);
		Text_NextLevelText->SetRichText(desc);
	}

	FName skillID = UUtilFunctionLibrary::GetResultSkillID(ECharacterCategoryTypeEnum::VE_Hero, FName(*HeroSkillInfo.skillId), HeroSkillInfo.level == 0 ? 1 : HeroSkillInfo.level);

	
	
	if (IsValid(Text_SkillDesc))
	{
		if(bSpiritSkill)
			Text_SkillDesc->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SpiritSkillTableInfo.Summary));
		else
			Text_SkillDesc->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, SkillTableInfo_V2.Summary));
	}

	if (IsValid(Text_SkillType))
	{
		Text_SkillType->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Skill_Category_SpiritPassive")));
	}

	if (IsValid(TextSkillRefreshTime))
	{
		if(bSpiritSkill)
			TextSkillRefreshTime->SetText(FText::AsNumber(SpiritSkillTableInfo.CrewCoolTime));
		else
			TextSkillRefreshTime->SetText(FText::AsNumber(SkillTableInfo_V2.CoolTime));
	}

	if (IsValid(Image_AttackRange))
	{
		FString strId = TEXT("Icon_SkillShape_");
		//FText text;
		ESkillShapedIconType shape = bSpiritSkill ? SpiritSkillTableInfo.UISkillShape : SkillTableInfo_V2.UISkillShape;
		switch (shape)
		{
		case ESkillShapedIconType::VE_Target:
			strId += TEXT("Target");
			//text = FText::FromString(TEXT("Target"));
			break;
		case ESkillShapedIconType::VE_Circle:
			strId += TEXT("Circle");
			//text = FText::FromString(TEXT("Circle"));
			break;
		case ESkillShapedIconType::VE_Box:
			strId += TEXT("Box");
			//text = FText::FromString(TEXT("Box"));
			break;
		case ESkillShapedIconType::VE_Sector:
			strId += TEXT("Sector");
			//text = FText::FromString(TEXT("Sector"));
			break;
		case ESkillShapedIconType::VE_TargetLocation:
			strId += TEXT("TargetLocation");
			//text = FText::FromString(TEXT("TargetLocation"));
			break;
		case ESkillShapedIconType::VE_TargetCircle:
			strId += TEXT("TargetCircle");
			//text = FText::FromString(TEXT("TargetCircle"));
			break;
		case ESkillShapedIconType::VE_Me:
			strId += TEXT("Me");
			//text = FText::FromString(TEXT("Me"));
			break;
		case ESkillShapedIconType::VE_Friend:
			strId += TEXT("Friend");
			//text = FText::FromString(TEXT("Friend"));
			break;
		}

		//FString strId = TEXT("Icon_SkillShape_") + text.ToString();

		UUIFunctionLibrary::SetImageBrushFromTable(FName(*strId), Image_AttackRange);
	}

	if (IsValid(Text_AttackRange))
	{	
		if(bSpiritSkill)
			Text_AttackRange->SetText(FText::FromString(UDescriptionFunctionLibrary::GetFloatToStringWithPrecision(SpiritSkillTableInfo.UISkillRange * 0.01f, 2)));
		else
			Text_AttackRange->SetText(FText::FromString(UDescriptionFunctionLibrary::GetFloatToStringWithPrecision(SkillTableInfo_V2.UISkillRange * 0.01f, 2)));
	}

	if (IsValid(Text_CastingRange))
	{
		//todo : default search value
		int32 TargetCastingRange = bSpiritSkill ? SpiritSkillTableInfo.CastingRange : SkillTableInfo_V2.CastingRange;
		if (TargetCastingRange == 100)
			TargetCastingRange = 0;

		Text_CastingRange->SetText(FText::FromString(UDescriptionFunctionLibrary::GetFloatToStringWithPrecision(TargetCastingRange * 0.01f, 2)));
	}

	if (IsValid(Text_CastingTime))
	{
		if(bSpiritSkill)
			Text_CastingTime->SetText(FText::AsNumber(SpiritSkillTableInfo.CastingTime));
		else
			Text_CastingTime->SetText(FText::AsNumber(SkillTableInfo_V2.CastingTime));
	}
	
	if (HeroSkillInfo.level == MAX_Skill_LEVEL)
		SkillLevelUpBtn->SetIsEnabled(false);
	else
		SkillLevelUpBtn->SetIsEnabled(true);

	
	
	if (HeroSkillInfo.level == 0)
	{
		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CurHeroID);
		if (CharacterTableInfo)
		{
			int32 TargetLv = 0;
			if (HeroSkillInfo.slotNo == (int32)EHERO_SKILL_SLOT::ActiveSkill03)
				TargetLv = 5;
			else if (HeroSkillInfo.slotNo == (int32)EHERO_SKILL_SLOT::PassiveSkill01)
				TargetLv = 7;
			
			FString str = TEXT("(") + UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, FName("Skill_UnLock_Default1")).ToString() + FString::FromInt(TargetLv) + UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, FName("Skill_UnLock_Default2")).ToString() + TEXT(")");
			FText text = FText::FromString(str);
			if(IsValid(Text_Warning))
				Text_Warning->SetText(text);
		}

		FSlateColor color = FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));

		for (size_t i = 0; i < ColorTextArray.Num(); ++i)
			ColorTextArray[i]->SetColorAndOpacity(color);

		if (IsValid(WidgetSwitcher_Active))
			WidgetSwitcher_Active->SetActiveWidgetIndex(1);


	}
	else
	{
		for (size_t i = 0; i < ColorTextArray.Num(); ++i)
			ColorTextArray[i]->SetColorAndOpacity(ColorArray[i]);

		if (IsValid(WidgetSwitcher_Active))
			WidgetSwitcher_Active->SetActiveWidgetIndex(0);

	}
			
	

	RefreshLevelUpEnable();
	RefreshRequireItem();
}

void UUC_SkillDetail::RefreshLevelUpEnable()
{
	if (bHeroActive && IsValid(SkillLevelUpBtn))
	{
		if (HeroSkillInfo.level != 0)
		{
			SkillLevelUpBtn->SetIsEnabled(true);
			SkillCostInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			SkillLevelUpBtn->SetIsEnabled(false);
			SkillCostInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}



void UUC_SkillDetail::RefreshRequireItem()
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	ESlateVisibility CurrencyItemVisible = ESlateVisibility::Hidden;
	if (false == NeedRequireItem.IsNone() && 0 < NeedRequireItemCount)
	{
		CurrencyItemVisible = ESlateVisibility::SelfHitTestInvisible;

		if (IsValid(Text_RequireItemCount))
		{
			Text_RequireItemCount->SetText(FText::AsNumber(NeedRequireItemCount));
		}
		if (IsValid(Text_CurrentItemCount))
		{
			Text_CurrentItemCount->SetText(FText::AsNumber(CurrentRequireItemCount));
		}
		auto ItemTableInfo = UTableManager::GetInstancePtr()->GetWalletRow(NeedRequireItem);
		if(ItemTableInfo)
		{
			/*if (IsValid(Text_RequireItemName))
			{
				Text_RequireItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemTableInfo->CommonStringKey));
			}*/

			if (IsValid(Image_NeedItem))
			{
				Image_NeedItem->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromTable(ItemTableInfo->ItemIconKey));
			}
		}

	}

	if (IsValid(Text_SkillLevelupCost))
	{
		Text_SkillLevelupCost->SetText(FText::AsNumber(NeedSkillUpgradeGold));
	}

	if (IsValid(Hbox_RequireItem))
	{
		Hbox_RequireItem->SetVisibility(CurrencyItemVisible);
	}
}


/************************************************************************/
/* Button OnClick Event								                    */
/************************************************************************/
void UUC_SkillDetail::OnClick_SkillLevelTab(int32 InIndex)
{
	CurrentSkillLevel = InIndex;
	if (MAX_Skill_LEVEL < CurrentSkillLevel)
		CurrentSkillLevel = MAX_Skill_LEVEL;

	RefreshUI();
}

void UUC_SkillDetail::OnClick_Close()
{
	URGameInstance* RGameInst = RGAMEINSTANCE(this);

	URWidgetManager* WidgetManager = RGameInst->RWidgetManager;
	if (WidgetManager)
	{
		UTopBar* TopBar = WidgetManager->GetUserWidgetT<UTopBar>(EWidgetBluePrintEnum::WBP_TopBar);
		if (TopBar)
			TopBar->PrevButton();
	}
}

void UUC_SkillDetail::OnClick_SkillLevelUp()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->EventManager) == false)
		return;

	FCharacterTableInfo* characterinfo = UTableManager::GetInstancePtr()->GetCharacterRow(CurHeroID);
	if (CurrentSkillLevel >= characterinfo->SkillLevelUpUnlock)
	{
		FText text = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, characterinfo->SkillLevelUpUnlockString);
		UUIFunctionLibrary::ShowCommonPopupSplash(FText::FromString(TEXT("")), text);
		return;
	}
		

	int32 CurrentGold = RGameInstance->RInventory->GetGold();
	if (CurrentRequireItemCount < NeedRequireItemCount)
	{
		UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_HeroSkill_WarnTitan")), ECommonPopupType::VE_OK);

		return;
	}
	if (CurrentGold < NeedSkillUpgradeGold)
	{
		UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_HeroSkill_WarnGold")), ECommonPopupType::VE_OKCANCEL);

		CommonPopup->OnClickOK.Clear();
		CommonPopup->OnClickOK.AddDynamic(this, &UUC_SkillDetail::Callback_NotEnoughGold);

		if (IsValid(CommonPopup->LocalizingTextBlock_OK))
		{
			CommonPopup->LocalizingTextBlock_OK->CustomControl = true;
			CommonPopup->LocalizingTextBlock_OK->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Areana_Result_confirm")));
		}

		if (IsValid(CommonPopup->LocalizingTextBlock_Cancel))
		{
			CommonPopup->LocalizingTextBlock_Cancel->CustomControl = true;
			CommonPopup->LocalizingTextBlock_Cancel->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_ItemManage_cancel")));
		}

		return;
	}
		
	if (UC_SkillLvUpSuccessPopup.IsValid())
	{
		UImage* texture = UC_SkillLvUpSuccessPopup.Get()->Image_SkillIcon;
		texture->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceTableInfo.SkillIconRef));

		UTextBlock* text = UC_SkillLvUpSuccessPopup.Get()->Text_SkillLevel;
		text->SetText(FText::FromString(FString::FromInt(CurrentSkillLevel + 1)));

		UTextBlock* textskill = UC_SkillLvUpSuccessPopup.Get()->Text_SkillName;
		textskill->SetText(Text_SkillName->Text);
	}
	

	//RGameInstance->EventManager->OnClickSkillUpgrade_UC_SkillDetail.Broadcast(HeroSkillInfo.slotNo);
	UPacketFunctionLibrary::INVENTORY_HERO_SKILL_LEVELUP_RQ(CurHeroUD, HeroSkillInfo.slotNo);
}

void UUC_SkillDetail::Callback_NotEnoughGold()
{
	URWidgetManager* widgetmanager = RGAMEINSTANCE(this)->RWidgetManager;
	if (widgetmanager)
	{
		widgetmanager->ChangeUIPage(EUIPageEnum::UIPage_Shop);

		/*UTopBar* topbar = widgetmanager->GetUserWidgetT<UTopBar>(EWidgetBluePrintEnum::WBP_TopBar);
		topbar->SetbShopNotEnoughMoney(true);*/

		/*UUP_Shop* shop = widgetmanager->GetUserWidgetT<UUP_Shop>(EWidgetBluePrintEnum::WBP_Shop);
		shop->SetShopDisplayPage(1, true);*/
	}
	

	
	
}
