// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ASyncPVPDeckSetting.h"
#include "Global/ArenaManager.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"
#include "Global/MirrorDungeonManager.h"
#include "Global/RGameInstance.h"
#include "UIFunctionLibrary.h"
#include "Renewal_BattleFunctionLibrary.h"
#include "UtilFunctionLibrary.h"
#include "Client_BattleFunctionLibrary.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/UI_Common/TopBar.h"

#define DECK_BUTTON_COUNT 9

#define ARENA_AutoDeck_311	0
#define ARENA_AutoDeck_212	1
#define ARENA_AutoDeck_113	2

const int32 STATE_NONE = 0;
const int32 STATE_DAILY_BONUS = 1;
const int32 STATE_DEFENCE_CHANGE = 2;

void UASyncPVPDeckSetting::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = RGAMEINSTANCE(this);
	UArenaManager* ArenaManager = GameInstance->ArenaManager;

	if (ArenaManager->bMirrorDeckSetting)
		variables.DeckType = EPVPDeckType::PVPMIRROR;
	else
		variables.DeckType = EPVPDeckType::PVPARENA;

	/*Blackboard Setting Start*/
	Renderer.Blackboard = &(UUIFunctionLibrary::GetGenericScrollview()->Blackboard);
	Renderer.Blackboard->SelectedHeroUDs.Init(TEXT(""), DECK_BUTTON_COUNT);
	Renderer.Blackboard->OnBBStateChanged.AddDynamic(this, &UASyncPVPDeckSetting::Update_FromBlackboard);
	/*Blackboard Setting End*/

	/*StateController Setting Start*/
	Renderer.StateController = &(UUIFunctionLibrary::GetGenericScrollview()->StateController);
	/*StateController Setting End*/
	
	variables.CheckBoxArray[ARENA_AutoDeck_311]->OnCheckStateChanged.AddDynamic(this, &UASyncPVPDeckSetting::OnClick_Auto_311);
	variables.CheckBoxArray[ARENA_AutoDeck_212]->OnCheckStateChanged.AddDynamic(this, &UASyncPVPDeckSetting::OnClick_Auto_212);
	variables.CheckBoxArray[ARENA_AutoDeck_113]->OnCheckStateChanged.AddDynamic(this, &UASyncPVPDeckSetting::OnClick_Auto_113);

	if (variables.DeckType == EPVPDeckType::PVPARENA)
	{
		if (ArenaManager->isDefenceSetting())
		{
			if (IsValid(variables.SaveButton))
			{
				variables.SaveButton->OnClicked.RemoveDynamic(this, &UASyncPVPDeckSetting::DefenceSetting);
				variables.SaveButton->OnClicked.AddDynamic(this, &UASyncPVPDeckSetting::DefenceSetting);
			}
		}
		else
		{
			if (IsValid(variables.Button_ArenaCountCharge))
			{
				variables.Button_ArenaCountCharge->OnClicked.RemoveDynamic(this, &UASyncPVPDeckSetting::Button_ClickDailyBonusAdd);
				variables.Button_ArenaCountCharge->OnClicked.AddDynamic(this, &UASyncPVPDeckSetting::Button_ClickDailyBonusAdd);
			}

			if (IsValid(variables.Button_ResetTarget))
			{
				variables.Button_ResetTarget->OnClicked.RemoveDynamic(this, &UASyncPVPDeckSetting::OnClickDefenceReset);
				variables.Button_ResetTarget->OnClicked.AddDynamic(this, &UASyncPVPDeckSetting::OnClickDefenceReset);
			}

			if (IsValid(variables.EnterButton))
			{
				variables.EnterButton->OnClicked.RemoveDynamic(this, &UASyncPVPDeckSetting::StartArenaMatch);
				variables.EnterButton->OnClicked.AddDynamic(this, &UASyncPVPDeckSetting::StartArenaMatch);
			}

// 			if (IsValid(variables.Button_ResetTarget))
// 			{
// 				variables.Button_ResetTarget->OnClicked.Clear();
// 				variables.Button_ResetTarget->OnClicked.AddDynamic(this, &UASyncPVPDeckSetting::OnClick_Button_ResetTarget);
// 			}
		}
	}
	else
	{
		if (IsValid(variables.EnterButton))
		{
			variables.EnterButton->OnClicked.RemoveDynamic(this, &UASyncPVPDeckSetting::StartMirrorMatch);
			variables.EnterButton->OnClicked.AddDynamic(this, &UASyncPVPDeckSetting::StartMirrorMatch);
		}
	}

	if (IsValid(GameInstance->HttpClient))
	{
		GameInstance->HttpClient->OnContentResetRpDelegate.Clear();
		GameInstance->HttpClient->OnContentResetRpDelegate.AddDynamic(this, &UASyncPVPDeckSetting::CountResetRp);
		GameInstance->HttpClient->OnContentResetStatusRpDelegate.Clear();
		GameInstance->HttpClient->OnContentResetStatusRpDelegate.AddDynamic(this, &UASyncPVPDeckSetting::CountResetStatusRp);
	}

	ArenaManager->m_bIsAlreadyPlayArena = false;
	Renderer.variables = &variables;
	Renderer.Prepare();
	Renderer.Render();
}

void UASyncPVPDeckSetting::NativeDestruct()
{
	Renderer.Blackboard->OnBBStateChanged.RemoveDynamic(this, &UASyncPVPDeckSetting::Update_FromBlackboard);

	variables.CheckBoxArray[ARENA_AutoDeck_311]->OnCheckStateChanged.RemoveDynamic(this, &UASyncPVPDeckSetting::OnClick_Auto_311);
	variables.CheckBoxArray[ARENA_AutoDeck_212]->OnCheckStateChanged.RemoveDynamic(this, &UASyncPVPDeckSetting::OnClick_Auto_212);
	variables.CheckBoxArray[ARENA_AutoDeck_113]->OnCheckStateChanged.RemoveDynamic(this, &UASyncPVPDeckSetting::OnClick_Auto_113);

	UUP_GenericScrollview* genericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (genericScrollview)
	{
		genericScrollview->Appear(false);
	}

	Super::NativeDestruct();
}

void UASyncPVPDeckSetting::CountResetRp(bool bResult)
{
	Renderer.Prepare();
	Renderer.Render();
}

void UASyncPVPDeckSetting::CountResetStatusRp(int32 nResetCount)
{
	if (CurrencyChargeState == STATE_DAILY_BONUS)
		DailyBonusCharge(nResetCount);
	else if (CurrencyChargeState == STATE_DEFENCE_CHANGE)
		DefenceChangeCharge(nResetCount);
}

void UASyncPVPDeckSetting::Update_FromBlackboard()
{
	Renderer.Render();
}

void UASyncPVPDeckSetting::DefenceSetting()
{
	UArenaManager* ArenaManager = RGAMEINSTANCE(this)->ArenaManager;

	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);

	TArray<FString>		ReceiveString;
	bool				bAddedSlot = false;
	TArray<URBaseDeckWidget*> DeckArr = Renderer.Blackboard->TargetWidgetArray;
	ReceiveString.Reserve(DeckArr.Num());
	for (int32 index = 0; index < DeckArr.Num(); ++index)
	{
		UUC_Arena_DeckBtn* CurrDeck = Cast<UUC_Arena_DeckBtn>(DeckArr[index]);
		if (DeckArr.IsValidIndex(index) == false || CurrDeck->IsEmptyDeck())
		{
			ReceiveString.Emplace("");
			continue;
		}

		FString		HeroUD;
		CurrDeck->GetMyDeckHeroUD(HeroUD);
		ReceiveString.Emplace(HeroUD);
		bAddedSlot = true;
	}
	if (!bAddedSlot)
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000045"))), ECommonPopupType::VE_OK);
		return;
	}

	if (variables.DeckType == EPVPDeckType::PVPARENA)
		UPacketFunctionLibrary::ARENA_DEFENSE_DECK_SET_RQ(ReceiveString);
	else
		UPacketFunctionLibrary::MIRROR_DEFENSE_DECK_SET_RQ(ReceiveString);

	if (variables.DeckType == EPVPDeckType::PVPARENA)
	{
		RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ArenaLobby);
	}
}

void UASyncPVPDeckSetting::StartArenaMatch()
{
	checkf(GameInstance->RInventory != nullptr, TEXT("[%s]RInventory is Empty"), __FUNCTION__);
	checkf(GameInstance->ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);

	FStaminaUseTableInfo*	StaminaUseTable = UTableManager::GetInstancePtr()->GetStaminaUseTableRow(FName("ARENA"));

	checkf(StaminaUseTable != nullptr, TEXT("[%s]StaminaUseTable is Empty"), __FUNCTION__);

	int32		StaminaPVP = GameInstance->RInventory->GetStaminaPVP();

	if (StaminaPVP < StaminaUseTable->StaminaPVP)
	{
		UUP_CommonPopup* CommonPopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Popup_Campaign_Start_StaminaCondition")), ECommonPopupType::VE_OKCANCEL);

		if (IsValid(CommonPopup))
		{
			CommonPopup->OnClickOK.Clear();
			CommonPopup->OnClickOK.AddDynamic(this, &UASyncPVPDeckSetting::GoShop);
		}
		return;
	}

	//int32	ReceivePosition = 0;
	int32	Count = 0;
	int32	deck_place_num = variables.m_MyHeroDeckBtnList.Num();

	for (int32 n = 0; n < deck_place_num; ++n)
	{
		if (variables.m_MyHeroDeckBtnList.IsValidIndex(n) == false)
			return;

		if (variables.m_MyHeroDeckBtnList[n]->IsEmptyDeck() == false)
			Count++;
	}

	GameInstance->ArenaManager->m_ArenaMyHeroUDs = Renderer.Blackboard->SelectedHeroUDs;

	int32 power = 0;

	for (FString& ud : Renderer.Blackboard->SelectedHeroUDs)
		power += UClient_BattleFunctionLibrary::GetHeroCombatStat(ud);

	GameInstance->ArenaManager->m_MyPower = power;


	if (Count == 0)
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName("UI_Colosseum_Warn_NoEnter")), ECommonPopupType::VE_OK);
		return;
	}
	else if (Count < 5)
	{
		UUP_CommonPopup* common = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000044"))), ECommonPopupType::VE_OKCANCEL);

		if (common)
		{
			common->Button_OK->OnClicked.Clear();
			common->Button_OK->OnClicked.AddDynamic(GameInstance->ArenaManager, &UArenaManager::RequestPlayStart);
		}
		return;
	}

	GameInstance->ArenaManager->RequestPlayStart();
}

void UASyncPVPDeckSetting::StartMirrorMatch()
{
	UMirrorDungeonManager* MirrorManager = GameInstance->MirrorDungeonManager;
	checkf(MirrorManager != nullptr, TEXT("[%s]MirrorManager is Empty"), __FUNCTION__);

	int32 Count = 0;
	int32 deck_place_num = variables.m_MyHeroDeckBtnList.Num();
	for (int32 n = 0; n < deck_place_num; ++n)
	{
		if (variables.m_MyHeroDeckBtnList.IsValidIndex(n) == false)
			return;

		if (false == variables.m_MyHeroDeckBtnList[n]->IsEmptyDeck())
		{
			Count++;
		}
	}
	MirrorManager->m_MirrorMyHeroUDs = Renderer.Blackboard->SelectedHeroUDs;

	if (Count == 0)
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000045"))),
			ECommonPopupType::VE_OK);

		return;
	}
	else if (Count < 5)
	{
		UUP_CommonPopup* common = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000044"))),
			ECommonPopupType::VE_OKCANCEL);

		if (common)
		{
			common->OnClickOK.RemoveDynamic(MirrorManager, &UMirrorDungeonManager::RequestPlayStart);
			common->OnClickOK.AddDynamic(MirrorManager, &UMirrorDungeonManager::RequestPlayStart);
		}
		return;
	}
	MirrorManager->RequestPlayStart();
}

void UASyncPVPDeckSetting::GoShop()
{
	checkf(GameInstance->RWidgetManager != nullptr, TEXT("[%s]GameInstance->RWidgetManager is Empty"), __FUNCTION__);

	GameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_Shop);
}

void UASyncPVPDeckSetting::OnClick_Auto_311(bool isChecked)
{
	Renderer.SetAutoDeckRender(ARENA_AutoDeck_311);
	//variables.AutoSelectRadioGroup->SetActiveRadioIndex(ARENA_AutoDeck_311);
	SetRadioSwitcher(ARENA_AutoDeck_311);
}

void UASyncPVPDeckSetting::OnClick_Auto_212(bool isChecked)
{
	Renderer.SetAutoDeckRender(ARENA_AutoDeck_212);
	SetRadioSwitcher(ARENA_AutoDeck_212);
	//variables.AutoSelectRadioGroup->SetActiveRadioIndex(ARENA_AutoDeck_212);
}

void UASyncPVPDeckSetting::OnClick_Auto_113(bool isChecked)
{
	Renderer.SetAutoDeckRender(ARENA_AutoDeck_113);
	SetRadioSwitcher(ARENA_AutoDeck_113);
	//variables.AutoSelectRadioGroup->SetActiveRadioIndex(ARENA_AutoDeck_113);
}

void UASyncPVPDeckSetting::Button_ClickDailyBonusAdd()
{
	CurrencyChargeState = STATE_DAILY_BONUS;
	UPacketFunctionLibrary::CONTENT_RESET_STATUS_RQ(TEXT("ArenaDailyBonus"), TEXT(""));
}

void UASyncPVPDeckSetting::OnClickDefenceReset()
{
	CurrencyChargeState = STATE_DEFENCE_CHANGE;
	UPacketFunctionLibrary::CONTENT_RESET_STATUS_RQ(TEXT("ArenaDefenderReset"), TEXT(""));
}

// void UASyncPVPDeckSetting::OnClick_Button_ArenaCountCharge()
// {
// 	//CurrencyChargeState = ECurrencyChargeState::VE_DailyBonus;
// 	UPacketFunctionLibrary::CONTENT_RESET_STATUS_RQ(TEXT("ArenaDailyBonus"), TEXT(""));
// }
// 
// void UASyncPVPDeckSetting::OnClick_Button_ResetTarget()
// {
// 	//CurrencyChargeState = ECurrencyChargeState::VE_DefenceChange;
// 	UPacketFunctionLibrary::CONTENT_RESET_STATUS_RQ(TEXT("ArenaDefenderReset"), TEXT(""));
// }

void UASyncPVPDeckSetting::ShowRejectRequestPopupDelay()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	RGameInstance->GetTimerManager().ClearTimer(DelayTimer);

	UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("USK_up_ItemManage_OptionValueShortage"))),
		ECommonPopupType::VE_OK);
}

void UASyncPVPDeckSetting::DailyBonusCharge(int32 nResetCount)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	URInventory*		RInventory = RGameInstance->RInventory;
	int32 nHaveGemCount = RGameInstance->RInventory->GetGem();

	auto TableInfo = UTableManager::GetInstancePtr()->GetContentResetTableRow(FName(TEXT("ArenaDailyBonus")));
	if (TableInfo == nullptr)
		return;

	if (RInventory->GetArenaDailyBonusToken() == 0 && TableInfo->resetLimitCountPerDay <= nResetCount)
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000061")), ECommonPopupType::VE_OK);
		return;
	}

	int32 nNeedGemCount = TableInfo->currencyValue*(nResetCount + 1);

	if (nNeedGemCount <= nHaveGemCount)
	{
		if (RInventory->GetArenaDailyBonusToken() > 0)
		{
			UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000062")), ECommonPopupType::VE_OK);
		}
		else
		{
			if (IsValid(variables.UC_Arena_charge_popup))
			{
				variables.UC_Arena_charge_popup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				variables.UC_Arena_charge_popup->SetText(nNeedGemCount, RInventory->GetArenaDailyBonusToken(), true);
			}
		}
	}
	else
		RGameInstance->GetTimerManager().SetTimer(DelayTimer, this, &UASyncPVPDeckSetting::ShowRejectRequestPopupDelay, 0.1f);
	CurrencyChargeState = STATE_NONE;
}

void UASyncPVPDeckSetting::DefenceChangeCharge(int32 nResetCount)
{

	GameInstance = RGAMEINSTANCE(this);
	UArenaManager*	ArenaManager = GameInstance->ArenaManager;
	URInventory*	RInventory = GameInstance->RInventory;

	int32 nHaveGemCount = GameInstance->RInventory->GetGem();

	auto TableInfo = UTableManager::GetInstancePtr()->GetContentResetTableRow(FName(TEXT("ArenaDefenderReset")));
	if (TableInfo == nullptr)
		return;

	if (RInventory->GetArenaDefenderResetToken() == 0 && TableInfo->resetLimitCountPerDay <= nResetCount)
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000063")), ECommonPopupType::VE_OK);
		return;
	}

	int32 nNeedGemCount = TableInfo->currencyValue*(nResetCount + 1);

	if (RInventory->GetArenaDefenderResetToken() != 0)
		ArenaManager->RequestDefenceReset();
	else
	{
		if (nNeedGemCount <= nHaveGemCount)
		{
			if (IsValid(variables.UC_Arena_charge_popup))
			{
				variables.UC_Arena_charge_popup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				variables.UC_Arena_charge_popup->SetText(nNeedGemCount, RInventory->GetArenaDefenderResetToken(), false);
			}
		}
		else
			GameInstance->GetTimerManager().SetTimer(DelayTimer, this, &UASyncPVPDeckSetting::ShowRejectRequestPopupDelay, 0.1f);
	}
	CurrencyChargeState = STATE_NONE;
}

void UASyncPVPDeckSetting::SetRadioSwitcher(int32 idx)
{
	Renderer.ReleaseAllRadioSwitcher();
	variables.SwitcherArray[idx]->SetActiveWidgetIndex(1);
}

void FRenderer_ASyncPVP::SetMyInfo()
{
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);
	checkf(UserInfo != nullptr, TEXT("[%s]UserInfo is Empty"), __FUNCTION__);
	checkf(GuildInfo != nullptr, TEXT("[%s]GuildInfo is Empty"), __FUNCTION__);
	checkf(MirrorManager != nullptr, TEXT("[%s]MirrorManager is Empty"), __FUNCTION__);

	//FARENA_STATUS& my_arena_info = ArenaManager->GetArenaInfo();
	FAVATAR& Avatar = GameInstance->UserInfo->GetAvatarData();

	//TArray<FString>& UDList = variables->bIsArena? ArenaManager->m_ArenaMyHeroUDs : MirrorManager->m_MirrorMyHeroUDs;
	int32& nMyPower = variables->bIsArena ? ArenaManager->m_MyPower : MirrorManager->m_MyPower;

	FText info_text;

	if (ArenaManager->isDefenceSetting() == true)
	{
		info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(nMyPower));
		if (IsValid(variables->TextBlock_DefencePower))
			variables->TextBlock_DefencePower->SetText(info_text);
	}
	else
	{
		info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(Avatar.level));
		if (IsValid(variables->Avatar_Level))
			variables->Avatar_Level->SetText(info_text);

		info_text = FText::FromString(Avatar.nick);
		if (IsValid(variables->Avatar_Name))
			variables->Avatar_Name->SetText(info_text);

		info_text = FText::FromString(GuildInfo->MyGuildInfo.name);
		if (IsValid(variables->Avatar_Guild))
			variables->Avatar_Guild->SetText(info_text);

		info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(nMyPower));
		if (IsValid(variables->Avatar_Power))
			variables->Avatar_Power->SetText(info_text);
	}
}

void FRenderer_ASyncPVP::SetCompetitorInfo()
{
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);
	checkf(MirrorManager != nullptr, TEXT("[%s]MirrorManager is Empty"), __FUNCTION__);

	FBATTLE_USER& MatchTargetInfo = variables->DeckType == EPVPDeckType::PVPARENA ? ArenaManager->m_MatchList : MirrorManager->m_MatchList;
	int32& Power = variables->bIsArena ? ArenaManager->m_TargetPower : MirrorManager->m_TargetPower;

	FText info_text;

	Power = 0;

	if (ArenaManager->isDefenceSetting() == false)
	{
		info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(MatchTargetInfo.level));
		if (IsValid(variables->Enemy_Level))
			variables->Enemy_Level->SetText(info_text);

		info_text = FText::FromString(MatchTargetInfo.nick);
		if (IsValid(variables->Enemy_Name))
			variables->Enemy_Name->SetText(info_text);

		info_text = FText::FromString(MatchTargetInfo.guild);
		if (IsValid(variables->Enemy_Guild))
			variables->Enemy_Guild->SetText(info_text);

		int32 deck_place_num = variables->m_EnemyHeroDeckBtnList.Num();
		for (int32 n = 0; n < deck_place_num; ++n)
		{
			if (variables->m_EnemyHeroDeckBtnList.IsValidIndex(n) == false)
				continue;

			if (false == variables->m_EnemyHeroDeckBtnList[n]->IsEmptyDeck())
			{
				if (variables->m_EnemyHeroDeckBtnList[n])
					Power += variables->m_EnemyHeroDeckBtnList[n]->GetHeroPower();
			}
		}

		info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(Power));
		if (IsValid(variables->Enemy_Power))
			variables->Enemy_Power->SetText(info_text);
	}
}

void FRenderer_ASyncPVP::SetCompetitorDeck()
{
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);
	checkf(TablePtr != nullptr, TEXT("[%s]TablePtr is Empty"), __FUNCTION__);
	checkf(MirrorManager != nullptr, TEXT("[%s]MirrorManager is Empty"), __FUNCTION__);

	if (variables->DeckType == EPVPDeckType::PVPARENA && ArenaManager->isDefenceSetting() == true)
		return;

	TArray<FString>&	UDList = variables->bIsArena ? ArenaManager->m_ArenaMyHeroUDs : MirrorManager->m_MirrorMyHeroUDs;
	FBATTLE_USER&		MatchList = variables->DeckType == EPVPDeckType::PVPMIRROR ? MirrorManager->m_MatchList : ArenaManager->m_MatchList;
	int32				deck_count = MatchList.decks.defense.Num();

	for (int32 index = 0; index < deck_count; ++index)
	{
		FHERO_ICON_INFO icon_info;
		FString& hero_info = MatchList.decks.defense[index];
		FEQUIPED_HERO SelectHero;
		bool bFind = false;
		FMIRROR mirror = MirrorManager->GetSelectedMirrorSlotData();

		for (auto& Hero : MatchList.heros)
		{
			if (Hero.heroUD == hero_info)
			{
				SelectHero = Hero;
				bFind = true;
				break;
			}
		}

		if (!bFind)
			continue;

		FCharacterTableInfo*	CharTable = TablePtr->GetCharacterRow(SelectHero.heroId);
		checkf(CharTable != nullptr, TEXT("[%s]CharTable is Empty"), __FUNCTION__);
		UUIFunctionLibrary::GetHeroIconInfoByEquipedHero(icon_info, SelectHero);

		if (variables->DeckType == EPVPDeckType::PVPMIRROR)
			SetDeckBtnHP(mirror, icon_info, SelectHero, index);

		if ((variables->m_EnemyHeroDeckBtnList).IsValidIndex(index))
			variables->m_EnemyHeroDeckBtnList[index]->SetTargetHeroIconInfo(MatchList, icon_info, hero_info);
	}
}

void FRenderer_ASyncPVP::SetCompetitorRefreshOverlay()
{
	checkf(RInventory != nullptr, TEXT("[%s]RInventory is Empty"), __FUNCTION__);

	if (RInventory->GetArenaDefenderResetToken() <= 0)
	{
		if (IsValid(variables->Text_TargetCount))
			variables->Text_TargetCount->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(variables->Text_TargetMax))
			variables->Text_TargetMax->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(variables->Text_TargetReset))
			variables->Text_TargetReset->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Arena_DeckSetting_ResetOpponentCharge"));
	}
	else
	{
		if (IsValid(variables->Text_TargetCount))
			variables->Text_TargetCount->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (IsValid(variables->Text_TargetMax))
			variables->Text_TargetMax->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (IsValid(variables->Text_TargetReset))
			variables->Text_TargetReset->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "UI_Arena_DeckSetting_ResetOpponent"));
	}

	if (IsValid(variables->Text_TargetCount))
		variables->Text_TargetCount->SetText(FText::AsNumber(RInventory->GetArenaDefenderResetToken()));
}

void FRenderer_ASyncPVP::ReleaseAllRadioSwitcher()
{
	for (int32 i = 0; i < variables->SwitcherArray.Num(); i++)
		variables->SwitcherArray[i]->SetActiveWidgetIndex(0);
}

int32 FRenderer_ASyncPVP::GetListNum(TArray<FString> list)
{
	int32 cnt = 0;

	for (FString& str : list)
	{
		if (!str.IsEmpty())
			cnt++;
	}

	return cnt;
}

void FRenderer_ASyncPVP::SetPickCountNPowerUI(TArray<FString> herolist)
{
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);
	checkf(MirrorManager != nullptr, TEXT("[%s]MirrorManager is Empty"), __FUNCTION__);

	int32 pickCount = GetListNum(herolist);
	int32 power = 0;

	for (FString& ud : herolist)
		power += UClient_BattleFunctionLibrary::GetHeroCombatStat(ud);

	if (IsValid(variables->Pick_PlaceCount) && IsValid(variables->Avatar_Power))
	{
		FText info_text;

		info_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(pickCount));
		variables->Pick_PlaceCount->SetText(info_text);

		if (variables->DeckType == EPVPDeckType::PVPARENA && ArenaManager->isDefenceSetting())
			variables->TextBlock_DefencePower->SetText(FText::AsNumber(power));
		else
			variables->Avatar_Power->SetText(FText::AsNumber(power));

		int32 ImgWdgCount = variables->PickProgressImageArray.Num();
		for (int32 n = 0; n < ImgWdgCount; ++n)
		{
			if (variables->PickProgressImageArray.IsValidIndex(n) == false)
				continue;

			if (variables->PickProgressImageArray[n])
			{
				if (n < pickCount)
					variables->PickProgressImageArray[n]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				else
					variables->PickProgressImageArray[n]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void FRenderer_ASyncPVP::Prepare()
{
	//
	GameInstance = RGAMEINSTANCE(GEngine);
	ArenaManager = GameInstance->ArenaManager;
	WidgetManager = GameInstance->RWidgetManager;
	UserInfo = GameInstance->UserInfo;;
	GuildInfo = GameInstance->GuildInfo;;
	MirrorManager = GameInstance->MirrorDungeonManager;
	RInventory = GameInstance->RInventory;
	TablePtr = UTableManager::GetInstancePtr();

	Blackboard->TargetWidgetArray.Reset();

	if (ArenaManager->isDefenceSetting())
	{
		for (auto DeckButton : variables->m_EnemyHeroDeckBtnList)
			Blackboard->TargetWidgetArray.Emplace(DeckButton);
	}
	else
	{
		for (auto DeckButton : variables->m_MyHeroDeckBtnList)
			Blackboard->TargetWidgetArray.Emplace(DeckButton);
	}

	if (variables->DeckType == EPVPDeckType::PVPMIRROR)
	{
		if (MirrorManager->IsMirrorDefenceSetting())
			DefenceSettingInit();
		else
			MirrorDungeonInit();
	}
	else
	{
		if (ArenaManager->isDefenceSetting() == true)
			DefenceSettingInit();
		else
			ArenaAttackInit();

		if (IsValid(variables->TextBlock_SeasonCount))
			variables->TextBlock_SeasonCount->SetText(FText::AsNumber(RInventory->GetArenaDailyBonusToken()));
	}
	//
	DeckButtonInit(true);
	DeckButtonInit(false);
	//
	SetCompetitorDeck();
	//
	SetMyInfo();
	SetCompetitorInfo();
	SetCompetitorRefreshOverlay();

	if (variables->DeckType == EPVPDeckType::PVPMIRROR)
	{
		if (MirrorManager->IsMirrorDefenceSetting())
		{
		}
		else
		{
		}
	}
	else
	{
		if (ArenaManager->isDefenceSetting())
			SetDefenceDeck();
		else
			SetAttackDeck();
	}
}

void FRenderer_ASyncPVP::Render()
{
	switch (StateController->UserState)
	{
	case EUserBehaviorState::IDLE:
	{
		auto SetDeckButtonIdle = [](TArray<URBaseDeckWidget*> arr) {
			for (int32 i = 0; i < arr.Num(); i++)
			{
				if (arr.IsValidIndex(i))
				{
					arr[i]->SetAvailable(false);
					arr[i]->SetSelecting(false);
				}
			}
		};

		SetDeckButtonIdle(Blackboard->TargetWidgetArray);

		for (int32 i = 0; i < DECK_BUTTON_COUNT; i++)
		{
			if (!Blackboard->SelectedHeroUDs[i].IsEmpty())
				Blackboard->TargetWidgetArray[i]->SetHero(Blackboard->SelectedHeroUDs[i]);
			else
				Cast<UUC_Arena_DeckBtn>(Blackboard->TargetWidgetArray[i])->DelHeroIconInfo();

		}
		break;
	}
	case EUserBehaviorState::SELECTING_START:
	{
		auto FindAvailableDeck = [](TArray<URBaseDeckWidget *> InArray, int32 Targetidx) {
			for (size_t i = 0; i < InArray.Num(); ++i)
			{
				if (InArray.IsValidIndex(i))
				{
					if (i == Targetidx)
						InArray[i]->SetSelecting(true);
				}
			}
		};

		UUserWidget *widget = Blackboard->WaitingList.front();
		URBaseDeckWidget *Waiting_Deck = Cast<URBaseDeckWidget>(widget);
		if (Waiting_Deck)
		{
			int32 iIdx = Blackboard->TargetWidgetArray.Find(Waiting_Deck);
			if (iIdx != INDEX_NONE)
				FindAvailableDeck(Blackboard->TargetWidgetArray, iIdx);
		}
		break;
	}
	case EUserBehaviorState::CHANGING_START:
	{
		if (Blackboard->WaitingList.size() == 1)
		{
			URBaseDeckWidget *heroDeck = Cast<URBaseDeckWidget>(Blackboard->WaitingList.front());
			if (heroDeck)
			{
				int32 iIdx = Blackboard->TargetWidgetArray.Find(heroDeck);
				if (iIdx != INDEX_NONE)
				{
					auto FindAvailableDeck = [=](TArray<URBaseDeckWidget*> InArray) {
						for (size_t i = 0; i < InArray.Num(); ++i)
						{
							if (i == iIdx)
								InArray[i]->SetSelecting(true);
							else
								InArray[i]->SetAvailable(true);
						}
					};

					FindAvailableDeck(Blackboard->TargetWidgetArray);
				}
			}
		}
		break;
	}
	}
	if (GetListNum(Blackboard->SelectedHeroUDs) < 5)
		ReleaseAllRadioSwitcher();

	SetPickCountNPowerUI(Blackboard->SelectedHeroUDs);
}

void FRenderer_ASyncPVP::DefenceSettingInit()
{
	if (IsValid(variables->PlayerInfo))
		variables->PlayerInfo->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(variables->today_number_info))
		variables->today_number_info->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(variables->renewal_btn))
		variables->renewal_btn->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(variables->defence_play_info))
		variables->defence_play_info->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (IsValid(variables->WidgetSwitcher_Start))
		variables->WidgetSwitcher_Start->SetActiveWidgetIndex(0);
}

void FRenderer_ASyncPVP::MirrorDungeonInit()
{
	if (IsValid(variables->PlayerInfo))
		variables->PlayerInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (IsValid(variables->today_number_info))
		variables->today_number_info->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(variables->renewal_btn))
		variables->renewal_btn->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(variables->defence_play_info))
		variables->defence_play_info->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(variables->WidgetSwitcher_Start))
		variables->WidgetSwitcher_Start->SetActiveWidgetIndex(1);

	if (IsValid(variables->ArenaEnterCostOverlay))
		variables->ArenaEnterCostOverlay->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(variables->EnterButton))
	{
// 		variables->EnterButton->OnClicked.RemoveDynamic(this, &UUP_Arena_DeckSetting::OnClickMirrorStart);
// 		variables->EnterButton->OnClicked.AddDynamic(this, &UUP_Arena_DeckSetting::OnClickMirrorStart);
	}
}

void FRenderer_ASyncPVP::ArenaAttackInit()
{
	if (IsValid(variables->PlayerInfo))
		variables->PlayerInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (IsValid(variables->today_number_info))
		variables->today_number_info->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (IsValid(variables->renewal_btn))
		variables->renewal_btn->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (IsValid(variables->defence_play_info))
		variables->defence_play_info->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(variables->WidgetSwitcher_Start))
		variables->WidgetSwitcher_Start->SetActiveWidgetIndex(1);

	if (IsValid(variables->ArenaEnterCostOverlay))
		variables->ArenaEnterCostOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void FRenderer_ASyncPVP::DeckButtonInit(bool isMine)
{
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);

	int32 count = 0;

	if (isMine)
	{
		for (auto widget : variables->m_MyHeroDeckBtnList)
		{
			if (!variables->m_MyHeroDeckBtnList.IsValidIndex(count))
				continue;

			if (ArenaManager->isDefenceSetting())
				widget->Init(Blackboard, count, !isMine);
			else
				widget->Init(Blackboard, count, isMine);

			widget->BP_InitArenaDeckBtn();
			count++;
		}
	}
	else
	{
		for (auto widget : variables->m_EnemyHeroDeckBtnList)
		{
			if (!variables->m_EnemyHeroDeckBtnList.IsValidIndex(count))
				continue;

			if (ArenaManager->isDefenceSetting())
				widget->Init(Blackboard, count, !isMine);
			else
				widget->Init(Blackboard, count, isMine);

			widget->BP_InitArenaDeckBtn();
			count++;
		}
	}
}

void FRenderer_ASyncPVP::SetAttackDeck()
{
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);
	checkf(RInventory != nullptr, TEXT("[%s]RInventory is Empty"), __FUNCTION__);

	if (ArenaManager->isDefenceSetting() == true)
		return;

	FDECKS Decks = RInventory->GetHeroDeck();
	TArray<FString>& UDList = variables->DeckType == EPVPDeckType::PVPMIRROR? Decks.mirror : Decks.arena;

	if (UDList.GetAllocatedSize() <= 0)
		return;

	Blackboard->SelectedHeroUDs = UDList;
}

void FRenderer_ASyncPVP::SetDefenceDeck()
{
	checkf(ArenaManager != nullptr, TEXT("[%s]ArenaManager is Empty"), __FUNCTION__);
	checkf(RInventory != nullptr, TEXT("[%s]RInventory is Empty"), __FUNCTION__);

	if (!ArenaManager->isDefenceSetting())
		return;

	FDECKS Decks = RInventory->GetHeroDeck();
	Blackboard->SelectedHeroUDs = Decks.defense;
}

void FRenderer_ASyncPVP::SetAutoDeckRender(int32 index)
{
	int32 pos_index;

	for (auto &Element : Blackboard->SelectedHeroUDs)
		Element = TEXT("");

	TArray<FString> UDlist;// = UUIFunctionLibrary::GetGenericScrollview()->Variables.CachedItemUDList;
	
	RInventory->GetHeroUDArray(UDlist);
	for (size_t i = 0; i < UDlist.Num();)
	{
		FHERO* hero = RInventory->GetHeroWithUD(UDlist[i]);
		if (hero)
		{
			FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
			if (!tableinfo)
				continue;
			if (!tableinfo->IsHero)
			{
				UDlist.RemoveAt(i);
				continue;
			}
			else if (MirrorManager->FindRemainHP(hero->heroUD) == 0)
			{
				UDlist.RemoveAt(i);
				continue;
			}
		}
		++i;
	}

	for (int32 n = 0; n < 5; ++n)
	{
		if (Blackboard->SelectedHeroUDs.IsValidIndex(n))
		{
			if (index == ARENA_AutoDeck_311)
			{
				if (ArenaManager->isDefenceSetting())
					pos_index = DAUTO_DECK_311_POS_INDEX[n];
				else
					pos_index = AUTO_DECK_311_POS_INDEX[n];
			}

			else if (index == ARENA_AutoDeck_212)
			{
				pos_index = AUTO_DECK_212_POS_INDEX[n];
			}

			else if (index == ARENA_AutoDeck_113)
			{
				if (ArenaManager->isDefenceSetting())
					pos_index = DAUTO_DECK_113_POS_INDEX[n];
				else
					pos_index = AUTO_DECK_113_POS_INDEX[n];
			}
			else
				break;

			auto SortingHelper = [](FString SourceUD, FString DestUD) -> bool {
				int32 SourcePower = UClient_BattleFunctionLibrary::GetHeroCombatStat(SourceUD);
				int32 DestPower = UClient_BattleFunctionLibrary::GetHeroCombatStat(DestUD);

				return SourcePower > DestPower;
			};
					   		
			Algo::Sort(UDlist, SortingHelper);

			int32 iCount = 0;
			for (size_t i = 0; i < UDlist.Num(); ++i)
			{
				if (!StateController->isSameHeroInDeck(UDlist[i], TEXT("")))
				{
					Blackboard->SelectedHeroUDs[pos_index] = UDlist[i];
					++iCount;

					if (iCount == 5)
						break;
				}
			}
		}
	}
	Blackboard->OnBBStateChanged.Broadcast();
}

void FRenderer_ASyncPVP::SetDeckBtnHP(FMIRROR mirror, FHERO_ICON_INFO& info, FEQUIPED_HERO selectHero, int32 index)
{
	if (variables->DeckType == EPVPDeckType::PVPMIRROR)
	{
		info.bIsHp = true;
		info.HpRate = 1.0f;
		info.bIsDead = false;
		int32 nMaxHP = 0;
		UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromEquipedHero(selectHero);
		if (CharacterBattleInfo)
		{
			nMaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
			CharacterBattleInfo = nullptr;
		}

		info.MaxHP = nMaxHP;

		int32 nRemainHP = 0;
		if (mirror.hpList.IsValidIndex(index))
			nRemainHP = mirror.hpList[index];
		else
			nRemainHP = nMaxHP;

		if (nRemainHP == 0)
		{
			info.HpRate = 0.0f;
		}
		else if (0 < nRemainHP)
		{
			info.HpRate = (float)nRemainHP / (float)nMaxHP;
		}

		if (info.HpRate <= 0.0f)
		{
			info.bIsDead = true;
		}
	}
}
