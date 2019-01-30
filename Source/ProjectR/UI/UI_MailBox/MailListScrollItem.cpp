// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "UtilFunctionIntegrated.h"

#include "SharedConstants/GlobalConstants.h"

#include "GlobalIntegrated.h"


#include "UI/RWidgetManager.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Component/LocalizingTextBlock.h"

#include "Public/Components/TextBlock.h"
#include "Public/Components/Image.h"
#include "Public/Components/Button.h"

#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"

#include "MailListScrollData.h"
#include "MailListScrollItem.h"

#define MAX_GIFTICON 4

void UMailListScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	GiftIcons.Reset(MAX_GIFTICON);
	GiftIcons.Emplace(ItemIcon1);
	GiftIcons.Emplace(ItemIcon2);
	GiftIcons.Emplace(ItemIcon3);
	GiftIcons.Emplace(ItemIcon4);

	if (IsValid(Button_Click))
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UMailListScrollItem::ClickReciveButton);
		Button_Click->OnClicked.AddDynamic(this, &UMailListScrollItem::ClickReciveButton);
	}
}

bool UMailListScrollItem::IsValidData()
{
	if (IsValid(Text_Title) == false)
		return false;
	if (IsValid(Text_NickName) == false)
		return false;
	if (IsValid(Text_ReceiveDay) == false)
		return false;
	if (IsValid(Text_RemainDay) == false)
		return false;

	return true;
}

void UMailListScrollItem::Clear()
{
	//if (IsValidData() == true)
	{
		
	}
}

void UMailListScrollItem::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	SetMailScrollItem();
}

void UMailListScrollItem::SetMailScrollItem()
{
	if (IsValidData() == true)
	{
		Clear();
		
		if (IsValid(ItemData))
		{
			auto MailScrollData = Cast<UMailListScrollData>(ItemData);
			if (IsValid(MailScrollData))
			{
				bEventMail = MailScrollData->bEventMail;
				MailUD = MailScrollData->MailUD;
				kId = MailScrollData->kId;
				Text_Title->SetText(MailScrollData->Text_Title);
				Text_NickName->SetText(MailScrollData->Text_NickName);
				Text_ReceiveDay->SetText(MailScrollData->Text_ReceiveDay);
				Text_RemainDay->SetText(MailScrollData->Text_RemainDay);
				
				//set reward 
				const auto& UI_Info = MailScrollData->ScrollItemInfo;
				
				for (int32 i = 0; i < MAX_GIFTICON; ++i)
				{
					if (GiftIcons.IsValidIndex(i) == false)
						break;

					if (UI_Info.IsValidIndex(i))
					{
						GiftIcons[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						// TODO: fix
						//GiftIcons[i]->SetItemIconType(EItemIconType::VE_UIRenderer);
						GiftIcons[i]->SetItemIconStructure(UI_Info[i]);
					}
					else
					{
						GiftIcons[i]->SetVisibility(ESlateVisibility::Collapsed);
					}
				}

				/*for (auto& ItemIconElem : GiftIcons)
				{
					ItemIconElem->SetVisibility(ESlateVisibility::Collapsed);
				}

				int32 ItemIconIndex = 0;
				for (auto& ItemIconInfoElem : UI_Info)
				{
					if (GiftIcons.IsValidIndex(ItemIconIndex))
					{
						GiftIcons[ItemIconIndex]->SetVisibility(ESlateVisibility::Visible);
						GiftIcons[ItemIconIndex]->SetItemIconType(EItemIconType::VE_UIRenderer);
						GiftIcons[ItemIconIndex]->SetItemIconStructure(ItemIconInfoElem);
						++ItemIconIndex;
					}
				}*/
			}
		}
	}
}

void UMailListScrollItem::ClickReciveButton()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		TArray<FString> mailUDs;
		mailUDs.Empty();
		mailUDs.Emplace(MailUD);

		if (bEventMail)
		{
			UMailListScrollData* MailScrollData = Cast<UMailListScrollData>(ItemData);
			if (IsValid(MailScrollData))
			{
				RGameInstance->MailManager->AddReceiveList(this->ItemData);
				RGameInstance->MailManager->OnEventRewardOpen.Broadcast(MailUD, MailScrollData->RewardInfo);
			}
		}
		else
		{
			if (IsValid(RGameInstance->MailManager))
			{
				RGameInstance->MailManager->AddReceiveList(this->ItemData);
			}
			/*if (IsValid(RGameInstance->HttpClient))
			{
				RGameInstance->HttpClient->SetBusy(true);
			}*/
			UPacketFunctionLibrary::GIFT_OPEN_RQ(mailUDs);
		}
	}
}