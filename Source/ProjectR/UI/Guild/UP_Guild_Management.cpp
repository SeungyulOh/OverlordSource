// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Guild_Management.h"

#include "GlobalIntegrated.h"
#include "EngineMinimal.h"

#include "UI/Guild/GuildMember_ScrollItemData.h"
#include "UI/Guild/GuildManagerListScrollItemData.h"
#include "UI/Guild/GuildManagerListScrollItem.h"
#include "UI/Guild/UC_Guild_Attendance.h"
#include "UI/Guild/UC_Guild_Donation.h"
#include "UI/Guild/UC_Guild_RuneSupport.h"
#include "UI/Guild/UC_Guild_Awaiter.h"
#include "UI/Guild/UC_Guild_History.h"
#include "UI/Guild/UC_Guild_LevelUpDesc.h"
#include "UI/Guild/UC_Guild_RuneSupport.h"
#include "UI/Guild/UC_Guild_JoinType_Change.h"
#include "UI/Guild/GuildList_ScrollItemData.h"
#include "UC_Guild_Grade.h"
#include "UI/Common/UC_CurrencyDisplay.h"
#include "UI/Avatar/Item/UC_ScrollItem_AvatarIcon.h"
#include "UI/Control/RScrollView.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"
#include "UI/Component/LocalizingTextBlock.h"
#include "UI/RWidgetManager.h"
#include "UI/UIDefs.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UC_Guild_JoinType_Change.h"
#include "GameInfo/GuildInfo.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "UtilFunctionIntegrated.h"
#include "UC_GuildModifyPopup.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UP_GuildLobby.h"
#include "UP_Guild_Join.h"

#define MAXGUilDMEMBERCNT	50
#define MAXGUILDLISTCNT	50
#define MAXGUILDMEMBERINMANAGERCNT	50

void UUP_Guild_Management::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (RadioGroup_GuildManagementUI)
	{
		CheckBox_Radio_GuildInfo->OnCheckRadioCheck.AddDynamic(this, &UUP_Guild_Management::OnClick_GuildInfoUI);
		CheckBox_Radio_GuildMember->OnCheckRadioCheck.AddDynamic(this, &UUP_Guild_Management::OnClick_GuildMemberUI);
		CheckBox_Radio_GuildRanking->OnCheckRadioCheck.AddDynamic(this, &UUP_Guild_Management::OnClick_GuildRankUI);
		CheckBox_Radio_GuildManager->OnCheckRadioCheck.AddDynamic(this, &UUP_Guild_Management::OnClick_GuildManagerUI);
	}

	Button_GuildClose->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_Member_GuildClose);
	Button_ShowGuildJoinList->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_Member_JoinList);

	Button_Page_Prev->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_GuildMemberPrev);
	Button_Page_Next->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_GuildMemberNext);

	Button_Manager_Prev->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_GuildManagerPrev);
	Button_Manager_Next->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_GuildManagerNext);

	Button_PrevRanking->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_GuildRankingPrev);
	Button_NextRanking->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_GuildRankingNext);

	Button_Manager_Notice->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_OpenNotiPopup);
	Button_Manager_Desc->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_OpenDescPopup);

	ButtonGuildJoinType->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_Manager_ChangeGuildJoinType);
	ButtonGuildName->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_Manager_ChangeGuildName);

	RGameInstance->EventManager->OnRequestGuildInfo.AddDynamic(this, &UUP_Guild_Management::OnRequestGuildInfo);
	RGameInstance->EventManager->OnRefreshGuildInfoUI.AddDynamic(this, &UUP_Guild_Management::OnRefreshGuildInfoUI);
	RGameInstance->EventManager->OnRequestGuildMember.AddDynamic(this, &UUP_Guild_Management::OnRequestGuildMember);
	RGameInstance->EventManager->OnRefreshGuildMemberUI.AddDynamic(this, &UUP_Guild_Management::OnRefreshGuildMemberUI);
	RGameInstance->EventManager->OnReceiveGuestGuildInfo.AddDynamic(this, &UUP_Guild_Management::OnReceiveGuestGuildInfo);
	RGameInstance->EventManager->OnOpenGradeChangePop.AddDynamic(this, &UUP_Guild_Management::OpenChangePositionPop);
	RGameInstance->HttpClient->OnGuildContributeRankingRpDelegate.AddDynamic(this, &UUP_Guild_Management::OnGuildContributeRankingRpDelegate);

	Init();
}

void UUP_Guild_Management::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	CheckBox_Radio_GuildInfo->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Guild_Management::OnClick_GuildInfoUI);
	CheckBox_Radio_GuildMember->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Guild_Management::OnClick_GuildMemberUI);
	CheckBox_Radio_GuildRanking->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Guild_Management::OnClick_GuildRankUI);
	CheckBox_Radio_GuildManager->OnCheckRadioCheck.RemoveDynamic(this, &UUP_Guild_Management::OnClick_GuildManagerUI);

	Button_Page_Prev->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_GuildMemberPrev);
	Button_Page_Next->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_GuildMemberNext);

	Button_Manager_Prev->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_GuildManagerPrev);
	Button_Manager_Next->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_GuildManagerNext);

	Button_PrevRanking->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_GuildRankingPrev);
	Button_NextRanking->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_GuildRankingNext);

	Button_Manager_Notice->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_OpenNotiPopup);
	Button_Manager_Desc->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_OpenDescPopup);

	Button_GuildClose->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_Member_GuildClose);
	Button_ShowGuildJoinList->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_Member_JoinList);

	ButtonGuildJoinType->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_Manager_ChangeGuildJoinType);
	ButtonGuildName->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_Manager_ChangeGuildName);

	RGameInstance->EventManager->OnRequestGuildInfo.RemoveDynamic(this, &UUP_Guild_Management::OnRequestGuildInfo);
	RGameInstance->EventManager->OnReceiveGuestGuildInfo.RemoveDynamic(this, &UUP_Guild_Management::OnReceiveGuestGuildInfo);
	RGameInstance->EventManager->OnRefreshGuildInfoUI.RemoveDynamic(this, &UUP_Guild_Management::OnRefreshGuildInfoUI);
	RGameInstance->EventManager->OnRequestGuildMember.RemoveDynamic(this, &UUP_Guild_Management::OnRequestGuildMember);
	RGameInstance->EventManager->OnRefreshGuildMemberUI.RemoveDynamic(this, &UUP_Guild_Management::OnRefreshGuildMemberUI);
	RGameInstance->EventManager->OnOpenGradeChangePop.RemoveDynamic(this, &UUP_Guild_Management::OpenChangePositionPop);
	RGameInstance->HttpClient->OnGuildContributeRankingRpDelegate.RemoveDynamic(this, &UUP_Guild_Management::OnGuildContributeRankingRpDelegate);

	Super::NativeDestruct();
}

void UUP_Guild_Management::Init()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	GuildUIState = EGuildManagementTab::VE_GuildInfo;
	bGuildManager = false;
	CurrentMemberPage = 1;
	RequestMemberPage = 1;
	CurrentRankingPage = 1;
	RequestRankingPage = 1;
	CurrenManagerPage = 1;
	RequestManagerPage = 1;

	if(RadioGroup_GuildManagementUI)
		RadioGroup_GuildManagementUI->SetActiveRadioIndex(0);

	if (CachedGuildMemberList.Num() == 0)
	{
		for (int32 i = 0; i < MAXGUilDMEMBERCNT; i++)
		{
			UGuildMember_ScrollItemData* ItemData = Cast<UGuildMember_ScrollItemData>(ScrollView_GuildMemberList->CreateScrollItemData(UGuildMember_ScrollItemData::StaticClass()));
			ItemData->WidgetSize = ScrollView_GuildMemberList->GetItemInitSize();
			CachedGuildMemberList.Emplace(ItemData);
		}
	}

	if (CachedGuildRankingList.Num() == 0)
	{
		for(int32 i = 0; i < MAXGUILDLISTCNT; i++)
		{
			UGuildList_ScrollItemData* ItemData = Cast<UGuildList_ScrollItemData>(ScrollView_GuildRankingList->CreateScrollItemData(UGuildList_ScrollItemData::StaticClass()));
			ItemData->WidgetSize = ScrollView_GuildRankingList->GetItemInitSize();
			CachedGuildRankingList.Emplace(ItemData);
		}
	}

	if (CachedGuildMemberInManagerList.Num() == 0)
	{
		for (int32 i = 0; i < MAXGUILDMEMBERINMANAGERCNT; i++)
		{
			UGuildManagerListScrollItemData* ItemData = Cast<UGuildManagerListScrollItemData>(ScrollView_MemberList_InManager->CreateScrollItemData(UGuildManagerListScrollItemData::StaticClass()));
			ItemData->WidgetSize = ScrollView_MemberList_InManager->GetItemInitSize();
			CachedGuildMemberInManagerList.Emplace(ItemData);
		}
	}

 	OnRequestGuildInfo();
// 	if (RGameInstance->GuildInfo->Guest_GuildMemberList.Num() > 0)
// 	{
// 		GuildUIState = EGuildManagementTab::VE_GuildRank;
// 		RGameInstance->GuildInfo->Guest_GuildMemberList.Empty(0);
// 		RadioGroup_GuildManagementUI->SetActiveRadioIndex(2);
// 		RefreshGuildManagementUI();
// 	}
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Tab Click
void UUP_Guild_Management::RefreshGuildManagementUI()
{
	if (!IsValid(WidgetSwitcher_GuildManagementUI))
		return;

	switch (GuildUIState)
	{
	case EGuildManagementTab::VE_GuildInfo:		
		WidgetSwitcher_GuildManagementUI->SetActiveWidgetIndex(0);
		GuildInfoUpdate();
		SetGuildRaidOpenInfo();	
		break;
	case EGuildManagementTab::VE_GuildMember:	
		WidgetSwitcher_GuildManagementUI->SetActiveWidgetIndex(1);	
		OnRequestGuildMember();
		break;
	case EGuildManagementTab::VE_GuildRank:		
		WidgetSwitcher_GuildManagementUI->SetActiveWidgetIndex(2);  
		RequestGuildRanking();
		break;
	case EGuildManagementTab::VE_GuildManager:	
		WidgetSwitcher_GuildManagementUI->SetActiveWidgetIndex(3);	
		SetGuildManagerUI(false);
		break;
	}
}


void UUP_Guild_Management::RequestGuildRanking()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (0 >= RequestRankingPage)
		RequestRankingPage = 1;
	else if (10 < RequestRankingPage)
		RequestRankingPage = 10;

	UPacketFunctionLibrary::GUILD_CONTRIBUTE_RANKING_RQ(RequestRankingPage);
}

//////////////////////////////////////////////////////////////////////////
// Info Tab
void UUP_Guild_Management::OnClick_Info_Attendance()
{
	if (UC_Guild_AttendanceBook)
	{
		UC_Guild_AttendanceBook->OpenAttendance();
	}
}
void UUP_Guild_Management::OnClick_Info_Donation()
{
	if (UC_Guild_Donation)
	{
		UC_Guild_Donation->OpenDonation();
	}
}
void UUP_Guild_Management::OnClick_Info_Rune()
{
	RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_GuildDonation);
}
void UUP_Guild_Management::OnClick_Info_Boss()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->RWidgetManager ) )
		return;

	if (Switcher_GuildRaidBoss->GetActiveWidgetIndex() == 2)
	{
		if (RGameInstance->GuildInfo)
		{
			if (RGameInstance->GuildInfo->MyGuildInfo.level < 3)
			{
				UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_GuildRaid_0001")),
					ECommonPopupType::VE_OK);
			}
			else if (RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MEMBER)
			{
				UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_GuildRaid_0002")),
					ECommonPopupType::VE_OK);
			}
		}
	}
	else
	{
		RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_GuildRaid);
	}
}
void UUP_Guild_Management::OnClick_Info_Battle()
{

}
void UUP_Guild_Management::OnClick_Info_Desc()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MEMBER)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));
		return;
	}
// 	if (IsValid(GuildDesc))
// 	{
// 		UPacketFunctionLibrary::GUILD_EDIT_DESC_RQ(this, GuildDesc->GetText().ToString());
// 	}
}
void UUP_Guild_Management::OnClick_Info_Notice()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
	if (RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MEMBER)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));
		return;
	}
// 	if (IsValid(GuildNotice))
// 	{
// 		UPacketFunctionLibrary::GUILD_EDIT_NOTICE_RQ(this, GuildNotice->GetText().ToString());
// 	}
}


void UUP_Guild_Management::GuildInfoUpdate()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	const FGUILD_INFO& GuildInfo = RGameInstance->GuildInfo->MyGuildInfo;
	if(IsValid(LocalizingTextBlock_GuildName))
		LocalizingTextBlock_GuildName->SetText(FText::FromString(GuildInfo.name));
	if (IsValid(TextBlock_GuildRank))
		TextBlock_GuildRank->SetText(FText::AsNumber(GuildInfo.point_rank));
	if (IsValid(TextBlock_GuildPoint))
		TextBlock_GuildPoint->SetText(FText::AsNumber(GuildInfo.point));
	if (IsValid(TextBlock_GuildGold))
		TextBlock_GuildGold->SetText(FText::AsNumber(GuildInfo.gold));
	if (IsValid(LocalizingTextBlock_MemberCount))
		LocalizingTextBlock_MemberCount->SetText(FText::AsNumber(GuildInfo.member_count));
	if (IsValid(LocalizingTextBlock_MemberLimit))
		LocalizingTextBlock_MemberLimit->SetText(FText::AsNumber(GuildInfo.member_limit));
	if (IsValid(LocalizingTextBlock_GuildMaster))
		LocalizingTextBlock_GuildMaster->SetText(FText::FromString(RGameInstance->GuildInfo->GuildMasterData.nick));

	if (IsValid(LocalizingTextBlock_Info_Notice))
	{
		if (GuildInfo.notice.IsEmpty())
			LocalizingTextBlock_Info_Notice->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_GuildNotice")));
		else
			LocalizingTextBlock_Info_Notice->SetText(FText::FromString(GuildInfo.notice));
	}
	
	
	if (IsValid(Image_GuildLevelIcon))
	{
		FString LevelIconKey = "Icon_GuildLevel_";
		if (10 > GuildInfo.level)
			LevelIconKey += "0";
		LevelIconKey.AppendInt(GuildInfo.level);
		LevelIconKey += "_Sprite";
		UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(FName(*LevelIconKey)), Image_GuildLevelIcon);
	}
	if (IsValid(BtnGuildDesc) && IsValid(BtnGuildNotice))
	{
		if (RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MEMBER)
		{
			BtnGuildDesc->SetIsEnabled(false);
			BtnGuildNotice->SetIsEnabled(false);
		}
		else
		{
			BtnGuildDesc->SetIsEnabled(true);
			BtnGuildNotice->SetIsEnabled(true);
		}
	}
// 	if (IsValid(Button_Info_Attendance))
// 	{
// 		Button_Info_Attendance->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_Info_Attendance);
// 		Button_Info_Attendance->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_Info_Attendance);
// 	}
// 	if (IsValid(Button_Info_Donation))
// 	{
// 		Button_Info_Donation->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_Info_Donation);
// 		Button_Info_Donation->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_Info_Donation);
// 	}
// 	if (IsValid(Button_History))
// 	{
// 		Button_History->OnClicked.RemoveDynamic(this, &UUP_Guild_Management::OnClick_Manager_History);
// 		Button_History->OnClicked.AddDynamic(this, &UUP_Guild_Management::OnClick_Manager_History);
// 	}
}

//////////////////////////////////////////////////////////////////////////
// Member Tab

void UUP_Guild_Management::SetGuildMemberUI(bool ApplyData)
{
	if (ApplyData)
	{
		CurrentMemberPage = RequestMemberPage;
	}
	else
	{
		RequestMemberPage = CurrentMemberPage;
	}

	if (TextBlock_MemberPage)
	{
		TextBlock_MemberPage->SetText(FText::AsNumber(CurrentMemberPage));
	}

	SetScrollDataGuildMemberList();
}

void UUP_Guild_Management::OnClick_Member_JoinList()
{
	if (UC_Guild_Approve)
	{
		UC_Guild_Approve->OpenAwaiter();
	}
}

void UUP_Guild_Management::OnClick_Member_GuildClose()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	int32 UserKID = RGameInstance->HttpClient->GetKID();

	FFormatArgumentValue FormatArg = FText::FromString(RGameInstance->GuildInfo->MyGuildInfo.name);
	FText LocalizedText;

	if (RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MASTER)
	{
		LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error12"));
		LocalizedText = FText::Format(LocalizedText, FormatArg);

		auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			LocalizedText,
			ECommonPopupType::VE_OKCANCEL);

		if (IsValid(UP_CommonPopup))
		{
			UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UUP_Guild_Management::GuildClose);
			UP_CommonPopup->OnClickOK.AddDynamic(this, &UUP_Guild_Management::GuildClose);
		}
	}
	else
	{
		LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error11"));
		LocalizedText = FText::Format(LocalizedText, FormatArg);

		auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			LocalizedText,
			ECommonPopupType::VE_OKCANCEL);

		if (IsValid(UP_CommonPopup))
		{
			UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UUP_Guild_Management::GuildDropOut);
			UP_CommonPopup->OnClickOK.AddDynamic(this, &UUP_Guild_Management::GuildDropOut);
		}
	}
}

void UUP_Guild_Management::GuildClose()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	int32 UserKID = RGameInstance->HttpClient->GetKID();

	UPacketFunctionLibrary::GUILD_CLOSE_RQ();
}

void UUP_Guild_Management::GuildDropOut()
{
	int32 UserKID = RGAMEINSTANCE(this)->HttpClient->GetKID();

	UPacketFunctionLibrary::GUILD_MEMBER_KICK_RQ(UserKID);
}

void UUP_Guild_Management::OnClick_GuildMemberPrev()
{
	if (Button_Page_Prev)
	{
		if(RequestMemberPage > 1)
			--RequestMemberPage;

		OnRequestGuildMember();
	}
}

void UUP_Guild_Management::OnClick_GuildMemberNext()
{
	if (Button_Page_Next)
	{
		if (RequestMemberPage < 10)
			++RequestMemberPage;

		OnRequestGuildMember();
	}
}

void UUP_Guild_Management::SetScrollDataGuildMemberList()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (!IsValid(ScrollView_GuildMemberList))
		return;

	ScrollView_GuildMemberList->Init_NoClear(10);
	ScrollView_GuildMemberList->ClearScrollItemData();
	ScrollView_GuildMemberList->SetVisibility(ESlateVisibility::Visible);

	TArray<FGUILD_MEMBER> GuildMemberList = RGameInstance->GuildInfo->MyGuildMemberList;
	EGUILD_GRADE MyGrade = RGameInstance->GuildInfo->MyMemberData.grade;
	int32 MyKid = RGameInstance->GuildInfo->MyMemberData.kId;

	int32 i = 0;

	for (FGUILD_MEMBER& GuildMember : GuildMemberList)
	{
		UGuildMember_ScrollItemData* ScrollItemData = CachedGuildMemberList[i];
		if (ScrollItemData)
		{
			ScrollItemData->GuildMemberData = GuildMember;
			ScrollItemData->MyGuildGrade = MyGrade;
			ScrollItemData->MyAccountKid = MyKid;
			ScrollView_GuildMemberList->ItemDataList.Emplace(ScrollItemData);
		}
		i++;
	}
	ScrollView_GuildMemberList->InvalidateData();
}

void UUP_Guild_Management::SetScrollDataGuildManagerList()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (!IsValid(ScrollView_MemberList_InManager))
		return;

	ScrollView_MemberList_InManager->Init();
	ScrollView_MemberList_InManager->ClearScrollItemData();
	ScrollView_MemberList_InManager->SetVisibility(ESlateVisibility::Visible);

	TArray<FGUILD_MEMBER> GuildMemberList = RGameInstance->GuildInfo->MyGuildMemberList;
	EGUILD_GRADE MyGrade = RGameInstance->GuildInfo->MyMemberData.grade;
	int32 MyKid = RGameInstance->GuildInfo->MyMemberData.kId;

	int32 i = 0;

	for (FGUILD_MEMBER& GuildMember : GuildMemberList)
	{
		UGuildManagerListScrollItemData* ScrollItemData = CachedGuildMemberInManagerList[i];
		if (ScrollItemData)
		{
			ScrollItemData->GuildMemberData = GuildMember;
			ScrollItemData->MyGuildGrade = MyGrade;
			ScrollItemData->MyAccountKid = MyKid;
			ScrollItemData->IsBtnOn = true;
			ScrollView_MemberList_InManager->ItemDataList.Emplace(ScrollItemData);
		}
		i++;
	}

	ScrollView_MemberList_InManager->InvalidateData();
}

//////////////////////////////////////////////////////////////////////////
// Manager Tab
void UUP_Guild_Management::SetGuildManagerUI(bool ApplyData)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (RGameInstance->GuildInfo->MyGuildInfo.notice.IsEmpty())
		Text_Manager_Notice->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_GuildNotice")));
	else
		Text_Manager_Notice->SetText(FText::FromString(RGameInstance->GuildInfo->MyGuildInfo.notice));
	
	if (!RGameInstance->GuildInfo->MyGuildInfo.desc.IsEmpty())
		Text_Manager_Desc->SetText(FText::FromString(RGameInstance->GuildInfo->MyGuildInfo.desc));
	else
		Text_Manager_Desc->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_GuildDesc")));

	if (ApplyData)
	{
		CurrenManagerPage = RequestManagerPage;
	}
	else
	{
		RequestManagerPage = CurrenManagerPage;
	}

	if (Text_Manager_Page)
	{
		Text_Manager_Page->SetText(FText::AsNumber(CurrenManagerPage));
	}
	SetScrollDataGuildManagerList();
}

// void UUP_Guild_Management::SetGuildManagerUI(UCanvasPanel_RadioGroup*	RadioGroup_AutoGradeUp,
// 	UEditableText* Text_ChangeGuildName, UUC_CurrencyDisplay* CurrenyChangeGuildName,
// 	UTextBlock* Text_NeedLevelUpPoint, UTextBlock* Text_NeedLevelUpGold,
// 	UButton* Btn_GuildLevelUp, UTextBlock* Text_CurrentGuildLevel, UTextBlock* Text_NextGuildLevel)
// {
// 	if (!IsValid(RadioGroup_AutoGradeUp) || !IsValid(Text_ChangeGuildName) || !IsValid(CurrenyChangeGuildName) || 
// 		!IsValid(Text_NeedLevelUpPoint) || !IsValid(Text_NeedLevelUpGold) ||
// 		!IsValid(Btn_GuildLevelUp) || !IsValid(Text_CurrentGuildLevel) || !IsValid(Text_NextGuildLevel))
// 		return;
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	if (RGameInstance->GuildInfo->MyGuildInfo.auto_grade_up)
// 	{
// 		RadioGroup_AutoGradeUp->SetActiveRadioIndex(0);
// 	}
// 	else
// 	{
// 		RadioGroup_AutoGradeUp->SetActiveRadioIndex(1);
// 	}
// 
// 	Text_ChangeGuildName->SetText(FText::FromString(RGameInstance->GuildInfo->MyGuildInfo.name));
// 
// 	int32 ChangeNameQuantity = 1;
// 	CurrenyChangeGuildName->SetCurrency(EREWARD_TYPE::GEM, ChangeNameQuantity);
// 
// 	int32 CurrentGuldLevel = RGameInstance->GuildInfo->MyGuildInfo.level;
// 	int32 NextGuldLevel = CurrentGuldLevel + 1;
// 	Text_CurrentGuildLevel->SetText(FText::AsNumber(CurrentGuldLevel));
// 	Text_NextGuildLevel->SetText(FText::AsNumber(NextGuldLevel));
// 
// 	int32 NeedLevelUpPoint = 0;
// 	int32 NeedLevelUpGold = 0;
// 	FName GuildLevelUpKey = FName(*FString::FromInt(NextGuldLevel));
// 	FGuildLevelUpTableInfo* GuildTableInfo = UTableManager::GetInstancePtr()->GetGuildLevelUpRow(GuildLevelUpKey);
// 	if (GuildTableInfo)
// 	{
// 		NeedLevelUpPoint = GuildTableInfo->point;
// 		NeedLevelUpGold = GuildTableInfo->gold;
// 	}
// 
// 	Text_NeedLevelUpPoint->SetText(FText::AsNumber(NeedLevelUpPoint));
// 	Text_NeedLevelUpGold->SetText(FText::AsNumber(NeedLevelUpGold));
// 
// 	FSlateColor LevelUpColor = FSlateColor(FLinearColor::White);
// 
// 	if (RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MEMBER)
// 	{
// 		RadioGroup_AutoGradeUp->SetIsEnabled(false);
// 		Btn_GuildLevelUp->SetIsEnabled(false);
// 	}
// 	else
// 	{
// 		RadioGroup_AutoGradeUp->SetIsEnabled(true);
// 		Btn_GuildLevelUp->SetIsEnabled(true);
// 		
// 		if ((NeedLevelUpPoint <= RGameInstance->GuildInfo->MyGuildInfo.point) && (NeedLevelUpGold <= RGameInstance->GuildInfo->MyGuildInfo.gold))
// 		{
// 			LevelUpColor = FSlateColor(FLinearColor::Blue);
// 		}
// 		else
// 		{
// 			LevelUpColor = FSlateColor(FLinearColor::Red);
// 		}
// 	}
// 
// 	Text_NeedLevelUpPoint->SetColorAndOpacity(LevelUpColor);
// 	Text_NeedLevelUpGold->SetColorAndOpacity(LevelUpColor);
// }

void UUP_Guild_Management::OnClick_Manager_ChangeGuildJoinType()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	if (RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MASTER)
	{
		//RGameInstance->RWidgetManager->ShowGuildModifyPopup(EGuildModifyPopupType::VE_GuildName);
		uint8 JoinType = RGameInstance->GuildInfo->MyGuildInfo.auto_grade_up ? 0 : 1;
		UP_Popup_Guild_Manager_JoinSetting->SetVisibility(ESlateVisibility::Visible);
		UP_Popup_Guild_Manager_JoinSetting->OpenJoinType(JoinType);
	}
	else
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));
	}
}

void UUP_Guild_Management::OnClick_Manager_ChangeGuildName()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	if (RGameInstance->GuildInfo->MyMemberData.grade != EGUILD_GRADE::MEMBER)
	{
		UC_Popup_Guild_Manager_Notice->Init(EGuildModifyPopupType::VE_GuildName);
		UC_Popup_Guild_Manager_Notice->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));
	}
}

void UUP_Guild_Management::OnClick_Manager_LevelUp()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	if (RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MEMBER)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));
		return;
	}
// 
// 	int32 NextLevel = RGameInstance->GuildInfo->MyGuildInfo.level + 1;
// 	UPacketFunctionLibrary::GUILD_LEVEL_UP_RQ(this, NextLevel);
}

void UUP_Guild_Management::OnClick_GuildManagerPrev()
{
	if (Button_Manager_Prev)
	{
		if (RequestManagerPage > 1)
			--RequestManagerPage;

		OnRequestGuildMember();
	}
}

void UUP_Guild_Management::OnClick_GuildManagerNext()
{
	if (Button_Manager_Next)
	{
		if (RequestManagerPage < 10)
			++RequestManagerPage;

		OnRequestGuildMember();
	}
}

// void UUP_Guild_Management::OnClick_Manager_ChangeGuildName(UEditableText* Text_ChangeGuildName)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	if (IsValid(Text_ChangeGuildName))
// 	{
// 		UPacketFunctionLibrary::GUILD_CHANGE_NAME_RQ(this, Text_ChangeGuildName->GetText().ToString());
// 	}
// }

// void UUP_Guild_Management::OnClick_Manager_ChangeGuildName(UEditableText* Text_ChangeGuildName)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	if (IsValid(Text_ChangeGuildName))
// 	{
// 		UPacketFunctionLibrary::GUILD_CHANGE_NAME_RQ(this, Text_ChangeGuildName->GetText().ToString());
// 	}
// }

// void UUP_Guild_Management::OnClick_Manager_AutoGradeUp(bool bAuto)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	UPacketFunctionLibrary::GUILD_EDIT_AUTO_GRADE_UP_RQ(this, bAuto);
// }

void UUP_Guild_Management::OnClick_Manager_GuildBossDesc()
{

}

//////////////////////////////////////////////////////////////////////////
// Delegate


void UUP_Guild_Management::OnRequestGuildInfo()
{
	UPacketFunctionLibrary::GUILD_GET_RQ();
}

void UUP_Guild_Management::OnRefreshGuildInfoUI()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (RGameInstance->GuildInfo->IsGuildExist())
	{
		RefreshGuildManagementUI();
	}
	else
	{
		UUP_GuildLobby* GuildLobby = Cast<UUP_GuildLobby>(ParentWidget);
		if (IsValid(GuildLobby))
			GuildLobby->Switcher->SetActiveWidgetIndex(0);
	}
}

void UUP_Guild_Management::OnRequestGuildMember()
{
	if (GuildUIState == EGuildManagementTab::VE_GuildMember)
	{
		if (0 >= RequestMemberPage)
			RequestMemberPage = 1;
		else if (99 < RequestMemberPage)
			RequestMemberPage = 99;

		UPacketFunctionLibrary::GUILD_MEMBER_LIST_RQ(RequestMemberPage);
	}
	else if (GuildUIState == EGuildManagementTab::VE_GuildManager)
	{
		if (0 >= RequestManagerPage)
			RequestManagerPage = 1;
		else if (99 < RequestManagerPage)
			RequestManagerPage = 99;

		UPacketFunctionLibrary::GUILD_MEMBER_LIST_RQ(RequestManagerPage);
	}
}

void UUP_Guild_Management::OnRefreshGuildMemberUI(bool ApplyData)
{
	if (GuildUIState == EGuildManagementTab::VE_GuildMember)
		SetGuildMemberUI(ApplyData);
	else if (GuildUIState == EGuildManagementTab::VE_GuildManager)
		SetGuildManagerUI(ApplyData);
}

void UUP_Guild_Management::OnGuildContributeRankingRpDelegate(bool ApplyData)
{
	if (ApplyData)
	{
		CurrentRankingPage = RequestRankingPage;
	}
	else
	{
		RequestRankingPage = CurrentRankingPage;
	}

	if (TextBlock_RankingPage)
	{
		TextBlock_RankingPage->SetText(FText::AsNumber(CurrentRankingPage));
	}
	
	SetScrollViewUI(ScrollView_GuildRankingList, RGAMEINSTANCE(this)->GuildInfo->GuildRankingList);
}

void UUP_Guild_Management::OnReceiveGuestGuildInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_GuildVisit);
}

void UUP_Guild_Management::OnClick_GuildRankingPrev()
{
	if (Button_PrevRanking)
	{
		if(RequestRankingPage > 1)
			--RequestRankingPage;

		RequestGuildRanking();
	}
}

void UUP_Guild_Management::OnClick_GuildRankingNext()
{
	if (Button_NextRanking)
	{
		if (RequestRankingPage < 10)
			++RequestRankingPage;

		RequestGuildRanking();
	}
}

void UUP_Guild_Management::OnClick_GuildInfoUI(bool IsCheck)
{
	GuildUIState = EGuildManagementTab::VE_GuildInfo;
	WidgetSwitcher_GuildManagementUI->SetActiveWidgetIndex(0);
	GuildInfoUpdate();
	SetGuildRaidOpenInfo();
}

void UUP_Guild_Management::OnClick_GuildMemberUI(bool IsCheck)
{
	GuildUIState = EGuildManagementTab::VE_GuildMember;
	WidgetSwitcher_GuildManagementUI->SetActiveWidgetIndex(1);
	OnRequestGuildMember();
}

void UUP_Guild_Management::OnClick_GuildRankUI(bool IsCheck)
{
	GuildUIState = EGuildManagementTab::VE_GuildRank;
	WidgetSwitcher_GuildManagementUI->SetActiveWidgetIndex(2);
	RequestGuildRanking();
}

void UUP_Guild_Management::OnClick_GuildManagerUI(bool IsCheck)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MEMBER)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_BeLackingInAuthority"))));

		FTimerDelegate SelectPrevRadioCallback;
		SelectPrevRadioCallback.BindLambda([=]
		{
			switch (GuildUIState)
			{
			case EGuildManagementTab::VE_GuildInfo:
				RadioGroup_GuildManagementUI->SetActiveRadioIndex(0);
				break;
			case EGuildManagementTab::VE_GuildMember:
				RadioGroup_GuildManagementUI->SetActiveRadioIndex(1);
				break;
			case EGuildManagementTab::VE_GuildRank:
				RadioGroup_GuildManagementUI->SetActiveRadioIndex(2);
				break;
			default:
				break;
			}
		});

		RGameInstance->GetWorld()->GetTimerManager().SetTimerForNextTick(SelectPrevRadioCallback);
		return;
	}
	GuildUIState = EGuildManagementTab::VE_GuildManager;
	WidgetSwitcher_GuildManagementUI->SetActiveWidgetIndex(3);
	OnRequestGuildMember();
	//SetGuildManagerUI();
}

void UUP_Guild_Management::OnClick_OpenNotiPopup()
{
	UC_Popup_Guild_Manager_Notice->Init(EGuildModifyPopupType::VE_GuildNotice);
	UC_Popup_Guild_Manager_Notice->SetVisibility(ESlateVisibility::Visible);
}

void UUP_Guild_Management::OnClick_OpenDescPopup()
{
	UC_Popup_Guild_Manager_Notice->Init(EGuildModifyPopupType::VE_GuildDesc);
	UC_Popup_Guild_Manager_Notice->SetVisibility(ESlateVisibility::Visible);
}

void UUP_Guild_Management::SetScrollViewUI(URScrollView* ScrollView, TArray<FGUILD_INFO> dataList)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (!IsValid(ScrollView))
		return;

	ScrollView->Init_NoClear(10);
	ScrollView->ClearScrollItemData();
	ScrollView->SetVisibility(ESlateVisibility::Visible);

	TArray<FGUILD_INFO>& outGuildList = dataList;
	TArray<int32>& waitingList = RGameInstance->GuildInfo->WaitingGuildIDs;
	int32 i = 0;
	for (FGUILD_INFO& FindGuildInfo : outGuildList)
	{
		UGuildList_ScrollItemData* ScrollItemData = CachedGuildRankingList[i];
		if (ScrollItemData)
		{
			ScrollItemData->GuildInfo = FindGuildInfo;
			ScrollItemData->isWaiting = waitingList.Contains(FindGuildInfo.guild_id);
			ScrollItemData->isInfoPage = true;
			ScrollItemData->isMyGuild = RGameInstance->GuildInfo->CheckIsMyGuild(FindGuildInfo.guild_id);
			ScrollView->ItemDataList.Emplace(ScrollItemData);
		}
		i++;
	}
	ScrollView->InvalidateData();
}

void UUP_Guild_Management::OpenChangePositionPop(uint8 Grade, int32 kid, FString Nick)
{
	UC_Popup_Guild_Manager_Position->OpenGrade(Grade, kid, Nick);
}

void UUP_Guild_Management::RequestRaidOpenInfo()
{
// 	UPacketFunctionLibrary::GUILD_RAID_STATUS_RQ();
}

void UUP_Guild_Management::SetGuildRaidOpenInfo()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

// 	if(RGameInstance->GuildInfo->OpenRaid.isOpen)
// 	{
// 		if (Switcher_GuildRaidBoss)
// 			Switcher_GuildRaidBoss->SetActiveWidgetIndex(0);
// 
// 		if (Vertical_RaidInfo)
// 		{
// 			Vertical_RaidInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		}
// 
// 		if (RGameInstance->GuildInfo->OpenRaid.isOpen)
// 			bOpenRaidTimeCount = true;
// 
// 	}
// 	else // close
	{ 

		if(RGameInstance->GuildInfo->MyGuildInfo.level < 3 ||
			RGameInstance->GuildInfo->MyMemberData.grade == EGUILD_GRADE::MEMBER)
		{
			if (Switcher_GuildRaidBoss)
				Switcher_GuildRaidBoss->SetActiveWidgetIndex(2);
		}
		else
		{
			if (Switcher_GuildRaidBoss)
				Switcher_GuildRaidBoss->SetActiveWidgetIndex(1);
		}

		if (Vertical_RaidInfo)
		{
			Vertical_RaidInfo->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUP_Guild_Management::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	OpenRaidTimeTick(InDeltaTime);
}

void UUP_Guild_Management::SetOpenRaidTimeText(int32 InMin, int32 InSec)
{
	if (IsValid(Text_GuildRaidRemainTime_Min))
	{
		FString text = FText::AsNumber(InMin).ToString();
		if (InMin < 10)
			text = TEXT("0") + text;

		Text_GuildRaidRemainTime_Min->SetText(FText::FromString(text));
	}

	if (IsValid(Text_GuildRaidRemainTime_Sec))
	{
		FString text = FText::AsNumber(InSec).ToString();
		if (InSec < 10)
			text = TEXT("0") + text;

		Text_GuildRaidRemainTime_Sec->SetText(FText::FromString(text));
	}
}

void UUP_Guild_Management::OpenRaidTimeTick(float InDeltaTime)
{
// 	if (bOpenRaidTimeCount)
// 	{
// 		const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());
// 
// // 		fCurrentOpenRaidTime += InDeltaTime;
// 		if (CurrentTime.GetTicks() - nCurrentOpenRaidTime >= FTimespan::FromSeconds(1.0f).GetTicks())
// 		{
// 			nCurrentOpenRaidTime = CurrentTime.GetTicks();
// 
// 			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 			if (RGameInstance->GuildInfo == nullptr)
// 				return;
// 			if (RGameInstance->GuildInfo->OpenRaid.isOpen == false)
// 			{
// 				SetOpenRaidTimeText(0, 0);
// 				return;
// 			}
// 			int32& nRemainTime = RGameInstance->GuildInfo->OpenRaid.left;
// 			int Min = 0;
// 			int Sec = 0;
// 
// 			--nRemainTime;
// 
// 			if (nRemainTime > 0)
// 			{
// 				Min = (nRemainTime % 3600) / 60;
// 				Sec = nRemainTime % 60;
// 			}
// 
// 			bool bCloseRaid = ((Min == 0) && (Sec == 0));
// 			if (bCloseRaid)
// 			{
// 				bOpenRaidTimeCount = false;
// 			}
// 
// 			SetOpenRaidTimeText(Min, Sec);
// 		}
// 	}
}


void UUC_GuildMember_ScrollItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_GuildMember_ScrollItem::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_GuildMember_ScrollItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUC_GuildMember_ScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
	UGuildMember_ScrollItemData* pGuildData = Cast<UGuildMember_ScrollItemData>(item);
	if (!IsValid(TextBlockLevel) || !IsValid(TextBlockName) || !IsValid(TextBlockContribute))
		return;
	GuildMemberData = pGuildData->GuildMemberData;
	TextBlockName->SetText(FText::FromString(GuildMemberData.nick));
	TextBlockLevel->SetText(FText::AsNumber(GuildMemberData.level));
	TextBlockContribute->SetText(FText::AsNumber(GuildMemberData.point));

	int64 InTicks = ETimespan::TicksPerSecond * GuildMemberData.last_update;
	bool LessOneMinute = true;
	FTimespan RemainTimespan = FTimespan(InTicks);
	int32 CheckDay = RemainTimespan.GetDays();
	if (TextBlockDay && LocalizingTextBlockDay)
	{
		if (0 < CheckDay)
		{
// 			TextBlockDay->SetText(FText::AsNumber(CheckDay));
// 			TextBlockDay->SetVisibility(ESlateVisibility::Visible);
// 			LocalizingTextBlockDay->SetVisibility(ESlateVisibility::Visible);
			LessOneMinute = false;
		}
		else
		{
// 			TextBlockDay->SetVisibility(ESlateVisibility::Collapsed);
// 			LocalizingTextBlockDay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	int32 CheckHour = RemainTimespan.GetHours();
	if (TextBlockHour && LocalizingTextBlockHour)
	{
		if (0 < CheckHour)
		{
// 			TextBlockHour->SetText(FText::AsNumber(CheckHour));
// 			TextBlockHour->SetVisibility(ESlateVisibility::Visible);
// 			LocalizingTextBlockHour->SetVisibility(ESlateVisibility::Visible);
			LessOneMinute = false;
		}
		else
		{
// 			TextBlockHour->SetVisibility(ESlateVisibility::Collapsed);
// 			LocalizingTextBlockHour->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	int32 CheckMinute = RemainTimespan.GetMinutes();
	if (TextBlockMinute && LocalizingTextBlockMinute)
	{
		if (LessOneMinute)
		{
			if (0 == CheckMinute)
			{
				CheckMinute = 1;
			}
		}

		if (0 < CheckMinute)
		{
// 			TextBlockMinute->SetText(FText::AsNumber(CheckMinute));
// 			TextBlockMinute->SetVisibility(ESlateVisibility::Visible);
// 			LocalizingTextBlockMinute->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
// 			TextBlockMinute->SetVisibility(ESlateVisibility::Collapsed);
// 			LocalizingTextBlockMinute->SetVisibility(ESlateVisibility::Collapsed);
		}

		LocalizingTextBlockMinute->SetText(FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_Day_Time")),
			FText::AsNumber(CheckDay), FText::AsNumber(CheckHour, &FNumberFormattingOptions().SetMinimumIntegralDigits(2)), FText::AsNumber(CheckMinute, &FNumberFormattingOptions().SetMinimumIntegralDigits(2))));
	}

	GradeIconSwitcher->SetVisibility(ESlateVisibility::Visible);
	if (GuildMemberData.grade == EGUILD_GRADE::MASTER)
	{
		GradeIconSwitcher->SetActiveWidgetIndex(0);
		AvatarIcon->SetGuildGradeIcon(0);
	}
	else if (GuildMemberData.grade == EGUILD_GRADE::ELDER)
	{
		GradeIconSwitcher->SetActiveWidgetIndex(1);
		AvatarIcon->SetGuildGradeIcon(1);
	}
	else
	{
		GradeIconSwitcher->SetActiveWidgetIndex(2);
		AvatarIcon->SetGuildGradeIcon(2);
	}
	auto RGameInstance = RGAMEINSTANCE(this);
	auto Avatar = RGameInstance->UserInfo->GetAvatarData();

// 	if (AvatarIcon)
// 	{
// 		//todo avataricon table
// 		FName NameID;
// 		int32 TestValue = Avatar.icon - 1;
// 		if (TestValue == 0)
// 		{
// 			NameID = "PT_Avatar_Default";
// 		}
// 		else
// 		{
// 			auto ID = FString::FromInt(TestValue);
// 			auto Table = UTableManager::GetInstancePtr()->GetCharacterRow(FName(*ID));
// 			NameID = Table->PortraitTextureKey;
// 		}
// 		auto temp = AvatarIcon->Image_Hero;
// 		if (temp)
// 		{
// 			auto Image = UUIFunctionLibrary::GetIconImageFromTable(NameID);
// 			UUIFunctionLibrary::SetImageBrush(Image, temp);
// 		}
// 	}
}

// void UUC_GuildMember_ScrollItem::OnClick_Grade()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 	RGameInstance->RWidgetManager->ShowGuildGradePopup((uint8)RGameInstance->GuildInfo->MyMemberData.grade, GuildMemberData.kId, GuildMemberData.nick, (uint8)GuildMemberData.grade);
// }
//
// void UUC_GuildMember_ScrollItem::OnClick_Kick()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 
// 	FFormatArgumentValue FormatArg = FText::FromString(GuildMemberData.nick);
// 	FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Error10"));
// 	LocalizedText = FText::Format(LocalizedText, FormatArg);
// 
// 	auto UP_CommonPopup = UUIFunctionLibrary::ShowCommonPopup(
// 		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
// 		LocalizedText,
// 		ECommonPopupType::VE_OKCANCEL);
// 
// 	if (IsValid(UP_CommonPopup))
// 	{
// 		UP_CommonPopup->OnClickOK.RemoveDynamic(this, &UGuildManagerListScrollItem::MemberKick);
// 		UP_CommonPopup->OnClickOK.AddDynamic(this, &UGuildManagerListScrollItem::MemberKick);
// 	}
// }
// 
// void UUC_GuildMember_ScrollItem::MemberKick()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (RGameInstance == nullptr)
// 		return;
// 
// 	UPacketFunctionLibrary::GUILD_MEMBER_KICK_RQ(GuildMemberData.kId);
// }