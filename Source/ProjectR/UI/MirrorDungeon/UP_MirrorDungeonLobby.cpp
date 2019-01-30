// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_MirrorDungeonLobby.h"

#include "UI/MirrorDungeon/UC_MirrorDeckSlot.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/RWidgetManager.h"


#include "GlobalIntegrated.h"




#include "GameInfo/RInventory.h"

#include "UtilFunctionIntegrated.h"

#include "Network/PacketFunctionLibrary.h"
#include "Network/PacketDataStructures.h"



void UUP_MirrorDungeonLobby::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->MirrorDungeonManager->OnMirrorDungeonStatusRp.RemoveDynamic(this, &UUP_MirrorDungeonLobby::OnMirrorDungeunStatusRp);
	RGameInstance->MirrorDungeonManager->OnMirrorDungeonStatusRp.AddDynamic(this, &UUP_MirrorDungeonLobby::OnMirrorDungeunStatusRp);

	//request mirror dungeon status
	UPacketFunctionLibrary::MIRROR_STATUS_RQ();


	FindChildWidgetsWithCertainName<UUC_MirrorDeckSlot>(this, MirrorSlotList, TEXT("UC_MirrorDeck"));
	FindChildWidgetsWithCertainName<URenewal_BaseHeroIcon>(this, HeroList, TEXT("UC_Icon_Hero"));

	FreeRefreshButton->OnClicked.RemoveDynamic(this, &UUP_MirrorDungeonLobby::OnClickFreeRefreshButton);
	FreeRefreshButton->OnClicked.AddDynamic(this, &UUP_MirrorDungeonLobby::OnClickFreeRefreshButton);
	
	PaidRefreshButton->OnClicked.RemoveDynamic(this, &UUP_MirrorDungeonLobby::OnClickPaidRefreshButton);
	PaidRefreshButton->OnClicked.AddDynamic(this, &UUP_MirrorDungeonLobby::OnClickPaidRefreshButton);
	
	EnterButton->OnClicked.RemoveDynamic(this, &UUP_MirrorDungeonLobby::OnClickEnterButton);
	EnterButton->OnClicked.AddDynamic(this, &UUP_MirrorDungeonLobby::OnClickEnterButton);

	InitializeMirrorLobby();
}

// void UUP_MirrorDungeonLobby::NativeDestruct()
// {
// 
// 	Super::NativeDestruct();
// }

void UUP_MirrorDungeonLobby::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ResetTimeTick(InDeltaTime);

}

void UUP_MirrorDungeonLobby::InvalidateData()
{
	Super::InvalidateData();
}

void UUP_MirrorDungeonLobby::OnClickFreeRefreshButton()
{
// 	WALLET_MIRROR_DEFENDERRESET_FREE
// 	WALLET_MIRROR_ENTRANCE
	//OnMIRROR_RESET_RP
	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->MirrorDungeonManager)
	{
		if (RGameInstance->MirrorDungeonManager->IsProgressMirrorSlot())
		{
			bCheckbeforeRequest = true;
			UUP_CommonPopup* commonPopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000042"))),
				ECommonPopupType::VE_OKCANCEL);

			if (commonPopup)
			{
				commonPopup->OnClickOK.RemoveDynamic(this, &UUP_MirrorDungeonLobby::OnFreeRefreshConfirmOK);
				commonPopup->OnClickOK.AddDynamic(this, &UUP_MirrorDungeonLobby::OnFreeRefreshConfirmOK);
			}
			return;
		}
		else
		{
			bCheckbeforeRequest = false;
			OnFreeRefreshConfirmOK();
		}
	}
}

void UUP_MirrorDungeonLobby::OnClickPaidRefreshButton()
{
	// 	WALLET_MIRROR_DEFENDERRESET_GEM

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->MirrorDungeonManager)
	{
		if (RGameInstance->MirrorDungeonManager->IsProgressMirrorSlot())
		{
			bCheckbeforeRequest = true;
			UUP_CommonPopup* commonPopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000042"))),
				ECommonPopupType::VE_OKCANCEL);

			if (commonPopup)
			{
				commonPopup->OnClickOK.RemoveDynamic(this, &UUP_MirrorDungeonLobby::OnPaidRefreshConfirmOK);
				commonPopup->OnClickOK.AddDynamic(this, &UUP_MirrorDungeonLobby::OnPaidRefreshConfirmOK);
			}
		}
		else
		{
			bCheckbeforeRequest = false;
			OnPaidRefreshConfirmOK();
		}
	}
	
}

void UUP_MirrorDungeonLobby::OnClickEnterButton()
{
	if (!IsSelectSlot())
	{
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000043"))),
			ECommonPopupType::VE_OK);
		return;
	}
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nHaveEnterCount = RGameInstance->RInventory->GetMIRROR_ENTRANCE();

	if (nHaveEnterCount <= 0)
	{
		// TO DO : pop up
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000046"))),
			ECommonPopupType::VE_OK);
		return;
	}

	FMIRROR mirror = RGameInstance->MirrorDungeonManager->GetSelectedMirrorSlotData();

	RGameInstance->MirrorDungeonManager->SetTargetInfo(mirror.user);
	RGameInstance->ArenaManager->bMirrorDeckSetting = true;

	RGameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_MirrorDeckSetting);
}

void UUP_MirrorDungeonLobby::OnMirrorDungeunStatusRp()
{
	InitializeMirrorLobby();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FMIRROR_STATUS& rStatus = RGameInstance->MirrorDungeonManager->MirrorStatus;
// 	rStatus.heroHPs;
// 	rStatus.mirrors;
// 	rStatus.resetLeftTime;

	bResetTimeDecreaseTick = (0 < rStatus.resetLeftTime);
	SetTime(rStatus.resetLeftTime);
	SetButtonText();

	for (int32 i = 0; i < MirrorSlotList.Num(); ++i)
	{
		if(!MirrorSlotList.IsValidIndex(i))
			break;

		UUC_MirrorDeckSlot* slot = MirrorSlotList[i];

		if (rStatus.mirrors.IsValidIndex(i))
		{
			slot->SetDataAndOpenSlot(i, rStatus.mirrors[i]);
		}
		else
		{
			slot->CloseSlot();
		}
	}
}

void UUP_MirrorDungeonLobby::SetButtonText()
{
	SetRefreshButtonState();
	SetEnterButtonState();
}

void UUP_MirrorDungeonLobby::SetRefreshButtonState()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	int32 nFreeRefreshCount = RGameInstance->RInventory->GetMIRROR_DEFENDERRESET_FREE();

	if (0 < nFreeRefreshCount)
	{
		RefreshButtonSwitcher->SetActiveWidgetIndex(eFreeIndex);
		FText LocalText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Mirror_ResetButton01")));
		FText FreeText = FText::Format(FText::FromString(TEXT("{0} {1}/3")), LocalText, FText::AsNumber(nFreeRefreshCount));
		FreeRefreshText->SetText(FreeText);
	}
	else
	{
		RefreshButtonSwitcher->SetActiveWidgetIndex(ePaidIndex);

		auto TableInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("MirrorDefenderResetGem")));
		if (TableInfo == nullptr)
			return;

		int32 nNeedGemCount = 0;
		if (false == TableInfo->GetValue(nNeedGemCount))
			return;

		NeedGemText->SetText(FText::AsNumber(nNeedGemCount));
	}
// 	FreeRefreshText;
// 	PaidRefreshText;
}

void UUP_MirrorDungeonLobby::SetEnterButtonState()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	int32 nRemainEnterCount = RGameInstance->RInventory->GetMIRROR_ENTRANCE();
	FText EnterLocalText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Mirror_EntranceButton")));
	FText EnterText = FText::Format(FText::FromString(TEXT("{0} {1}/6")), EnterLocalText, nRemainEnterCount);
	EnterButtonText->SetText(FText::Format(EnterText, nRemainEnterCount));

// 	EnterButtonText;
}

void UUP_MirrorDungeonLobby::SetTime(int32 InRemainTime)
{
	//int32& nRemainTime = RGameInstance->GuildInfo->RemainTime;
// 	InRemainTime;
	int32 nHour = 0;
	int32 nMin = 0;
	int32 nSec = 0;

	if (InRemainTime > 0)
	{
		nHour = InRemainTime / 3600;
		nMin = (InRemainTime % 3600) / 60;
		nSec = InRemainTime % 60;
	}

	FText HourText = UUIFunctionLibrary::Get_0n_Number(nHour);
	FText MinText = UUIFunctionLibrary::Get_0n_Number(nMin);
	FText SecText = UUIFunctionLibrary::Get_0n_Number(nSec);

 	FText LocalTimeText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000040")));
	FText TimeText = FText::Format(LocalTimeText, HourText, MinText, SecText);

	RemainTimeText->SetText(TimeText);
}

void UUP_MirrorDungeonLobby::InitializeMirrorLobby()
{
	bSelectSlot = false;
	SetTime(0);
	SetDummyHeros();
	SelectedDeckNickNameText->SetText(FText());
	SelectedDeckPowerText->SetText(FText());

	for (int32 i = 0; i < MirrorSlotList.Num(); ++i)
	{
		MirrorSlotList[i]->SetSlotButtonEnable(true);
	}
}

void UUP_MirrorDungeonLobby::SetDummyHeros()
{
	for (URenewal_BaseHeroIcon* hero : HeroList)
	{
		hero->SetDummyHero(true);
	}
}

void UUP_MirrorDungeonLobby::ResetTimeTick(float InDeltaTime)
{
	if (bResetTimeDecreaseTick)
	{
		const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());
		// 		nCurrentSeasonOutTime += InDeltaTime;

		if (CurrentTime.GetTicks() - nCurrentCheckTime >= FTimespan::FromSeconds(1.0f).GetTicks())
		{
			nCurrentCheckTime = CurrentTime.GetTicks();

			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			FMIRROR_STATUS& rStatus = RGameInstance->MirrorDungeonManager->MirrorStatus;
			rStatus.resetLeftTime--;

			SetTime(rStatus.resetLeftTime);

			bool bTimeEnd = rStatus.resetLeftTime <= 0;
			if (bTimeEnd)
			{
				bResetTimeDecreaseTick = false;
				UPacketFunctionLibrary::MIRROR_STATUS_RQ();
			}

		}
	}
}

void UUP_MirrorDungeonLobby::SetMirrorSlotData(int32 InIndex, FMIRROR& InMirror)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->MirrorDungeonManager->SetSelectedMirrorSlotData(InMirror);
	bSelectSlot = true;
	int32 nAttackPower = 0;

	SetDummyHeros();
	//for()
	for (int32 i = 0; i < MirrorSlotList.Num(); ++i)
	{
		MirrorSlotList[i]->SetSlotButtonEnable(i != InIndex);
	}

	int32 IconIndex = 0;
	for (int32 i = 0; i < InMirror.user.decks.defense.Num(); ++i)
//	for(FString ud : InMirror.user.decks.defense)
	{
		FString ud = InMirror.user.decks.defense[i];
		if(ud.IsEmpty())
			continue;

		if (HeroList.IsValidIndex(IconIndex))
		{
			URenewal_BaseHeroIcon* hero = HeroList[IconIndex];
			FEQUIPED_HERO DeckHeroData;
			bool bFind = false;

			for (auto& HeroInfo : InMirror.user.heros)
			{
				if (HeroInfo.heroUD == ud)
				{
					DeckHeroData = HeroInfo;
					bFind = true;
					break;
				}
			}

			if (!bFind)
				continue;

			FHERO_ICON_INFO iconinfo;
			UUIFunctionLibrary::GetHeroIconInfoByEquipedHero(iconinfo, DeckHeroData);


			iconinfo.bIsHp = true;
			iconinfo.bIsDead = false;
			
			iconinfo.HpRate = 1.0f;
			iconinfo.bIsDead = false;

			float fMaxHP = 0;
			UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromEquipedHero(DeckHeroData);
			if (CharacterBattleInfo)
			{
				fMaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
				CharacterBattleInfo = nullptr;
			}

			iconinfo.MaxHP = fMaxHP;

			if (InMirror.hpList.IsValidIndex(i))
			{
				iconinfo.HpRate = (float)InMirror.hpList[i] / fMaxHP;
			}

			if (iconinfo.HpRate <= 0.0f)
			{
				iconinfo.bIsDead = true;
			}

			hero->SetOtherHeroIconInfo(DeckHeroData, iconinfo);
			hero->HidePowerPanel(true);

			if (iconinfo.bIsDead == false)
			{
				int32 Currentpower = UClient_BattleFunctionLibrary::GetEquipedHeroCombatStat(DeckHeroData);
				nAttackPower += Currentpower;
			}
		}

		++IconIndex;
	}

	FText NameText = FText::Format(FText::FromString(TEXT("Lv.{0} ")), InMirror.user.level);
// 	FText LocalText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000047")));
	NameText = FText::FromString(*(NameText.ToString() + InMirror.user.nick));
	SelectedDeckNickNameText->SetText(NameText);
// 	SelectedDeckPowerText->SetText(FText::Format(LocalText, nAttackPower));
	SelectedDeckPowerText->SetText(FText::AsNumber(nAttackPower));
}

void UUP_MirrorDungeonLobby::OnPaidRefreshConfirmOK()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nHaveGemCount = RGameInstance->RInventory->GetGem();

	auto TableInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("MirrorDefenderResetGem")));
	if (TableInfo == nullptr)
		return;

	int32 nNeedGemCount = 0;
	if (false == TableInfo->GetValue(nNeedGemCount))
		return;
	// 	int32 nNeedGemCount = PAID_REFRESH_PRICE;//RGameInstance->RInventory->GetMIRROR_DEFENDERRESET_GEM();

	if (nNeedGemCount <= nHaveGemCount)
	{
		RGameInstance->GetTimerManager().SetTimer(DelayTimer, this, &UUP_MirrorDungeonLobby::CheckEnoughCurrencyDelay, 0.1f);
	}
	else
	{
		RGameInstance->GetTimerManager().SetTimer(DelayTimer, this, &UUP_MirrorDungeonLobby::ShowRejectRequestPopupDelay, 0.1f);
	}
}

void UUP_MirrorDungeonLobby::OnPaidRefreshRequest()
{
	bool bUseGem = true;

	UPacketFunctionLibrary::MIRROR_RESET_RQ(bUseGem);
}

void UUP_MirrorDungeonLobby::OnFreeRefreshConfirmOK()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	int32 nHaveFreeCount = RGameInstance->RInventory->GetMIRROR_DEFENDERRESET_FREE();

	if (0 < nHaveFreeCount)
	{
		if (bCheckbeforeRequest)
		{
			OnFreeRefreshRequest();
			return;
		}

		UUP_CommonPopup* commonPopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000048"))),
			ECommonPopupType::VE_OKCANCEL);

		if (commonPopup)
		{
			commonPopup->OnClickOK.RemoveDynamic(this, &UUP_MirrorDungeonLobby::OnFreeRefreshRequest);
			commonPopup->OnClickOK.AddDynamic(this, &UUP_MirrorDungeonLobby::OnFreeRefreshRequest);
		}
	}
	else
	{
		// TO DO : pop up 
		UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000046"))),
			//FText::FromString(TEXT("Free reset count exhausted")),
			ECommonPopupType::VE_OK);

		UE_LOG(LogUI, Log, TEXT("Check Free Reset Count!!"));
	}
}

void UUP_MirrorDungeonLobby::OnFreeRefreshRequest()
{
	bool bUseGem = false;

	UPacketFunctionLibrary::MIRROR_RESET_RQ(bUseGem);
}

void UUP_MirrorDungeonLobby::CheckEnoughCurrencyDelay()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	auto TableInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("MirrorDefenderResetGem")));
	if (TableInfo == nullptr)
		return;

	int32 nNeedGemCount = 0;
	if (false == TableInfo->GetValue(nNeedGemCount))
		return;

	{
		RGameInstance->GetTimerManager().ClearTimer(DelayTimer);
	}
	UUP_CommonPopup* commonPopup = UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		FText::Format(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, FName(TEXT("Confirm_Message_000041"))), FText::AsNumber(nNeedGemCount)),
		ECommonPopupType::VE_OKCANCEL);

	if (commonPopup)
	{
		commonPopup->OnClickOK.RemoveDynamic(this, &UUP_MirrorDungeonLobby::OnPaidRefreshRequest);
		commonPopup->OnClickOK.AddDynamic(this, &UUP_MirrorDungeonLobby::OnPaidRefreshRequest);
	}
}

void UUP_MirrorDungeonLobby::ShowRejectRequestPopupDelay()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->GetTimerManager().ClearTimer(DelayTimer);
	}

	UUIFunctionLibrary::ShowCommonPopup(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
		UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("USK_up_ItemManage_OptionValueShortage"))),
		ECommonPopupType::VE_OK);
}
