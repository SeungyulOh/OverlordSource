// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"


#include "Table/AvatarAbilityTableInfo.h"

#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Common/TopBar.h"
//#include "UI/Avatar/Item/UC_AvatarSkillInfo.h"
#include "UI/Avatar/Item/UC_AvatarSkill.h"
#include "UI/ItemManagement/UP_ItemShortagePopup.h"
#include "UI/Common/UC_CurrencyDisplay.h"

#include "UP_AvatarAbility.h"
#include "UI/UI_Common/UP_CommonPopup.h"

#define AVATAR_SKILL_RESET_GEMS 10

void UUP_AvatarAbility::NativeConstruct()
{
	Super::NativeConstruct();

	RGameInstance = RGAMEINSTANCE(this);

	IconArray.Reset(18);
	IconArray.Emplace(Avatar_Skill1);
	IconArray.Emplace(Avatar_Skill2);
	IconArray.Emplace(Avatar_Skill3);
	IconArray.Emplace(Avatar_Skill4);
	IconArray.Emplace(Avatar_Skill5);
	IconArray.Emplace(Avatar_Skill6);
	IconArray.Emplace(Avatar_Skill7);
	IconArray.Emplace(Avatar_Skill8);
	IconArray.Emplace(Avatar_Skill9);
	IconArray.Emplace(Avatar_Skill10);
	IconArray.Emplace(Avatar_Skill11);
	IconArray.Emplace(Avatar_Skill12);
	IconArray.Emplace(Avatar_Skill13);
	IconArray.Emplace(Avatar_Skill14);
	IconArray.Emplace(Avatar_Skill15);
	IconArray.Emplace(Avatar_Skill16);
	IconArray.Emplace(Avatar_Skill17);
	IconArray.Emplace(Avatar_Skill18);
	
	if (RGameInstance)
	{
		RGameInstance->EventManager->OnUpdateUserInfo.RemoveDynamic(this, &UUP_AvatarAbility::Refresh);
		RGameInstance->EventManager->OnUpdateUserInfo.AddDynamic(this, &UUP_AvatarAbility::Refresh);
		RGameInstance->EventManager->OnRequestReceive.RemoveDynamic(this, &UUP_AvatarAbility::OnRequestReceive);
		RGameInstance->EventManager->OnRequestReceive.AddDynamic(this, &UUP_AvatarAbility::OnRequestReceive);
	}

	if (Button_Reset)
	{
		Button_Reset->OnClicked.RemoveDynamic(this, &UUP_AvatarAbility::ShowResetPopup);
		Button_Reset->OnClicked.AddDynamic(this, &UUP_AvatarAbility::ShowResetPopup);
	}

	int32 i = 0;
	for (auto Element : IconArray)
	{
		if (Element)
		{
			Element->SetIndex(i);
			Element->SetOwner(this);
			Element->Refresh();
		}

		++i;
	}

	if (IsValid(ResetCurrency))
	{
		ResetCurrency->SetCurrency(WALLET_GEM, 10);
	}
	Refresh();
}

void UUP_AvatarAbility::NativeDestruct()
{
	if (RGameInstance)
	{
		RGameInstance->EventManager->OnUpdateUserInfo.RemoveDynamic(this, &UUP_AvatarAbility::Refresh);
	}
	IsInitActive = false;

	Super::NativeDestruct();
}

void UUP_AvatarAbility::SetActiveSubPage()
{
	if (IsInitActive)
	{
		Reset();
		return;
	}

	IsInitActive = true;

	//auto Avatar = RGameInstance->UserInfo->GetAvatarData();
	
	//OnScrollViewSelectedItem
}

void UUP_AvatarAbility::SetUITopBarString()
{
	/*auto TopbarWidget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	if (TopbarWidget)
	{
		auto Target = Cast<UTopBar>(TopbarWidget);
		if (Target)
		{
			Target->SetCurrentUIString(EUIPageEnum::UIPage_AvatarAbility);
		}
	}*/
}

void UUP_AvatarAbility::Refresh()
{
	if (RGameInstance->GetIsResetSkill())
	{
		for (auto Element : IconArray)
		{
			if (Element)
			{
				Element->Refresh();
			}
		}

		RGameInstance->SetIsResetSkill(false);

		UUIFunctionLibrary::ShowCommonPopupSplash(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Account_Ability_0003")));

	}

	if (IconArray.IsValidIndex(CurrentIndex))
	{
		IconArray[CurrentIndex]->Refresh();
	}
	
	SetAbilityPointUI();

	RefreshPopupInfo(CurrentIndex);
}

void UUP_AvatarAbility::SetAbilityPointUI()
{
	auto Avatar = RGameInstance->UserInfo->GetAvatarData();
	int32 APCurrentValue = Avatar.level;
	for (auto Element : Avatar.ability)
	{
		APCurrentValue -= Element;
	}
	auto APCurrent = FText::AsNumber(APCurrentValue);

	bool bAbleReset = APCurrentValue != Avatar.level;

	if (IsValid(AbilityResetBox))
	{
		AbilityResetBox->SetVisibility(bAbleReset ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (IsValid(Text_APCurrent))
		Text_APCurrent->SetText(APCurrent);
}

void UUP_AvatarAbility::Reset()
{
// 	if (SkillInfo)
// 	{
// 		SkillInfo->SetEmpty();
// 	}

// 	if (CurrentIndex >= 0)
// 	{
// 		if (IconArray.IsValidIndex(CurrentIndex))
// 		{
// 			IconArray[CurrentIndex]->ToggleSelected();
// 		}
// 		CurrentIndex = -1;
// 	}
}

void UUP_AvatarAbility::SelectIndex(int32 InIndex)
{
	CurrentIndex = InIndex;
	if(IconArray.IsValidIndex(CurrentIndex))
		RGameInstance->EventManager->OnClickSkillIcon.Broadcast(IconArray[InIndex]);
}

void UUP_AvatarAbility::RefreshPopupInfo(int32 InIndex)
{
	CurrentIndex = InIndex;
	if (IconArray.IsValidIndex(CurrentIndex))
		RGameInstance->EventManager->OnRefreshSkillInfo.Broadcast(IconArray[InIndex]);
}

void UUP_AvatarAbility::ShowResetPopup()
{
	UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Shop_Reset")),
		ECommonPopupType::VE_OKCANCEL);

	if (IsValid(CommonPopup))
	{
		CommonPopup->OnClickOK.Clear();
		CommonPopup->OnClickOK.AddDynamic(this, &UUP_AvatarAbility::ClickResetButton);
	}
}

void UUP_AvatarAbility::ClickResetButton()
{
// 	if (!IsValid(RGameInstance))
// 		return;

	auto Avatar = RGameInstance->RInventory->GetGem();
	if (AVATAR_SKILL_RESET_GEMS > RGameInstance->RInventory->GetGem())
	{
		RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);

		auto Widget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		auto ShortagePopup = Cast<UUP_ItemShortagePopup>(Widget);
		ShortagePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		float GemsValue = AVATAR_SKILL_RESET_GEMS - RGameInstance->RInventory->GetGem();
		ShortagePopup->SetNecessaryGem(GemsValue);
		ShortagePopup->LocalText_Caption->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY));
		ShortagePopup->LocalText_Desc->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_ItemManage_OptionWarning"));
		return;
	}

	UPacketFunctionLibrary::AVATAR_ABILITY_RESET_RQ();
}

void UUP_AvatarAbility::OnRequestReceive()
{
	ShowSkillUpSplashPopup(CurrentIndex);
}

void UUP_AvatarAbility::ShowSkillUpSplashPopup(int32 InIndex)
{
	FName DescKey;
	FFormatOrderedArguments Args;

	DescKey = TEXT("UI_Account_Success1");
	//TArray<FString> HeroKeys;
	//RGameInstance->HeroInventory->GetInventoryKeysByType(HeroKeys);
	auto Avatar = RGameInstance->UserInfo->GetAvatarData();
	if (Avatar.ability.IsValidIndex(InIndex) == false)
		return;

	if (!IconArray.IsValidIndex(InIndex))
		return;

	auto Key = IconArray[InIndex]->GetTableKey();
	FAvatarAbilityTableInfo* Table = UTableManager::GetInstancePtr()->GetAvatarAbilityTableRow(Key);
	if (Table == nullptr)
		return;

	int32 Level = Avatar.ability[InIndex];
	int32 NextValue = 0;
	

	switch (Level)
	{
	case 1: NextValue = (Table->Ability_Value1); break;
	case 2: NextValue = (Table->Ability_Value2); break;
	case 3: NextValue = (Table->Ability_Value3); break;
	case 4: NextValue = (Table->Ability_Value4); break;
	case 5: NextValue = (Table->Ability_Value5); break;
	default: NextValue = 0; break;
	}

	FString AccountText;
	if (0 <= InIndex && InIndex <= 2)
		AccountText = "UI_Account_Abilitytab1";
	if (3 <= InIndex && InIndex <= 5)
		AccountText = "UI_Account_Abilitytab2";
	if (6 <= InIndex && InIndex <= 8)
		AccountText = "UI_Account_Abilitytab3";
	if (9 <= InIndex && InIndex <= 11)
		AccountText = "UI_Account_Abilitytab4";
	if (12 <= InIndex && InIndex <= 14)
		AccountText = "UI_Account_Abilitytab5";
	if (15 <= InIndex && InIndex <= 17)
		AccountText = "UI_Account_All";

	FText PropertyText = GetPropertyName(Table->Ability_type);

	Args.Emplace(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(*AccountText)));
	Args.Emplace(PropertyText);
	Args.Emplace(NextValue);

	FText DescText;
	DescText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, DescKey);
	DescText = FText::Format(DescText, Args);

	UUIFunctionLibrary::ShowCommonPopupSplash(PropertyText, DescText);
}

FText UUP_AvatarAbility::GetPropertyName(EAvatarAbilityType InType)
{
	FString StringKey;
	
	switch (InType)
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
