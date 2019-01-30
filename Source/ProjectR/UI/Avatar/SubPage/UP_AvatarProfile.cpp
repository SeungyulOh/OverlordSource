// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"

#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Common/TopBar.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/Control/RScrollView.h"
#include "UI/Avatar/Item/UC_AvatarRecord.h"
#include "UI/Avatar/Item/UC_ScrollItem_UserHero.h"
#include "UI/Avatar/Item/ItemData_UserHero.h"
#include "UI/Avatar/Item/UC_ScrollItem_AvatarIcon.h"

#include "UP_AvatarProfile.h"

void UUP_AvatarProfile::NativeConstruct()
{
	Super::NativeConstruct();

	RGameInstance = RGAMEINSTANCE(this);

	if (RGameInstance)
	{
		RGameInstance->EventManager->OnUpdateUserInfo.RemoveDynamic(this, &UUP_AvatarProfile::Refresh);
		RGameInstance->EventManager->OnUpdateUserInfo.AddDynamic(this, &UUP_AvatarProfile::Refresh);
		RGameInstance->EventManager->OnUpdateUserRecord.RemoveDynamic(this, &UUP_AvatarProfile::UpdateRecord);
		RGameInstance->EventManager->OnUpdateUserRecord.AddDynamic(this, &UUP_AvatarProfile::UpdateRecord);
	}
	MenuArray.Empty(3);
	MenuArray.Emplace(CheckBox_Radio_Pve);
	MenuArray.Emplace(CheckBox_Radio_Coloseum);
	MenuArray.Emplace(CheckBox_Radio_Arena);
	RecordArray.Empty(3);
	RecordArray.Emplace(Avatar_Record1);
	RecordArray.Emplace(Avatar_Record2);
	RecordArray.Emplace(Avatar_Record3);

	for (auto Element : MenuArray)
	{
		if (Element == nullptr)
			continue;

		Element->OnCheckRadioCheck.RemoveDynamic(this, &UUP_AvatarProfile::SetUseMenuClick);
		Element->OnCheckRadioCheck.AddDynamic(this, &UUP_AvatarProfile::SetUseMenuClick);
	}
	SelectMenuNumber = 0;

	UPacketFunctionLibrary::AVATAR_PROFILE_RQ();
}

void UUP_AvatarProfile::NativeDestruct()
{
	if (RGameInstance)
	{
		RGameInstance->EventManager->OnUpdateUserInfo.RemoveDynamic(this, &UUP_AvatarProfile::Refresh);
		RGameInstance->EventManager->OnUpdateUserRecord.RemoveDynamic(this, &UUP_AvatarProfile::UpdateRecord);
	}

	RGameInstance->UserInfo->CleanAvatarAccountInfo();
	IsInitActive = false;

	Super::NativeDestruct();
}

void UUP_AvatarProfile::SetActiveSubPage()
{
	if (IsInitActive)
	{
		Refresh();
		return;
	}

	IsInitActive = true;

	SetAvatarInfo();
	//SetRecord();
	//SetUseScroll();
}

void UUP_AvatarProfile::Refresh()
{
	auto Avatar = RGameInstance->UserInfo->GetAvatarData();

	if (AvatarIcon)
	{
		//todo avataricon table
		FName NameID;
		int32 TestValue = Avatar.icon - 1;
		if (TestValue == 0)
		{
			NameID = "PT_Avatar_Default";
		}
		else
		{
			auto ID = FString::FromInt(TestValue);
			auto Table = UTableManager::GetInstancePtr()->GetCharacterRow(FName(*ID));
			NameID = Table->PortraitTextureKey;
		}
// 		auto temp = AvatarIcon->Image_Hero;
// 		if (temp)
// 		{
// 			auto Image = UUIFunctionLibrary::GetIconImageFromTable(NameID);
// 			UUIFunctionLibrary::SetImageBrush(Image, temp);
// 		}
	}
}

void UUP_AvatarProfile::SetUITopBarString()
{
	/*auto TopbarWidget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	if (TopbarWidget)
	{
		auto Target = Cast<UTopBar>(TopbarWidget);
		if (Target)
		{
			Target->SetCurrentUIString(EUIPageEnum::UIPage_AvatarProfile);
		}
	}*/
}

void UUP_AvatarProfile::SetAvatarInfo()
{
	auto Avatar = RGameInstance->UserInfo->GetAvatarData();

	if (AvatarIcon)
	{
		//todo avataricon table
		FName NameID;
		int32 TestValue = Avatar.icon - 1;
		if (TestValue == 0)
		{
			NameID = "PT_Avatar_Default";
		}
		else
		{
			auto ID = FString::FromInt(TestValue);
			auto Table = UTableManager::GetInstancePtr()->GetCharacterRow(FName(*ID));
			if (Table != nullptr)
			{
				NameID = Table->PortraitTextureKey;
			}
			else
			{
				NameID = TEXT("PT_Avatar_Default");
			}
		}
// 		auto temp = AvatarIcon->Image_Hero;
// 		if (temp)
// 		{
// 			auto Image = UUIFunctionLibrary::GetIconImageFromTable(NameID);
// 			UUIFunctionLibrary::SetImageBrush(Image, temp);
// 		}
	}

	if (Text_UserName)
	{
		auto AvatarName = FText::FromString(Avatar.nick);
		Text_UserName->SetText(AvatarName);
	}

	if (Text_GuildName)
	{
		auto AvatarGuild = FText::FromString(RGameInstance->GuildInfo->MyGuildInfo.name);
		Text_GuildName->SetText(AvatarGuild);
	}
	
	if (Text_AvatarLevel)
	{
		auto AvatarLevel = FText::AsNumber(Avatar.level);
		Text_AvatarLevel->SetText(AvatarLevel);
	}

	if (Text_ExpMax)
	{
		auto AvatarExpMax = FText::AsNumber(RGameInstance->UserInfo->AvatarExpMax);
		Text_ExpMax->SetText(AvatarExpMax);
	}

	if (Text_ExpCurrent)
	{
		auto AvatarExp = FText::AsNumber(Avatar.exp);
		Text_ExpCurrent->SetText(AvatarExp);
	}

	if (Bar_Exp)
	{
		float AvatarEXP = static_cast<float>(Avatar.exp) / static_cast<float>(RGameInstance->UserInfo->AvatarExpMax);
		Bar_Exp->SetPercent(AvatarEXP);
	}
}

void UUP_AvatarProfile::UpdateRecord()
{
	SetRecord();
	SetUseScroll();
}

void UUP_AvatarProfile::SetRecord()
{
	auto Record = RGameInstance->UserInfo->GetAccountRecord();
	int32 i = 0;
	for (auto Element : RecordArray)
	{
		int32 nTotal = 0;// FMath::Rand() % 100;
		int32 nWin = 0;
		if (Record.IsValidIndex(i))
		{
			nTotal = Record[i].totalCount;
			nWin = Record[i].winCount;
		}
		if(IsValid(Element))
			Element->SetValue(nTotal, nWin);
		++i;
	}
}

void UUP_AvatarProfile::SetUseMenuClick(bool InChecked)
{
	if (MenuArray[SelectMenuNumber]->IsChecked())
		MenuArray[SelectMenuNumber]->SetIsChecked(false);

	int iNumber = -1;

	for (int i = 0; i < 3; i++)
	{
		if (MenuArray[i]->IsChecked())
			iNumber = i;
	}

	if (iNumber == -1)
		iNumber = SelectMenuNumber;
	else
		SelectMenuNumber = iNumber;

	MenuArray[SelectMenuNumber]->SetIsChecked(true);

	UseTargetNum = SelectMenuNumber;
	SetUseScroll();
}

void UUP_AvatarProfile::SetUseScroll()
{
// 	if (Scroll_UseHero == nullptr)
// 		return;
// 
// 	Scroll_UseHero->Init();
// 	Scroll_UseHero->ClearScrollItemData();
// 	
// 	int32 Max = 0;
// 	
// 	TArray<FName> RowNames = UTableManager::GetInstancePtr()->GetCharacterRowNames();
// 	TArray<FHERO_USAGE_TAB>& HeroUsage = RGameInstance->UserInfo->GetHeroUsage();
// 	if (HeroUsage.IsValidIndex(UseTargetNum))
// 	{
// 		for (auto Element : HeroUsage[UseTargetNum].usages)
// 		{
// 			UItemData_UserHero* UseHeroScrollData = Cast<UItemData_UserHero>(Scroll_UseHero->AddScrollItemData(UItemData_UserHero::StaticClass()));
// 			if (IsValid(UseHeroScrollData) == false)
// 				continue;
// 
// 			FName ID = FName(*Element.heroId);
// 
// 			auto Table = UTableManager::GetInstancePtr()->GetCharacterRow(ID);
// 			if (Table == nullptr)
// 				continue;
// 
// 			if (Max == 0)
// 			{
// 				Max = Element.count;
// 			}
// 			RowNames.Remove(ID);
// 
// 			UseHeroScrollData->Image_Hero = Table->PortraitTextureKey;
// 			UseHeroScrollData->Text_UserName = Table->DisplayName;
// 			UseHeroScrollData->UseValue = Element.count;
// 			UseHeroScrollData->UseMax = Max;
// 		}
// 	}
// 
// 	if (Max == 0)
// 	{
// 		Max = RowNames.Num();
// 	}
// 	for (auto Element : RowNames)
// 	{
// 		UItemData_UserHero* UseHeroScrollData = Cast<UItemData_UserHero>(Scroll_UseHero->AddScrollItemData(UItemData_UserHero::StaticClass()));
// 		if (IsValid(UseHeroScrollData) == false)
// 			continue;
// 		
// 		auto Table = UTableManager::GetInstancePtr()->GetCharacterRow(Element);
// 		if (Table == nullptr)
// 			continue;
// 
// 		UseHeroScrollData->Image_Hero = Table->PortraitTextureKey;
// 		UseHeroScrollData->Text_UserName = Table->DisplayName;
// 		UseHeroScrollData->UseValue = 0;//(FMath::Rand() % Max);
// 		UseHeroScrollData->UseMax = Max;
// 	}
// 
// 	Scroll_UseHero->InvalidateData();
}