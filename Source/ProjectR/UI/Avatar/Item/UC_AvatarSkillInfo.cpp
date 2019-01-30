// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"

#include "UtilFunctionIntegrated.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/RWidgetManager.h"
#include "UI/ItemManagement/UP_ItemShortagePopup.h"

#include "UI/UI_Common/UP_CommonPopup.h"

#include "UC_AvatarSkillInfo.h"

#define AVATAR_LEVELMAX 5
#define AVATAR_SKILL_RESET_GEMS 10

void UUC_AvatarSkillInfo::NativeConstruct()
{
	Super::NativeConstruct();

	RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		RGameInstance->EventManager->OnUpdateUserInfo.RemoveDynamic(this, &UUC_AvatarSkillInfo::Refresh);
		RGameInstance->EventManager->OnUpdateUserInfo.AddDynamic(this, &UUC_AvatarSkillInfo::Refresh);
	}

	if (Button_Up)
	{
		Button_Up->OnClicked.RemoveDynamic(this, &UUC_AvatarSkillInfo::ClickUpButton);
		Button_Up->OnClicked.AddDynamic(this, &UUC_AvatarSkillInfo::ClickUpButton);
	}

	if (Button_Reset)
	{
		Button_Reset->OnClicked.RemoveDynamic(this, &UUC_AvatarSkillInfo::ShowResetPopup);
		Button_Reset->OnClicked.AddDynamic(this, &UUC_AvatarSkillInfo::ShowResetPopup);
	}

	PropertyArray.Reserve(6);
	PropertyArray.Emplace(Text_Property1);
	PropertyArray.Emplace(Text_Property2);
	PropertyArray.Emplace(Text_Property3);
	PropertyArray.Emplace(Text_Property4);
	PropertyArray.Emplace(Text_Property5);
	PropertyArray.Emplace(Text_Property6);

	Refresh();
	SetEmpty();
}

void UUC_AvatarSkillInfo::NativeDestruct()
{
	if (RGameInstance)
	{
		RGameInstance->EventManager->OnUpdateUserInfo.RemoveDynamic(this, &UUC_AvatarSkillInfo::Refresh);
	}
	PropertyArray.Empty();

	Super::NativeDestruct();
}


void UUC_AvatarSkillInfo::ClickUpButton()
{
// 	if (Index < 0 || UsePoint <= 0 || NextLevel > AVATAR_LEVELMAX)
// 		return;
	
	/*if (APCurrentValue < UsePoint)
	{
		FText Caption = FText::FromString(TEXT(""));
		FText Contents = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "ee");
		UUIFunctionLibrary::ShowCommonPopup(Caption, Contents, ECommonPopupType::VE_Error);
		//Popup
		return;
	}*/

// 	if (UUIFunctionLibrary::IsEnableUsingCurrency(RGameInstance, EREWARD_TYPE::GOLD, UseGold))
// 	{
// 		IsSplashPopup = true;
// 		UPacketFunctionLibrary::AVATAR_ABILITY_UP_RQ(RGameInstance, Index, NextLevel);
// 	}
//
// 	auto Avatar = RGameInstance->UserInfo->GetAvatarData();
// 	if (UseGold > Avatar.gold)
// 	{
// 		RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
// 		
// 		auto Widget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
// 		auto ShortagePopup = Cast<UUP_ItemShortagePopup>(Widget);
// 		ShortagePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		float GoldValue = UseGold - Avatar.gold;
// 		ShortagePopup->SetNecessaryGold(GoldValue);
// 		//ShortagePopup->SetChangeTextBlock(FText("USK_up_ItemManage_OptionWarning"), "");
// 		//ShortagePopup->SetChangeTextBlock("USK_up_ItemManage_OptionWarning", "UI_HeroManage_Notify2");
// 
// 		/*auto Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Error, "NE1303");
// 		auto CaptionString = Caption.ToString();
// 		int32 Index = CaptionString.Find("(");
// 		FString ValueX = CaptionString.Left(Index);
// 		Caption = FText::FromString(ValueX);*/
// 
// 		auto Caption = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY);
// 		auto Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_HeroManage_Notify2");
// 		FFormatOrderedArguments Args;
// 		auto DescTemp = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_Result_gold"); 
// 		Args.Emplace(DescTemp);
// 		Args.Emplace(GoldValue);
// 		Desc = FText::Format(Desc, Args);
// 
// 		ShortagePopup->LocalText_Caption->SetText(Caption);
// 		ShortagePopup->LocalText_Desc->SetText(Desc);
// 		return;
// 	}	
}

void UUC_AvatarSkillInfo::ClickResetButton()
{
// 	IsSplashPopup = false;
// 	auto Avatar = RGameInstance->UserInfo->GetAvatarData();
// 	if (UUIFunctionLibrary::IsEnableUsingCurrency(RGameInstance, EREWARD_TYPE::GEM, AVATAR_SKILL_RESET_GEMS))
// 	{
// 		//ResetGems
// 		//Popup
// 		UPacketFunctionLibrary::AVATAR_ABILITY_RESET_RQ(RGameInstance);
// 	}
// 	if (AVATAR_SKILL_RESET_GEMS > Avatar.gems)
// 	{
// 		RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
// 
// 		auto Widget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
// 		auto ShortagePopup = Cast<UUP_ItemShortagePopup>(Widget);
// 		ShortagePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		float GemsValue = AVATAR_SKILL_RESET_GEMS - Avatar.gems;
// 		ShortagePopup->SetNecessaryGem(GemsValue);
// 		ShortagePopup->LocalText_Caption->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY));
// 		ShortagePopup->LocalText_Desc->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_ItemManage_OptionWarning"));
// 		return;
// 	}

}

void UUC_AvatarSkillInfo::ShowResetPopup()
{
	FString TestHeadString(TEXT("AP Point"));
	//FString TestString(TEXT("UI_Shop_Reset"));

	UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		//FText::FromString(TestHeadString),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_Reset")),
		ECommonPopupType::VE_OKCANCEL);

	if (IsValid(CommonPopup))
	{
		CommonPopup->OnClickOK.Clear();
		CommonPopup->OnClickOK.AddDynamic(this, &UUC_AvatarSkillInfo::ClickResetButton);
		//CommonPopup->OnClickCancel.Clear();
		//CommonPopup->OnClickCancel.AddDynamic(this, &UUC_AvatarSkillInfo::SkipCancel);
	}
}

void UUC_AvatarSkillInfo::SetEmpty()
{
	if (Button_Up)
	{
		Button_Up->SetIsEnabled(false);
		//Button_Up->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Switcher_Info)
	{
		Switcher_Info->SetActiveWidgetIndex(0);

		//if (Text_Empty)
		{
			auto Avatar = RGameInstance->UserInfo->GetAvatarData();
			
			//FFormatOrderedArguments Args;

			int32 ArrayIndex = 0;
			for (int32 i = 0; i < 18; i += 3)
			{
				int32 Point = Avatar.ability[i] + Avatar.ability[i + 1] + Avatar.ability[i + 2];
				
				if (PropertyArray.IsValidIndex(ArrayIndex) && PropertyArray[ArrayIndex] != nullptr)
				{
					auto DescText = FText::AsNumber(Point);
					PropertyArray[ArrayIndex]->SetText(DescText);
				}
				++ArrayIndex;
				//Args.Emplace(Point);
			}

			//FText DescText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Account_Ability_Default");
			//DescText = FText::Format(DescText, Args);

			//Text_Empty->SetText(DescText);
		}
	}
}

FText UUC_AvatarSkillInfo::GetPropertyName(int32 InIndex)
{
	FString StringKey;
	/*switch (InIndex)
	{
		case 0: StringKey = "USK_uc_heroProperty_PhyDefence"; break;
		case 1: StringKey = "USK_uc_heroProperty_Health"; break;
		case 2: StringKey = "USK_uc_heroProperty_MagicDefence"; break;

		case 3: StringKey = "USK_uc_heroProperty_CriticalRate"; break;
		case 4: StringKey = "USK_uc_heroProperty_CriticalDamage"; break;
		case 5: StringKey = "USK_uc_heroProperty_Dodge"; break;

		case 6: StringKey = "USK_uc_heroProperty_CriticalRate"; break;
		case 7: StringKey = "USK_uc_heroProperty_CriticalDamage"; break;
		case 8: StringKey = "USK_uc_heroProperty_Parry"; break;

		case 9: StringKey = "USK_uc_heroProperty_Pierce"; break;
		case 10: StringKey = "USK_uc_heroProperty_Magicattack"; break;
		case 11: StringKey = "USK_uc_heroProperty_MagicCritical"; break;

		case 12: StringKey = "USK_uc_heroProperty_CriticalResist"; break;
		case 13: StringKey = "USK_uc_heroProperty_Health"; break;
		case 14: StringKey = "USK_uc_heroProperty_ReduceCoolTime"; break;

		case 15: StringKey = "USK_uc_heroProperty_SkillCostBonus"; break;
		case 16: StringKey = "USK_uc_heroProperty_MoveSpeed"; break;
		case 17: StringKey = "USK_uc_heroProperty_ReceivedHeal"; break;
		default: return FText(); break;
	}*/

	switch (AbilityType)
	{
		case EAvatarAbilityType::VE_PhysicalDefence:	StringKey = "USK_uc_heroProperty_PhyDefence"; break;
		case EAvatarAbilityType::VE_MaxHP:				StringKey = "USK_uc_heroProperty_Health"; break;
		case EAvatarAbilityType::VE_MagicalDefence:		StringKey = "USK_uc_heroProperty_MagicDefence"; break;
		case EAvatarAbilityType::VE_CriticalRate:		StringKey = "USK_uc_heroProperty_CriticalRate"; break;
		case EAvatarAbilityType::VE_CriticalDamage:		StringKey = "USK_uc_heroProperty_CriticalDamage"; break;
		case EAvatarAbilityType::VE_Dodge:				StringKey = "USK_uc_heroProperty_Dodge"; break;
		case EAvatarAbilityType::VE_Parry:				StringKey = "USK_uc_heroProperty_Parry"; break;
		case EAvatarAbilityType::VE_Pierce:				StringKey = "USK_uc_heroProperty_Pierce"; break;
		case EAvatarAbilityType::VE_MagicalAttack:		StringKey = "USK_uc_heroProperty_Magicattack"; break;
		case EAvatarAbilityType::VE_MagicCritical:		StringKey = "USK_uc_heroProperty_MagicCritical"; break;
		case EAvatarAbilityType::VE_CriticalResist:		StringKey = "USK_uc_heroProperty_CriticalResist"; break;
		case EAvatarAbilityType::VE_ReduceCoolTime:		StringKey = "USK_uc_heroProperty_ReduceCoolTime"; break;
		case EAvatarAbilityType::VE_SkillCostBonus:		StringKey = "USK_uc_heroProperty_SkillCostBonus"; break;
		case EAvatarAbilityType::VE_MoveSpeed:			StringKey = "USK_uc_heroProperty_MoveSpeed"; break;
		case EAvatarAbilityType::VE_ReceivedHeal:		StringKey = "USK_uc_heroProperty_ReceivedHeal"; break;
		case EAvatarAbilityType::VE_None:
		case EAvatarAbilityType::VE_TypeMax: 
			return FText();
		break;
	}

	return UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(*StringKey));
}

void UUC_AvatarSkillInfo::Refresh()
{
	auto Avatar = RGameInstance->UserInfo->GetAvatarData();

	if (Text_APCurrent)
	{
		APCurrentValue = Avatar.level;
		for (auto Element : Avatar.ability)
		{
			APCurrentValue -= Element;
		}
		auto APCurrent = FText::AsNumber(APCurrentValue);
		Text_APCurrent->SetText(APCurrent);
	}

	if (Text_APMax)
	{
		MaxPoint = Avatar.level;
		auto APMax = FText::AsNumber(Avatar.level);
		Text_APMax->SetText(APMax);
	}

	if (Button_Reset)
	{
		if (APCurrentValue == MaxPoint)
		{
			Button_Reset->SetIsEnabled(false);
			//Button_Reset->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			Button_Reset->SetIsEnabled(true);
		}
	}

	if (IsSplashPopup)
	{
		ShowSkillUpSplashPopup();
	}
}

void UUC_AvatarSkillInfo::ShowSkillUpSplashPopup()
{
	FName DescKey;
	FFormatOrderedArguments Args;

	DescKey = TEXT("UI_Account_Success1");
	//TArray<FString> HeroKeys;
	//RGameInstance->HeroInventory->GetInventoryKeysByType(HeroKeys);

	FString AccountText;
	if (0 <= Index && Index <= 2)
		AccountText = "UI_Account_Abilitytab1";
	if (3 <= Index && Index <= 5)
		AccountText = "UI_Account_Abilitytab2";
	if (6 <= Index && Index <= 8)
		AccountText = "UI_Account_Abilitytab3";
	if (9 <= Index && Index <= 11)
		AccountText = "UI_Account_Abilitytab4";
	if (12 <= Index && Index <= 14)
		AccountText = "UI_Account_Abilitytab5";
	if (15 <= Index && Index <= 17)
		AccountText = "UI_Account_All";

	FText PropertyText = GetPropertyName(Index);

	Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(*AccountText)));
	Args.Emplace(PropertyText);
	Args.Emplace(NextValue);

	FText DescText;
	DescText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, DescKey);
	DescText = FText::Format(DescText, Args);

	UUIFunctionLibrary::ShowCommonPopupSplash(PropertyText, DescText);
	IsSplashPopup = false;
}

void UUC_AvatarSkillInfo::SetInfo(UUC_AvatarSkill* InSelectSkill)
{
	Index = InSelectSkill->GetIndex();

	auto Avatar = RGameInstance->UserInfo->GetAvatarData();

	if (Avatar.ability.IsValidIndex(Index) == false)
		return;

	int32 Level = Avatar.ability[Index];
	NextLevel = Level + 1;

	auto Key = InSelectSkill->GetTableKey();
	auto Table = UTableManager::GetInstancePtr()->GetAvatarAbilityTableRow(Key);
	if (Table == nullptr)
		return;

	AbilityType = Table->Ability_type;

	if (NextLevel > AVATAR_LEVELMAX)
	{
		if (Button_Up)
		{
			Button_Up->SetIsEnabled(false);
			//Button_Up->SetVisibility(ESlateVisibility::Hidden);
		}
		if (Overlay_Use)
		{
			Overlay_Use->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		if (Button_Up)
		{
			Button_Up->SetIsEnabled(true);
			//Button_Up->SetVisibility(ESlateVisibility::Visible);
		}
		if (Overlay_Use)
		{
			Overlay_Use->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	if (Switcher_Info)
	{
		Switcher_Info->SetActiveWidgetIndex(1);
	}

	if (Text_APCurrent)
	{
		APCurrentValue = Avatar.level;
		for (auto Element : Avatar.ability)
		{
			APCurrentValue -= Element;
		}
		auto APCurrent = FText::AsNumber(APCurrentValue);
		Text_APCurrent->SetText(APCurrent);
	}

	if (Text_APMax)
	{
		MaxPoint = Avatar.level;
		auto APMax = FText::AsNumber(Avatar.level);
		Text_APMax->SetText(APMax);
	}

	if (Button_Reset)
	{
		if (APCurrentValue == MaxPoint)
		{
			Button_Reset->SetIsEnabled(false);
			//Button_Reset->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			Button_Reset->SetIsEnabled(true);
		}
//		ResetGems = AVATAR_SKILL_RESET_GEMS;
	}

	if (Text_APUse)
	{
		switch (NextLevel)
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				UsePoint = 1;
			break;
			default:
				UsePoint = 0;
			break;
		}

		FText APUseText = FText::AsNumber(UsePoint);
		Text_APUse->SetText(APUseText);
	}

	if (Text_LevelCurrent)
	{
		auto LevelCurrent = FText::AsNumber(Level);
		Text_LevelCurrent->SetText(LevelCurrent);
	}

	if (Text_LevelNext)
	{
		FText LevelNext;
		if (NextLevel > AVATAR_LEVELMAX)
		{
			LevelNext = FText::FromString("Max");
		}
		else
		{
			LevelNext = FText::AsNumber(NextLevel);
		}
		Text_LevelNext->SetText(LevelNext);
	}

	if (Text_DescCurrent)
	{
		FFormatOrderedArguments Args;
		FText DescText;
		switch (Level)
		{
			case 1: PrevValue = (Table->Ability_Value1); break;
			case 2: PrevValue = (Table->Ability_Value2); break;
			case 3: PrevValue = (Table->Ability_Value3); break;
			case 4: PrevValue = (Table->Ability_Value4); break;
			case 5: PrevValue = (Table->Ability_Value5); break;
			default: PrevValue = 0; break;
		}
		
		Args.Emplace(PrevValue);

		if (Args.Num() > 0)
		{
			DescText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, InSelectSkill->UIStringKey);
			DescText = FText::Format(DescText, Args);
		}
		
		Text_DescCurrent->SetRichText(DescText);
	}

	if (Text_DescNext)
	{
		FFormatOrderedArguments Args;
		FText NextText;
		switch (NextLevel)
		{
			case 1: NextValue = (Table->Ability_Value1); break;
			case 2: NextValue = (Table->Ability_Value2); break;
			case 3: NextValue = (Table->Ability_Value3); break;
			case 4: NextValue = (Table->Ability_Value4); break;
			case 5: NextValue = (Table->Ability_Value5); break;
			default: NextValue = 0; break;
		}

		if (NextValue != 0)
		{
			Args.Emplace(NextValue);

			if (Args.Num() > 0)
			{
				NextText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, InSelectSkill->UIStringKey);
				NextText = FText::Format(NextText, Args);
			}

			Text_DescNext->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Text_DescNext->SetRichText(NextText);
		}
		else
		{
			Text_DescNext->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (Image_SPIcon)
	{

	}

	if (Text_SPValue)
	{
		switch (NextLevel)
		{
			case 1: UseGold = Table->Point1_gold; break;
			case 2: UseGold = Table->Point2_gold; break;
			case 3: UseGold = Table->Point3_gold; break;
			case 4: UseGold = Table->Point4_gold; break;
			case 5: UseGold = Table->Point5_gold; break;
			default: UseGold = 0; break;
		}

		FText SPCurrentText = FText::AsNumber(UseGold);
		Text_SPValue->SetText(SPCurrentText);
	}
}