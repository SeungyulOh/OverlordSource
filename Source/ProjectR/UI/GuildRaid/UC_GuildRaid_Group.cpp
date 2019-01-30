 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildRaid_Group.h"

#include "GlobalIntegrated.h"

//#include "Global/BattleFieldManager.h"

#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
//#include "UI/GuildRaid/UC_GuildRaid_Party.h"
#include "UI/GuildRaid/UC_GuildRaid_PartyMember.h"

void UUC_GuildRaid_Group::NativeConstruct()
{
	Super::NativeConstruct();

// 	SelectedMyHeroUD.Empty();
// 	ReadyToStartGame = false;
// 
// 	ClearPartyMember(SrcMember);
// 	ClearPartyMember(DestMember);
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
// 
// 	auto MyPartyMemberInfo = RGameInstance->BattleFieldManager->GetMyPartyMemberInfo();
// 	if (MyPartyMemberInfo == nullptr)
// 		return;
// 
// 	MyGroupID = RGameInstance->GameObjectMgr->GetMyGroupKey();
// 	MyPartyIndex = MyPartyMemberInfo->PartyIndex;
// 	MySlotIndex = MyPartyMemberInfo->SlotIndex;
// 
// 	// UI Setting
// 	int32 FixedPartyIndex = MyGroupID;
// 	for (auto PartyUI : PartyList)
// 	{
// 		++FixedPartyIndex;
// 
// 		if (IsValid(PartyUI))
// 		{
// 			PartyUI->SetGroupPartyData(MyGroupID, FixedPartyIndex);
// 		}
// 	}
// 
// 	// Hero Select Setting
// 	RealTimeModManagerPtr = RGameInstance->RealTimeModManager;
// 	if (RealTimeModManagerPtr.IsValid())
// 	{
// 		SelectedMyHeroUD = RealTimeModManagerPtr->MyRealTimeModHeroUD;
// 	}
// 
// 	if (IsValid(UC_HeroList))
// 	{
// 		UC_HeroList->SetLimitSelectCount(1);
// 		UC_HeroList->SetScrollViewEnable(true);
// 		UC_HeroList->Button_HeroManagement->SetIsEnabled(false);
// 
// 		if ((false == SelectedMyHeroUD.IsEmpty()) && IsValid(RGameInstance->HeroInventory))
// 		{
// 			auto HeroInfo = RGameInstance->HeroInventory->GetInventoryData(SelectedMyHeroUD);
// 			if (nullptr != HeroInfo)
// 			{
// 				UC_HeroList->SetSelectHeroID(HeroInfo->heroId);
// 			}
// 		}
// 	}
// 	int32 myActorUK = RealTimeModManagerPtr->GetMyPlayActorUK();
// 	FRTS_ACTOR* MyBattleFieldUserInfo = RealTimeModManagerPtr->Get_RTS_Actor(myActorUK);
// //	auto MyBattleFieldUserInfo = RGameInstance->BattleFieldManager->GetMyBattleFieldUserInfo();
// 	if (MyBattleFieldUserInfo == nullptr)
// 	{
// 		if (MyPartyMemberInfo->IsGroupCommander)
// 		{
// 			SetGroupStateType(EGroupStateType::VE_Master);
// 		}
// 		else
// 		{
// 			SetGroupStateType(EGroupStateType::VE_Member);
// 		}
// 	}
// 
// 	switch (CurGroupState)
// 	{
// 	case EGroupStateType::VE_ReSpawn:
// 	case EGroupStateType::VE_Member:
// 	case EGroupStateType::VE_Master:
// 		RefreshGroupUI();
// 		break;
// 	case EGroupStateType::VE_PartyMove:
// 		OnClick_PartyMove();
// 		break;
// 	case EGroupStateType::VE_Delegation:
// 		OnClick_Delegation();
// 		break;
// 	default:
// 		break;
// 	}
// 
// 	if (IsValid(RGameInstance->EventManager))
// 	{
// 		RGameInstance->EventManager->OnClick_HeroIcon_HeroManagement_HeroList.RemoveDynamic(this, &UUC_GuildRaid_Group::SelectHero);
// 		RGameInstance->EventManager->OnClick_HeroIcon_HeroManagement_HeroList.AddDynamic(this, &UUC_GuildRaid_Group::SelectHero);
// 
// 		RGameInstance->EventManager->OnPartyMemberRefresh.RemoveDynamic(this, &UUC_GuildRaid_Group::OnPartyMemberRefresh);
// 		RGameInstance->EventManager->OnPartyMemberRefresh.AddDynamic(this, &UUC_GuildRaid_Group::OnPartyMemberRefresh);
// 
// 		RGameInstance->EventManager->OnPartyMemberClick.RemoveDynamic(this, &UUC_GuildRaid_Group::OnPartyMemberClick);
// 		RGameInstance->EventManager->OnPartyMemberClick.AddDynamic(this, &UUC_GuildRaid_Group::OnPartyMemberClick);
// 	}
}

void UUC_GuildRaid_Group::NativeDestruct()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		if (IsValid(RGameInstance->EventManager))
// 		{
// 			RGameInstance->EventManager->OnClick_HeroIcon_HeroManagement_HeroList.RemoveDynamic(this, &UUC_GuildRaid_Group::SelectHero);
// 			RGameInstance->EventManager->OnPartyMemberRefresh.RemoveDynamic(this, &UUC_GuildRaid_Group::OnPartyMemberRefresh);
// 			RGameInstance->EventManager->OnPartyMemberClick.RemoveDynamic(this, &UUC_GuildRaid_Group::OnPartyMemberClick);
// 		}
// 	}
	Super::NativeDestruct();
}
// 
// void UUC_GuildRaid_Group::RefreshGroupUI()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
// 
// 	if (IsValid(Switcher_GroupStateButton))
// 	{
// 		switch (CurGroupState)
// 		{
// 		case EGroupStateType::VE_ReSpawn:
// 		case EGroupStateType::VE_Member:
// 		case EGroupStateType::VE_Master:
// 			Switcher_GroupStateButton->SetActiveWidgetIndex(0);
// 			break;
// 		default:
// 			Switcher_GroupStateButton->SetActiveWidgetIndex(1);
// 			break;
// 		}
// 	}
// 
// 	if(IsValid(Switcher_GroupStateDesc))
// 	{
// 		switch (CurGroupState)
// 		{
// 		case EGroupStateType::VE_ReSpawn:
// 		case EGroupStateType::VE_Member:
// 		case EGroupStateType::VE_Master:
// 			Switcher_GroupStateDesc->SetActiveWidgetIndex(0);
// 			break;
// 		case EGroupStateType::VE_PartyMove:
// 			Switcher_GroupStateDesc->SetActiveWidgetIndex(1);
// 			break;
// 		case EGroupStateType::VE_Delegation:
// 			Switcher_GroupStateDesc->SetActiveWidgetIndex(2);
// 			break;
// 		default:
// 			break;
// 		}
// 	}
// 	if (IsValid(UC_HeroList))
// 	{
// 		switch (CurGroupState)
// 		{
// 		case EGroupStateType::VE_ReSpawn:
// 		case EGroupStateType::VE_Member:
// 		case EGroupStateType::VE_Master:
// 			UC_HeroList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 			break;
// 		case EGroupStateType::VE_PartyMove:
// 		case EGroupStateType::VE_Delegation:
// 			UC_HeroList->SetVisibility(ESlateVisibility::Collapsed);
// 			break;
// 		default:
// 			break;
// 		}
// 	}
// 
// 	if (IsValid(PartyMoveButton))
// 	{
// 		if (EGroupStateType::VE_Member == CurGroupState || EGroupStateType::VE_ReSpawn == CurGroupState)
// 		{
// 			PartyMoveButton->SetVisibility(ESlateVisibility::Collapsed);
// 		}
// 		else
// 		{
// 			PartyMoveButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		}
// 	}
// 	if (IsValid(DelegationButton))
// 	{
// 		if (EGroupStateType::VE_Member == CurGroupState || EGroupStateType::VE_ReSpawn == CurGroupState)
// 		{
// 			DelegationButton->SetVisibility(ESlateVisibility::Collapsed);
// 		}
// 		else
// 		{
// 			DelegationButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		}
// 	}
// 
// 	if (IsValid(GameStartButton))
// 	{
// 		if (ReadyToStartGame)
// 		{
// 			GameStartButton->SetIsEnabled(false);
// 		}
// 		else
// 		{
// 			GameStartButton->SetIsEnabled(true);
// 		}
// 	}
// }
// 
// void UUC_GuildRaid_Group::SelectHero(FString InHeroUD)
// {
// 	if (false == RealTimeModManagerPtr.IsValid())
// 		return;
// 
// 	SelectedMyHeroUD = InHeroUD;
// 
// 	RealTimeModManagerPtr->Req_PartyJoin(SelectedMyHeroUD);
// }
// 
// void UUC_GuildRaid_Group::OnPartyMemberRefresh(int32 InPartyIndex, int32 InSlotIndex, bool IsMyData)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	UUC_GuildRaid_Party* PartyUI = GetPartyUI(InPartyIndex);
// 	if (IsValid(PartyUI))
// 	{
// 		PartyUI->RefreshGroupPartyData(InPartyIndex, InSlotIndex);
// 	}
// 
// 	EGroupStateType BeforeGroupState = CurGroupState;
// 
// 	if (IsMyData)
// 	{
// 		MyPartyIndex = InPartyIndex;
// 		MySlotIndex = InSlotIndex;
// 
// 		if (CurGroupState != EGroupStateType::VE_ReSpawn)
// 		{
// 			auto MyPartyMemberInfo = RGameInstance->BattleFieldManager->GetMyPartyMemberInfo();
// 			if (MyPartyMemberInfo == nullptr)
// 				return;
// 
// 			if (MyPartyMemberInfo->IsGroupCommander)
// 			{
// 				SetGroupStateType(EGroupStateType::VE_Master);
// 			}
// 			else
// 			{
// 				SetGroupStateType(EGroupStateType::VE_Member);
// 			}
// 
// 			RefreshGroupUI();
// 		}
// 	}
// }
// 
// void UUC_GuildRaid_Group::OnPartyMemberClick(int32 InPartyIndex, int32 InSlotIndex, bool BtnSelect)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	if (EGroupStateType::VE_ReSpawn == CurGroupState || EGroupStateType::VE_Member == CurGroupState || EGroupStateType::VE_Master == CurGroupState)
// 		return;
// 
// 	if (EGroupStateType::VE_Delegation == CurGroupState)
// 	{
// 		auto PartyMemberInfo = RGameInstance->BattleFieldManager->GetPartyMemberInfo(InPartyIndex, InSlotIndex);
// 		if (PartyMemberInfo == nullptr)
// 			return;
// 		if (PartyMemberInfo->PartyIndex == MyPartyIndex && PartyMemberInfo->SlotIndex == MySlotIndex)
// 			return;
// 	}
// 
// 	bool ClickEnable = true;
// 	bool SelectMember = false;
// 
// 	if (SrcMember.PartySlotData.partyIdx == InPartyIndex && SrcMember.PartySlotData.slotIdx == InSlotIndex)
// 	{
// 		ClearPartyMember(SrcMember);
// 		SelectMember = false;
// 	}
// 	else if (DestMember.PartySlotData.partyIdx == InPartyIndex && DestMember.PartySlotData.slotIdx == InSlotIndex)
// 	{
// 		ClearPartyMember(DestMember);
// 		SelectMember = false;
// 	}
// 	else if (REALTIME_PARTYID_NONE == SrcMember.PartySlotData.partyIdx)
// 	{
// 		SetPartyMember(SrcMember, InPartyIndex, InSlotIndex);
// 		SelectMember = true;
// 	}
// 	else if(REALTIME_PARTYID_NONE == DestMember.PartySlotData.partyIdx)
// 	{
// 		SetPartyMember(DestMember, InPartyIndex, InSlotIndex);
// 		SelectMember = true;
// 	}
// 	else
// 	{
// 		ClickEnable = false;
// 	}
// 
// 	if (ClickEnable)
// 	{
// 		UUC_GuildRaid_Party* PartyUI = GetPartyUI(InPartyIndex);
// 		if (IsValid(PartyUI))
// 		{
// 			PartyUI->ClickGroupPartyData(InPartyIndex, InSlotIndex, SelectMember);
// 		}
// 	}
// }
// 
// void UUC_GuildRaid_Group::OnClick_PartyMove()
// {
// 	SetGroupStateType(EGroupStateType::VE_PartyMove);
// 	RefreshGroupUI();
// 
// 	ClearPartyMember(SrcMember);
// 	ClearPartyMember(DestMember);
// }
// 
// void UUC_GuildRaid_Group::OnClick_Delegation()
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
// 
// 	SetGroupStateType(EGroupStateType::VE_Delegation);
// 	RefreshGroupUI();
// 
// 	SetPartyMember(SrcMember, MyPartyIndex, MySlotIndex);
// 	ClearPartyMember(DestMember);
// 
// 	UUC_GuildRaid_Party* PartyUI = GetPartyUI(MyPartyIndex);
// 	if (IsValid(PartyUI))
// 	{
// 		PartyUI->ClickGroupPartyData(MyPartyIndex, MySlotIndex, true);
// 	}
// }
// 
// void UUC_GuildRaid_Group::OnClick_MasterAction(bool bExecute)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (false == IsValid(RGameInstance))
// 		return;
// 
// 	UUC_GuildRaid_Party* PartyUI = nullptr;
// 	PartyUI = GetPartyUI(SrcMember.PartySlotData.partyIdx);
// 	if (IsValid(PartyUI))
// 	{
// 		PartyUI->ClickGroupPartyData(SrcMember.PartySlotData.partyIdx, SrcMember.PartySlotData.slotIdx, false);
// 	}
// 	PartyUI = GetPartyUI(DestMember.PartySlotData.partyIdx);
// 	if (IsValid(PartyUI))
// 	{
// 		PartyUI->ClickGroupPartyData(DestMember.PartySlotData.partyIdx, DestMember.PartySlotData.slotIdx, false);
// 	}
// 
// 	if (bExecute)
// 	{
// 		if (SrcMember.InvalidData && DestMember.InvalidData)
// 		{
// 			if (EGroupStateType::VE_PartyMove == CurGroupState || EGroupStateType::VE_Delegation == CurGroupState)
// 			{
// 				bool isChangeCommander = (EGroupStateType::VE_Delegation == CurGroupState ? true : false);
// 				//TODO: must delete
// // #if WITH_PROUD_NET_BINDING
// // 				UProudNetManager::GetInstancePtr()->m_ModRoomProxy.Req_PartyUpdate(Proud::HostID::HostID_Server, Proud::RmiContext::ReliableSend, UProudNetManager::pvpServerKey, 
// // 																				isChangeCommander, SrcMember.PartySlotData, DestMember.PartySlotData);
// // #endif
// 			}
// 		}
// 	}
// 
// 	ClearPartyMember(SrcMember);
// 	ClearPartyMember(DestMember);
// 
// // 	auto MyBattleFieldHero = RGameInstance->BattleFieldManager->GetMyBattleFieldHero();
// // 	if (IsValid(MyBattleFieldHero))
// // 	{
// // 		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_RealTimeGroup);
// // 	}
// // 	else
// // 	{
// // 		SetGroupStateType(EGroupStateType::VE_Master);
// // 		RefreshGroupUI();
// // 	}
// }
// 
// void UUC_GuildRaid_Group::OnClick_GameStart()
// {
// 	if (ReadyToStartGame)
// 		return;
// 	if (false == RealTimeModManagerPtr.IsValid())
// 		return;
// 	if (SelectedMyHeroUD.IsEmpty())
// 		return;
// 
// 	if (IsValid(UC_HeroList))
// 	{
// 		UC_HeroList->SetScrollViewEnable(false);
// 	}
// 
// 	if (IsValid(GameStartButton))
// 	{
// 		GameStartButton->SetIsEnabled(false);
// 	}
// 
// 	ReadyToStartGame = true;
// 	RealTimeModManagerPtr->Req_Ready();
// }
// 
// //////////////////////////////////////////////////////////////////////////
// 
// UUC_GuildRaid_Party* UUC_GuildRaid_Group::GetPartyUI(int32 InPartyIndex)
// {
// 	for (auto PartyUI : PartyList)
// 	{
// 		if (IsValid(PartyUI))
// 		{
// 			if (PartyUI->PartyIndex == InPartyIndex)
// 			{
// 				return PartyUI;
// 			}
// 		}
// 	}
// 
// 	return nullptr;
// }
// 
// void UUC_GuildRaid_Group::ClearPartyMember(FPartyUpdateReqData& PartyUpdateReqData)
// {
// 	PartyUpdateReqData.InvalidData = false;
// 
// 	PartyUpdateReqData.PartySlotData.isGroupCommander = false;
// 	PartyUpdateReqData.PartySlotData.partyIdx = REALTIME_PARTYID_NONE;
// 	PartyUpdateReqData.PartySlotData.slotIdx = 0;
// }
// 
// void UUC_GuildRaid_Group::SetPartyMember(FPartyUpdateReqData& PartyUpdateReqData, int32 InPartyIndex, int32 InSlotIndex)
// {
// 	PartyUpdateReqData.InvalidData = true;
// 
// 	PartyUpdateReqData.PartySlotData.isGroupCommander = false;
// 	PartyUpdateReqData.PartySlotData.partyIdx = InPartyIndex;
// 	PartyUpdateReqData.PartySlotData.slotIdx = InSlotIndex;
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		auto MyPartyMemberInfo = RGameInstance->BattleFieldManager->GetPartyMemberInfo(InPartyIndex, InSlotIndex);
// 		if (MyPartyMemberInfo)
// 		{
// 			PartyUpdateReqData.PartySlotData.isGroupCommander = MyPartyMemberInfo->IsGroupCommander;
// 		}
// 	}
// }