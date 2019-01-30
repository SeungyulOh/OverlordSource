// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"

#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Control/RScrollView.h"
#include "UI/Common/UC_Notify.h"

#include "Network/PacketFunctionLibrary.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketDataEnum.h"
#include "Network/HttpRequestClient.h"
#include "UI/UI_MailBox/UP_SelectRewardPopup.h"

#include "MailListScrollData.h"

#include "UP_MailBox.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Component/CheckBox_Radio.h"


void UUP_MailBox::NativeConstruct()
{
	Super::NativeConstruct();

	Init();

	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		if (IsValid(RGameInstance->UserInfo))
		{
			RGameInstance->MailManager->OnMailListRp.RemoveDynamic(this, &UUP_MailBox::Get_Gift_List_Rp);
			RGameInstance->MailManager->OnMailListRp.AddDynamic(this, &UUP_MailBox::Get_Gift_List_Rp);
			RGameInstance->MailManager->OnGiftOpenRp.RemoveDynamic(this, &UUP_MailBox::Get_Gift_Open_Rp);
			RGameInstance->MailManager->OnGiftOpenRp.AddDynamic(this, &UUP_MailBox::Get_Gift_Open_Rp);
			
			RGameInstance->MailManager->OnSpecialMailListRp.RemoveDynamic(this, &UUP_MailBox::Get_Special_Gift_List_Rp);
			RGameInstance->MailManager->OnSpecialMailListRp.AddDynamic(this, &UUP_MailBox::Get_Special_Gift_List_Rp);
			RGameInstance->MailManager->OnSpecialGiftOpenRp.RemoveDynamic(this, &UUP_MailBox::Get_Special_Gift_Open_Rp);
			RGameInstance->MailManager->OnSpecialGiftOpenRp.AddDynamic(this, &UUP_MailBox::Get_Special_Gift_Open_Rp);

			RGameInstance->MailManager->OnEventRewardRp.RemoveDynamic(this,&UUP_MailBox::OnEventRewardRp);
			RGameInstance->MailManager->OnEventRewardRp.AddDynamic(this, &UUP_MailBox::OnEventRewardRp);
			RGameInstance->MailManager->OnEventRewardOpen.RemoveDynamic(this, &UUP_MailBox::OnClick_EventRewardOpen);
			RGameInstance->MailManager->OnEventRewardOpen.AddDynamic(this, &UUP_MailBox::OnClick_EventRewardOpen);

			UPacketFunctionLibrary::GIFT_LIST_RQ(Scroll_MailList->GetScrollItemDataCount(), 0);
		}
	}

	if (IsValid(Btn_GetAll))
	{
		Btn_GetAll->OnClicked.RemoveDynamic(this, &UUP_MailBox::AllReceive);
		Btn_GetAll->OnClicked.AddDynamic(this, &UUP_MailBox::AllReceive);
	}

	if (IsValid(Checkbox_Gift))
	{
		Checkbox_Gift->OnCheckStateChanged.RemoveDynamic(this, &UUP_MailBox::OnClick_CheckBox);
		Checkbox_Gift->OnCheckStateChanged.AddDynamic(this, &UUP_MailBox::OnClick_CheckBox);
	}
	if (IsValid(Checkbox_Event))
	{
		Checkbox_Event->OnCheckStateChanged.RemoveDynamic(this, &UUP_MailBox::OnClick_CheckBox);
		Checkbox_Event->OnCheckStateChanged.AddDynamic(this, &UUP_MailBox::OnClick_CheckBox);
	}

	if (IsValid(SelectRewardPopup))
		SelectRewardPopup->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(WidgetSwitcher_Gift))
		WidgetSwitcher_Gift->SetActiveWidgetIndex(0);

	if (IsValid(WidgetSwitcher_Event))
		WidgetSwitcher_Event->SetActiveWidgetIndex(1);
}

void UUP_MailBox::Init()
{
	

	

	if (Scroll_MailList)
	{
		if (!Scroll_MailList->ScrollItemList.Num())
			Scroll_MailList->Init_NoClear(MAXSCROLLITEM);

		Scroll_MailList->ClearScrollItemData();

		if (!CachedItemDataList.Num())
		{
			for (size_t i = 0; i < MAXSCROLLITEMDATA; ++i)
			{
				UMailListScrollData* ItemData = Cast<UMailListScrollData>(Scroll_MailList->CreateScrollItemData(UMailListScrollData::StaticClass()));
				ItemData->WidgetSize = Scroll_MailList->GetItemInitSize();
				CachedItemDataList.Emplace(ItemData);
			}
		}
	}
	
	UpdateList();
}

void UUP_MailBox::Click_Btn_Close()
{

}


void UUP_MailBox::Get_Gift_List_Rp(const TArray<FMAIL>& InMailList)
{
	UE_LOG(LogUI, Log, TEXT("Get Mail Num - %d"), InMailList.Num());
	
	//auto RGameInstance = RGAMEINSTANCE(this);
	UWorld* World = this->GetWorld();
	bool bResult = false;

	Scroll_MailList->ClearScrollItemData();
	for (size_t i = 0; i < InMailList.Num() ; ++i)
	{
		UMailListScrollData* MailListScrollData = nullptr;

		if (i >= MAXSCROLLITEMDATA)
			break;

		if (CachedItemDataList.IsValidIndex(i))
			MailListScrollData = CachedItemDataList[i];

		if (IsValid(MailListScrollData))
		{
			MailListScrollData->bEventMail		= false;
			MailListScrollData->MailUD			= InMailList[i].mailUD;
			//MailListScrollData->kId				= MailList_Elem.kId;
			MailListScrollData->Text_Title		= FText::FromString(InMailList[i].subject);
			MailListScrollData->Text_NickName	= FText::FromString(InMailList[i].from);
			MailListScrollData->Text_ReceiveDay = FText::FromString(InMailList[i].created);
			FString Temp = InMailList[i].expire;
			int32 TestNum = Temp.Find("T");
			if(TestNum > 0)
				Temp = Temp.Left(TestNum);
			//MailListScrollData->Text_RemainDay	= FText::FromString(MailList_Elem.expire);
			MailListScrollData->Text_RemainDay = FText::FromString(Temp);
			MailListScrollData->ScrollItemInfo.Empty();
			//MailListScrollData->ScrollItemInfo.SetNum(MAIL_MAX_REWARDCOUNT);
			MailListScrollData->ScrollItemInfo.SetNum(InMailList[i].attach.Num());
			//Reward Icon
			int32 Index = 0;
			for (auto& RewardsElem : InMailList[i].attach)
			{
				if (MailListScrollData->ScrollItemInfo.IsValidIndex(Index))
				{
					UUIFunctionLibrary::GetRewardIconByRewardType(RewardsElem, MailListScrollData->ScrollItemInfo[Index]);
				}
				++Index;
			}

			Scroll_MailList->ItemDataList.Emplace(MailListScrollData);
		}
	}

	UpdateList();
}


void UUP_MailBox::Get_Gift_Open_Rp(const TArray<FREWARD>& InRewards)
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;
	
	if (IsValid(RGameInstance->MailManager))
	{
		for (auto Element : RGameInstance->MailManager->GetReceiveList())
		{
			Scroll_MailList->RemoveScrollItemData(Element);
		}
		Scroll_MailList->ClearScrollItemData();
		RGameInstance->MailManager->CleanReceiveList();
	}

	/*if (IsValid(RGameInstance->HttpClient))
	{
		RGameInstance->HttpClient->SetBusy(false);
	}*/

	UPacketFunctionLibrary::GIFT_LIST_RQ(Scroll_MailList->GetScrollItemDataCount(), 0);
	
}

void UUP_MailBox::Get_Special_Gift_List_Rp(const TArray<FMAIL>& InMailList)
{
	UE_LOG(LogUI, Log, TEXT("Get Mail Num - %d"), InMailList.Num());

	//auto RGameInstance = RGAMEINSTANCE(this);
	UWorld* World = this->GetWorld();
	bool bResult = false;
	for (auto& MailList_Elem : InMailList)
	{
		UMailListScrollData* MailListScrollData = Cast<UMailListScrollData>(Scroll_MailList->AddScrollItemData(UMailListScrollData::StaticClass()));
		if (IsValid(MailListScrollData))
		{
			MailListScrollData->bEventMail = true;
			MailListScrollData->MailUD = MailList_Elem.mailUD;
			//MailListScrollData->kId				= MailList_Elem.kId;
			MailListScrollData->Text_Title = FText::FromString(MailList_Elem.subject);
			MailListScrollData->Text_NickName = FText::FromString(MailList_Elem.from);
			MailListScrollData->Text_ReceiveDay = FText::FromString(MailList_Elem.created);
			FString Temp = MailList_Elem.expire;
			int32 TestNum = Temp.Find("T");
			if (TestNum > 0)
				Temp = Temp.Left(TestNum);
			//MailListScrollData->Text_RemainDay	= FText::FromString(MailList_Elem.expire);
			MailListScrollData->Text_RemainDay = FText::FromString(Temp);
			MailListScrollData->ScrollItemInfo.Empty();
			//MailListScrollData->ScrollItemInfo.SetNum(MAIL_MAX_REWARDCOUNT);

			MailListScrollData->RewardInfo = MailList_Elem.attach;
			//Reward Icon

			
		}
	}

	UpdateList();
}

void UUP_MailBox::Get_Special_Gift_Open_Rp(const TArray<FREWARD>& InRewards)
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	if (IsValid(RGameInstance->MailManager))
	{
		for (auto Element : RGameInstance->MailManager->GetReceiveList())
		{
			Scroll_MailList->RemoveScrollItemData(Element);
		}
		RGameInstance->MailManager->CleanReceiveList();
	}

	UPacketFunctionLibrary::SPECIAL_GIFT_LIST_RQ(Scroll_MailList->GetScrollItemDataCount(), 0);
}

void UUP_MailBox::Get_Gift_List_More_Rp()
{
	//GIFT_LIST_MORE_RQ
}


void UUP_MailBox::NativeDestruct()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->MailManager->OnMailListRp.RemoveDynamic(this, &UUP_MailBox::Get_Gift_List_Rp);
		RGameInstance->MailManager->OnGiftOpenRp.RemoveDynamic(this, &UUP_MailBox::Get_Gift_Open_Rp);
		RGameInstance->MailManager->OnSpecialMailListRp.RemoveDynamic(this, &UUP_MailBox::Get_Special_Gift_List_Rp);
		RGameInstance->MailManager->OnSpecialGiftOpenRp.RemoveDynamic(this, &UUP_MailBox::Get_Special_Gift_Open_Rp);

		RGameInstance->MailManager->OnEventRewardRp.RemoveDynamic(this, &UUP_MailBox::OnEventRewardRp);
		RGameInstance->MailManager->OnEventRewardOpen.RemoveDynamic(this, &UUP_MailBox::OnClick_EventRewardOpen);
	}

	Super::NativeDestruct();
}

void UUP_MailBox::UpdateList()
{
	Scroll_MailList->InvalidateData();

	//int32 MailNum = Scroll_MailList->GetScrollItemDataCount();
	//OnSetTextCount(MailNum);

	if (Scroll_MailList->GetScrollItemDataCount() == 0)
	{
		if (EmptyMail_CanvasPanel)
		{
			EmptyMail_CanvasPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		//OnEmptyMailText(true);
		if (Btn_GetAll)
		{
			Btn_GetAll->SetIsEnabled(false);
		}
	}
	else
	{
		if (EmptyMail_CanvasPanel)
		{
			EmptyMail_CanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
		//OnEmptyMailText(false);
		if (Btn_GetAll)
		{
			Btn_GetAll->SetIsEnabled(true);
		}
	}
}

void UUP_MailBox::AllReceive()
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
	auto MailManager = RGameInstance->MailManager;
	if (!IsValid(MailManager))
		return;
	MailManager->IsReceiveAll = true;
	//if (!IsValid(RGameInstance->NotifyManager))
	//	return;

	//int32 kId = 0;
	
	TArray<URScrollItemData*>& ItemDatas = Scroll_MailList->GetScrollItemAll();
	TArray<FString> mailUDs;
	mailUDs.Reserve(ItemDatas.Num());
	for (auto& Element : ItemDatas)
	{
		UMailListScrollData* MailListScrollData = Cast<UMailListScrollData>(Element);
		if (IsValid(MailListScrollData))
		{
			if(MailListScrollData->ScrollItemInfo.IsValidIndex(0) && MailListScrollData->ScrollItemInfo[0].ToolTipEventType == EToolTipEventTypeEnum::VE_Hero)
				continue;
			MailManager->AddReceiveList(Element);
			mailUDs.Emplace(MailListScrollData->MailUD);
			//kId = MailListScrollData->kId;
		}
	}
	
	/*if (IsValid(RGameInstance->HttpClient))
	{
		RGameInstance->HttpClient->SetBusy(true);
	}*/

	if(mailUDs.Num() != 0)
		UPacketFunctionLibrary::GIFT_OPEN_RQ(mailUDs);

	MailManager->SetNotify(false);
	//RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_MailBox, false);
}

void UUP_MailBox::OnClick_MailCategory(EMailBoxType InType)
{
	CurrentMailBoxType = InType;
	switch (InType)
	{
	case EMailBoxType::VE_Gift:
		Init();
		UPacketFunctionLibrary::GIFT_LIST_RQ(Scroll_MailList->GetScrollItemDataCount(), 0);
		if (IsValid(Btn_GetAll))
			Btn_GetAll->SetVisibility(ESlateVisibility::Visible);
		break;
	case EMailBoxType::VE_Event:
		Init();
		UPacketFunctionLibrary::SPECIAL_GIFT_LIST_RQ(Scroll_MailList->GetScrollItemDataCount(), 0);
		if (IsValid(Btn_GetAll))
			Btn_GetAll->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		Init();
		break;
	}
}

void UUP_MailBox::OnClick_CheckBox(bool _bChecked)
{
	if (Checkbox_Gift->IsChecked() && _bChecked)
		OnClick_MailCategory(EMailBoxType::VE_Gift);
	else if (Checkbox_Event->IsChecked() && _bChecked)
		OnClick_MailCategory(EMailBoxType::VE_Event);
}

void UUP_MailBox::OnClick_EventRewardOpen(const FString& InMailUD, const TArray<FREWARD>& InRewards)
{
	if (IsValid(SelectRewardPopup))
	{
		SelectRewardPopup->Init(InMailUD, InRewards);
		SelectRewardPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SelectRewardPopup->PlayAni(TEXT("Popup_Appear"));
	}
}

void UUP_MailBox::OnEventRewardRp(const FString& RewardId)
{

}