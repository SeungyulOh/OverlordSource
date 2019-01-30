// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Common/TopBar.h"
#include "UI/Control/RScrollView.h"
#include "UI/Avatar/Item/UC_ScrollItem_AvatarIcon.h"
#include "UI/Avatar/Item/ItemData_AvatarIcon.h"

#include "UP_AvatarIcon.h"

void UUP_AvatarIcon::NativeConstruct()
{
	Super::NativeConstruct();

	RGameInstance = RGAMEINSTANCE(this);

	RGameInstance->EventManager->OnUpdateAvatarIcon.RemoveDynamic(this, &UUP_AvatarIcon::SetIconScroll);
	RGameInstance->EventManager->OnUpdateAvatarIcon.AddDynamic(this, &UUP_AvatarIcon::SetIconScroll);
	

	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UUP_AvatarIcon::ClickButton);
		Button_Click->OnClicked.AddDynamic(this, &UUP_AvatarIcon::ClickButton);
	}
}

void UUP_AvatarIcon::NativeDestruct()
{
	if (RGameInstance)
	{
		RGameInstance->EventManager->OnUpdateAvatarIcon.RemoveDynamic(this, &UUP_AvatarIcon::SetIconScroll);
	}
	IsInitActive = false;

	Super::NativeDestruct();
}

void UUP_AvatarIcon::SetActiveSubPage()
{
	if (IsInitActive)
	{
		Refresh();
		return;
	}
	//else

	IsInitActive = true;

	UPacketFunctionLibrary::AVATAR_ICON_LIST_RQ();
	//SetIconScroll();
}

void UUP_AvatarIcon::Refresh()
{
	auto Avatar = RGameInstance->UserInfo->GetAvatarData();
	Scroll_Icon->SetSelectScrollListIndex(Avatar.icon - 1);
}

void UUP_AvatarIcon::SetUITopBarString()
{
	/*auto TopbarWidget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TopBar);
	if (TopbarWidget)
	{
		auto Target = Cast<UTopBar>(TopbarWidget);
		if (Target)
		{
			Target->SetCurrentUIString(EUIPageEnum::UIPage_AvatarIcon);
		}
	}*/
}

void UUP_AvatarIcon::SetIconScroll()
{
	if (IsValid(Scroll_Icon) == false)
		return;

	Scroll_Icon->Init();
	Scroll_Icon->ClearScrollItemData();

	auto IconList = RGameInstance->UserInfo->GetIconList();

	TArray<FName> RowNames = UTableManager::GetInstancePtr()->GetCharacterRowNames();
	UItemData_AvatarIcon* DefaultScrollData = Cast<UItemData_AvatarIcon>(Scroll_Icon->AddScrollItemData(UItemData_AvatarIcon::StaticClass()));
	if (IsValid(DefaultScrollData))
	{
		DefaultScrollData->Image_Hero = "PT_Avatar_Default";
	}

	int32 i = 1;
	int32 LockNum = IconList.Num();
	//GetAvatarIconTableRow
	for (auto Element : RowNames)
	{
		UItemData_AvatarIcon* UseAvatarScrollData = Cast<UItemData_AvatarIcon>(Scroll_Icon->AddScrollItemData(UItemData_AvatarIcon::StaticClass()));
		if (IsValid(UseAvatarScrollData))
		{
			auto Table = UTableManager::GetInstancePtr()->GetCharacterRow(Element);
			UseAvatarScrollData->Image_Hero = Table->PortraitTextureKey;
			UseAvatarScrollData->TableKey = Table->DisplayName;
			bool bEnable = (i < LockNum); //false == lock
			UseAvatarScrollData->bSelectEnable = bEnable;
		}
		++i;
	}

	Scroll_Icon->InvalidateData();

	auto Avatar = RGameInstance->UserInfo->GetAvatarData();
	Scroll_Icon->SetSelectScrollListIndex(Avatar.icon - 1);
}

void UUP_AvatarIcon::ClickButton()
{
	TArray<int32> SelectedIndex = Scroll_Icon->GetSelectedScrollItem();
	int32 Index = 0;
	if (SelectedIndex.Num() > 0)
	{
		Index = SelectedIndex[0];
	}
	++Index; //server startnum(1)
	UPacketFunctionLibrary::AVATAR_ICON_SET_RQ(Index);
}