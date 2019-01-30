// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ChatItem.h"
#include "UI/Control/RScrollView.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GamePlayBluePrintFunction.h"

void UUC_ChatItem::NativeConstruct()
{
	Super::NativeConstruct();

	InvalidateNative();
}

void UUC_ChatItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//if(MyGeometry.GetLocalSize().Y < 50)
	ItemSizeChangedByTextSize();
}

void UUC_ChatItem::InvalidateData()
{
	UUC_ChatItemData* data =  Cast<UUC_ChatItemData>(ItemData);
	EntranceSizeBox->SetVisibility(ESlateVisibility::Collapsed);
	if (data->currTab == EChatListEnum::VE_WHISPER)
	{
		EntranceSizeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetNickName(FText::FromString(data->InviteData.m_Nick));
		SetMessage(FText::FromString(data->InviteData.m_ModName));
		//UPacketFunctionLibrary::req_mat
		Button_Goto->OnClicked.Clear();
		Button_Goto->OnClicked.AddDynamic(this, &UUC_ChatItem::OnClick_GoTo);
	}
	else
	{
		if (data->itemData.UIType == EChatItemUIType::VE_INVITE)
		{
			Button_Invite->SetVisibility(ESlateVisibility::Visible);
			Button_Invite->OnClicked.Clear();
			Button_Invite->OnClicked.AddDynamic(this, &UUC_ChatItem::OnClick_Invite);
		}
		SetNickName(FText::FromString(ChatItem.Who));
		SetMessage(ChatItem.Message);

		switch (ChatItem.UIType)
		{
		case EChatItemUIType::VE_NORMAL:
			SetColor(FColor(255, 255, 255));
			break;
		case EChatItemUIType::VE_GET:
			SetColor(FColor(0, 0, 255));
			break;
		case EChatItemUIType::VE_EVENT:
			SetColor(FColor(0, 255, 0));
			break;
		case EChatItemUIType::VE_GM:
			SetColor(FColor(255, 0, 0));
			break;
		case EChatItemUIType::VE_WHISPER:
			SetColor(FColor(0, 0, 255));
			break;
		case EChatItemUIType::VE_INOUTALARM:
			SetColor(FColor(192, 192, 192));
			break;
		case EChatItemUIType::VE_INVITE:
			SetColor(FColor(255, 0, 0));
			break;
		default:
			SetColor(FColor(0, 0, 0));
			break;
		}
	}
}

void UUC_ChatItem::OnClick_Invite()
{
	UUC_ChatItemData* data = Cast<UUC_ChatItemData>(ItemData);

	data->OnClickPlayer.Broadcast();
}

void UUC_ChatItem::OnClick_GoTo()
{
	UUC_ChatItemData* data = Cast<UUC_ChatItemData>(ItemData);
	if (data->InviteData.m_ModType == uint8(EPVP::CS))
	{
		RGAMEINSTANCE(this)->RealTimeModManager->JoinNumberString = FString::FromInt(data->InviteData.m_PartyNo);
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		BaseStageInfo->Reset();
		BaseStageInfo->StageID = data->InviteData.m_ModId;
		UPacketFunctionLibrary::TOWER_BOSS_PLAY_CHECK_RQ(data->InviteData.m_ModId.ToString());
	}
	else if (data->InviteData.m_ModType == uint8(EPVP::MR))
	{
		RGAMEINSTANCE(this)->RealTimeModManager->JoinNumberString = FString::FromInt(data->InviteData.m_PartyNo);
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		BaseStageInfo->Reset();
		BaseStageInfo->StageID = data->InviteData.m_ModId;
		UPacketFunctionLibrary::TOWER_BOSS_PLAY_CHECK_RQ(data->InviteData.m_ModId.ToString());
	}
// 	else if (data->InviteData.m_ModType == uint8(EPVP::SR))
// 	{
// 	}
}

EChatItemUIType UUC_ChatItem::GetItemUIType() const
{
	return ChatItem.UIType;
}

FString UUC_ChatItem::GetWho() const
{
	return ChatItem.Who;
}

FText UUC_ChatItem::GetMessage() const
{
	return ChatItem.Message;
}

FString UUC_ChatItem::GetTimestamp() const
{
	// TODO:...
	return ChatItem.Timestamp.ToString(TEXT("%H:%M"));
}

// FChatUser UUC_ChatItem::GetChatUser() const
// {
// 	return FChatUser(ChatItem.kId, ChatItem.Who);
// }

void UUC_ChatItem::InvalidateNative()
{
	if(IsValid(Text_Timestamp))
		Text_Timestamp->SetVisibility(ESlateVisibility::Collapsed);

	if(EntranceSizeBox)
		EntranceSizeBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_ChatItem::OnClick_Name()
{
	//OnClickPlayerName.Broadcast(GetChatUser());
}

void UUC_ChatItem::SetNickName(FText setText)
{
	if (Text_Title)
		Text_Title->SetRichText(setText);
}

void UUC_ChatItem::SetColor(FColor setColor)
{
	if (Text_Title)
		Text_Title->SetRichColor(setColor);
	if (Text_Message)
		Text_Message->SetColorAndOpacity(FSlateColor(FLinearColor::FromSRGBColor(setColor)));
}

void UUC_ChatItem::SetMessage(FText setText)
{
	if (Text_Message)
		Text_Message->SetText(setText);
}

void UUC_ChatItem::ItemSizeChangedByTextSize()
{
 	cachedGeo = Text_Message->GetCachedGeometry();
 	messageSize = cachedGeo.GetLocalSize();
	/*messageSize = Text_Message->GetDesiredSize();*/
	FVector2D itemSize;
	itemSize.Set(560, messageSize.Y + 35);
	SetSizeChange(itemSize, true);
	Cast<UCanvasPanelSlot>(Overlay_ScrollItem->Slot)->SetSize(itemSize);
}

void UUC_ChatItem::SetSizeChange(FVector2D ChangeSize, bool bParentUpdate)
{
	Size = ChangeSize;
	SetCanvasPanelSize(this, Size);

	if (bParentUpdate)
	{
		if (!ParentScrollView)
			return;

		ParentScrollView->SetScrollUpdate(this, FVector2D::ZeroVector);
		//set itemdata widget size update
		for (int32 i = 0; i < ParentScrollView->ItemDataList.Num(); i++)
		{
			if (ItemData == ParentScrollView->ItemDataList[i])
				ParentScrollView->ItemDataList[i]->WidgetSize = ChangeSize;
		}
	}
}

void UUC_ChatItem::SetChatItem(const FChatItem& InChatItem)
{
	ChatItem = InChatItem;
}

void UUC_ChatItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	UUC_ChatItemData* chatData = Cast<UUC_ChatItemData>(item);
	ChatItem = chatData->itemData;
	ParentScrollView = ScrollView;
}

void UUC_ChatItem::SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{

}

void UUC_ChatItem::SetSelected(bool bSelected)
{

}

void UUC_InviteItem::NativeConstruct()
{

}

void UUC_InviteItem::InvalidateData()
{

}

void UUC_InviteItem::OnClick_AcceptInvite()
{

}
