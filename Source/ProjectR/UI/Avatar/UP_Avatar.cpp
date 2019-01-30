// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Avatar.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"

#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UI/Component/CheckBox_Radio.h"
#include "Network/PacketDataStructures.h"
#include "UI/Avatar/Item/UC_AvatarSkill.h"
#include "UI/Avatar/Item/UC_PopupAvatarSkillUp.h"
#include "GameInfo/UserInfo.h"

void UUP_Avatar::NativeConstruct()
{
	Super::NativeConstruct();

	RGameInstance = RGAMEINSTANCE(this);
	//auto Avatar = RGameInstance->UserInfo->GetAvatarData();

	MenuArray.Empty(3);
	MenuArray.Emplace(Radio_Profile);
	MenuArray.Emplace(Radio_Ability);
	MenuArray.Emplace(Radio_Icon);

	for (auto Element : MenuArray)
	{
		if (Element == nullptr)
			continue;

		Element->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Avatar::SelectMenu);
		Element->OnCheckRadioCheck.AddDynamic(this, &UUP_Avatar::SelectMenu);
		//RadioGroupoComponentStateChanged
		//Element->RadioGroupoComponentStateChanged.Unbind();
		//Element->RadioGroupoComponentStateChanged.BindDynamic(this, &UUP_Avatar::SelectMenu);
	}

	if (Button_Setting)
	{
		Button_Setting->OnClicked.RemoveDynamic(this, &UUP_Avatar::OnClick_Setting);
		Button_Setting->OnClicked.AddDynamic(this, &UUP_Avatar::OnClick_Setting);
	}

	if( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnClickSkillIcon.RemoveDynamic(this, &UUP_Avatar::OnClick_SkillIcon);
		RGameInstance->EventManager->OnClickSkillIcon.AddDynamic(this, &UUP_Avatar::OnClick_SkillIcon);
	}
	if( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnRefreshSkillInfo.RemoveDynamic(this, &UUP_Avatar::OnRefreshSkillInfo);
		RGameInstance->EventManager->OnRefreshSkillInfo.AddDynamic(this, &UUP_Avatar::OnRefreshSkillInfo);
	}

	//if (RGameInstance->UserInfo->nPageIndex != -1)
	//{
	//	if (Radio_Ability)
	//		Radio_Ability->OnCheckStateChanged.Broadcast(true);
	//	SelectMenu(RGameInstance->UserInfo->nPageIndex, true);
	//	RGameInstance->UserInfo->nPageIndex = -1;
	//}
	//else
	//{
	SelectMenuNumber = 0;
	SelectMenu(true);
	//}
}

void UUP_Avatar::NativeDestruct()
{
	/*for (auto Element : MenuArray)
	{
		if (Element == nullptr)
			continue;

		Element->RadioGroupoComponentStateChanged.Unbind();
		//Element->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Avatar::SelectMenu);
	}*/


	if (Radio_Profile)
	{
		Radio_Profile->OnCheckStateChanged.Broadcast(true);
	}

	if (PageSwitcher)
	{
		PageSwitcher->SetActiveWidgetIndex(0);
	}

	Super::NativeDestruct();
}

void UUP_Avatar::SelectMenu(/*int InIndex,*/bool InChecked)
{
	/*if (InChecked == false)
		return;

	int32 i = 0;
	for (auto Element : MenuArray)
	{
		if (Element && Element->IsChecked())
		{
			break;
		}
		else
		{
			++i;
		}
	}*/
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

	if (PageSwitcher)
	{
		PageSwitcher->SetActiveWidgetIndex(iNumber);

		auto ActiveWidget = PageSwitcher->GetChildAt(iNumber);
		if (ActiveWidget)
		{
			auto BaseWidget = Cast<URBaseWidget>(ActiveWidget);
			if (BaseWidget)
			{
				BaseWidget->SetActiveSubPage();
			}
		}
	}
}

void UUP_Avatar::OnClick_Setting()
{
	if ( IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_Setting);
	}
}

void UUP_Avatar::OnClick_SkillIcon(UUC_AvatarSkill* InSelectSkill)
{
	if (IsValid(Avatar_Popup_LevelUP))
	{
		Avatar_Popup_LevelUP->SetInfo(InSelectSkill);
		Avatar_Popup_LevelUP->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUP_Avatar::OnRefreshSkillInfo(UUC_AvatarSkill* InSelectSkill)
{
	if (IsValid(Avatar_Popup_LevelUP))
	{
		Avatar_Popup_LevelUP->SetInfo(InSelectSkill);
	}
}