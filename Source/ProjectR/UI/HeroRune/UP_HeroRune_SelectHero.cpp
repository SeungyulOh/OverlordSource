

#include "ProjectR.h"
#include "UP_HeroRune_SelectHero.h"

#include "UtilFunctionIntegrated.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"
#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"

#include "GlobalIntegrated.h"



//

#include "UI/RWidgetManager.h"
#include "UI/Control/RScrollView.h"
#include "UI/Common/UP_ContentsReset.h"
#include "UI/HeroRune/UC_HeroEpisode_ScrollItem.h"
#include "UI/HeroRune/HeroEpisode_ScrollItemData.h"

void UUP_HeroRune_SelectHero::NativeConstruct()
{
	Super::NativeConstruct();
// 	URGameInstance*	RGameInstance = RGAMEINSTANCE(this);
// 
// 	if( IsValid(RGameInstance->EventManager))
// 	{
// 		RGameInstance->EventManager->OnReceiveHeroRuneInfo.RemoveDynamic(this, &UUP_HeroRune_SelectHero::Init);
// 		RGameInstance->EventManager->OnReceiveHeroRuneInfo.AddDynamic(this, &UUP_HeroRune_SelectHero::Init);
// 	}
// 
// 	if (IsValid(ChallengeButton))
// 	{
// 		ChallengeButton->OnClicked.RemoveDynamic(this, &UUP_HeroRune_SelectHero::OnClickChallengeButton);
// 		ChallengeButton->OnClicked.AddDynamic(this, &UUP_HeroRune_SelectHero::OnClickChallengeButton);
// 	}
// 	if (IsValid(ChargeButton))
// 	{
// 		ChargeButton->OnClicked.RemoveDynamic(this, &UUP_HeroRune_SelectHero::OnClickChargeButton);
// 		ChargeButton->OnClicked.AddDynamic(this, &UUP_HeroRune_SelectHero::OnClickChargeButton);
// 	}
// 
// 	if (IsValid(HeroSelectScrollView))
// 	{
// 		HeroSelectScrollView->OnScrollViewSelectedItem.RemoveDynamic(this, &UUP_HeroRune_SelectHero::OnSelectItem);
// 		HeroSelectScrollView->OnScrollViewSelectedItem.AddDynamic(this, &UUP_HeroRune_SelectHero::OnSelectItem);
// 
// 		HeroSelectScrollView->Init();
// 		HeroSelectScrollView->ClearScrollItemData();
// 		HeroSelectScrollView->InvalidateData();
// 	}
// 	
// 	if (IsValid(RGameInstance->HttpClient))
// 	{
// 		RGameInstance->HttpClient->OnContentResetRpDelegate.RemoveDynamic(this, &UUP_HeroRune_SelectHero::CountResetRp);
// 		RGameInstance->HttpClient->OnContentResetRpDelegate.AddDynamic(this, &UUP_HeroRune_SelectHero::CountResetRp);
// 		RGameInstance->HttpClient->OnContentResetStatusRpDelegate.RemoveDynamic(this, &UUP_HeroRune_SelectHero::CountResetStatusRp);
// 		RGameInstance->HttpClient->OnContentResetStatusRpDelegate.AddDynamic(this, &UUP_HeroRune_SelectHero::CountResetStatusRp);
// 	}
// 	if (IsValid(MyPlaySwitcher))
// 		MyPlaySwitcher->SetActiveWidgetIndex(1);
// 
// 
// 	UPacketFunctionLibrary::HERO_EPISODE_ROBBY_RQ(this);
}

void UUP_HeroRune_SelectHero::NativeDestruct()
{
	URGameInstance*	RGameInstance = RGAMEINSTANCE(this);

// 	if ( IsValid(RGameInstance->EventManager))
// 	{
// 		RGameInstance->EventManager->OnReceiveHeroRuneInfo.RemoveDynamic(this, &UUP_HeroRune_SelectHero::Init);
// 	}

	if (IsValid(ChallengeButton))
	{
		ChallengeButton->OnClicked.RemoveDynamic(this, &UUP_HeroRune_SelectHero::OnClickChallengeButton);
	}

	if (IsValid(ChargeButton))
	{
		ChargeButton->OnClicked.RemoveDynamic(this, &UUP_HeroRune_SelectHero::OnClickChargeButton);
	}

	if (IsValid(HeroSelectScrollView))
	{
		HeroSelectScrollView->OnScrollViewSelectedItem.RemoveDynamic(this, &UUP_HeroRune_SelectHero::OnSelectItem);
	}

	if (IsValid(RGameInstance->HttpClient))
	{
		RGameInstance->HttpClient->OnContentResetRpDelegate.RemoveDynamic(this, &UUP_HeroRune_SelectHero::CountResetRp);
		RGameInstance->HttpClient->OnContentResetStatusRpDelegate.RemoveDynamic(this, &UUP_HeroRune_SelectHero::CountResetStatusRp);
	}

	Super::NativeDestruct();
}

void UUP_HeroRune_SelectHero::Init()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (RGameInstance->HeroRuneDungeonManager->IsExistRecentRecord())
	{
		if (IsValid(MyPlaySwitcher))
			MyPlaySwitcher->SetActiveWidgetIndex(0);
		FEPISODE& recent = RGameInstance->HeroRuneDungeonManager->RecentlyEpisode;

		if (IsValid(ChallengeButtonSwitcher))
			ChallengeButtonSwitcher->SetActiveWidgetIndex((recent.trial == recent.max) ? 1 : 0);
		if (IsValid(RemainCount))
			RemainCount->SetText(FText::AsNumber(recent.max - recent.trial));
		if (IsValid(MaxCount))
			MaxCount->SetText(FText::AsNumber(recent.max));

		FName heroID = GetHeroIDbyEpisodeID(recent.episodeId);
		

		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(heroID);
		if (CharacterTableInfo)
		{
			//AvatarImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(CharacterTableInfo->PortraitTextureKey), AvatarImage);

			HeroNick->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->Nick));
			HeroName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, CharacterTableInfo->DisplayName));
		}

// 		FHERO* OutHeroData = RGameInstance->HeroInventory->GetInventoryDataWithID(heroID);
// 		if (OutHeroData != nullptr)
// 			RuneCount->SetText(FText::AsNumber(OutHeroData->rune));
	}
	else
	{
		if (IsValid(MyPlaySwitcher))
			MyPlaySwitcher->SetActiveWidgetIndex(1);
	}

	//IsCheckEvent = (RGameInstance->RWidgetManager->UIEventQueue.Num() > 0);

	InvalidateScrollData();

	/*if (IsCheckEvent)
	{		
		FUIEventInfo eventInfo = RGameInstance->RWidgetManager->UIEventQueue[0];
		if (eventInfo.EventType == EUIEventEnum::VE_GoContent
			&& eventInfo.IValue == (int32)EUIMenuIDEnum::VE_HeroRune)
		{
			auto TargetItem = Cast<UUC_HeroEpisode_ScrollItem>(HeroSelectScrollView->GetScrollItem(EventItemIndex));
			TargetItem->OnDeckClick(RGameInstance->HeroRuneDungeonManager->SelectHeroID);
			//UUIFunctionLibrary::GoContents(RGameInstance, (EUIMenuIDEnum)eventInfo.IValue);
			GameInstance->RWidgetManager->UIEventQueue.RemoveAt(0);
		}
	}*/
	
// 	if ( IsValid(RGameInstance->ForceTutorialManager))
// 	{
// 		RGameInstance->ForceTutorialManager->OnStartUnlockTutorial();
// 	}
}

void UUP_HeroRune_SelectHero::OnClickChallengeButton()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
	if (RGameInstance->HeroRuneDungeonManager->IsExistRecentRecord() == false)
		return;

	FName heroID = GetHeroIDbyEpisodeID(RGameInstance->HeroRuneDungeonManager->RecentlyEpisode.episodeId);
	RGameInstance->HeroRuneDungeonManager->SelectHeroID = heroID;

	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_HeroRune, 0);
}

void UUP_HeroRune_SelectHero::OnClickChargeButton()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
	if (RGameInstance->HeroRuneDungeonManager->IsExistRecentRecord() == false)
		return;
	
	RGameInstance->HeroRuneDungeonManager->ResetEpisodeID = RGameInstance->HeroRuneDungeonManager->RecentlyEpisode.episodeId;

	UPacketFunctionLibrary::CONTENT_RESET_STATUS_RQ(TEXT("HeroEpisodePlay"), RGameInstance->HeroRuneDungeonManager->RecentlyEpisode.episodeId.ToString());
}

void UUP_HeroRune_SelectHero::InvalidateScrollData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
	if (IsValid(HeroSelectScrollView))
	{
		HeroSelectScrollView->ClearScrollItemData();
	}
	
	//EventItemIndex = 0;
	TArray<FEPISODE>& EpisodeList = RGameInstance->HeroRuneDungeonManager->HeroEpisodes;
	for (FEPISODE& episode : EpisodeList)
	{
		FName heroID = GetHeroIDbyEpisodeID(episode.episodeId);

		UHeroEpisode_ScrollItemData* pData = Cast<UHeroEpisode_ScrollItemData>(HeroSelectScrollView->AddScrollItemData(UHeroEpisode_ScrollItemData::StaticClass()));

		
		FHERO* OutHeroData = RGameInstance->RInventory->GetHeroWithID(heroID);
		if (OutHeroData == nullptr)
			continue;

		/*if (IsCheckEvent)
		{
			if (RGameInstance->HeroRuneDungeonManager->SelectHeroID == heroID)
			{
				break;
			}
			++EventItemIndex;
		}*/
		
		FCharacterTableInfo* pInfo = UTableManager::GetInstancePtr()->GetCharacterRow(heroID);
		if (pInfo == nullptr)
			continue;

		pData->bEpisodeExist = pInfo->HeroEpisode;
		pData->HeroUD = OutHeroData->heroUD;
		pData->nCurrentCount = episode.max - episode.trial;
		pData->nMaxCount = episode.max;
	}

	HeroSelectScrollView->InvalidateData();
	HeroSelectScrollView->ComputeSpawnAni(0);
}

FName UUP_HeroRune_SelectHero::GetHeroIDbyEpisodeID(FName& episodeId)
{
	FString strID = episodeId.ToString();
	FString strFID;
	for (int32 i = 0; i < strID.Len(); ++i)
	{
		if (strID[i] >= TEXT('0') && strID[i] <= TEXT('9'))
		{
			strFID += strID[i];
		}
	}

	int32 iId = FCString::Atoi(*strFID);
	return FName(*FString::FromInt(iId));
}

void UUP_HeroRune_SelectHero::OnSelectItem(URScrollItem* selectItem)
{
	if (IsValid(selectItem))
	{
		UUC_HeroEpisode_ScrollItem* pSelectedItem = static_cast<UUC_HeroEpisode_ScrollItem*>(selectItem);
		if (IsValid(pSelectedItem))
		{
			pSelectedItem->IsSelected = false;
			UHeroEpisode_ScrollItemData* SelectedItemData = static_cast<UHeroEpisode_ScrollItemData*>(selectItem->ItemData);
			if (SelectedItemData)
			{
				pSelectedItem->OnDeckClick(SelectedItemData->HeroUD);
			}
		}
	}
}

void UUP_HeroRune_SelectHero::CountResetRp(bool bResult)
{
	if (!bResult)
		return;

	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	FContentResetTableInfo* ContentResetTableInfo = UTableManager::GetInstancePtr()->GetContentResetTableRow("HeroEpisodePlay");

	if (ContentResetTableInfo)
	{
		FEPISODE* outEpisode = RGameInstance->HeroRuneDungeonManager->FindEpisodebyID(RGameInstance->HeroRuneDungeonManager->ResetEpisodeID);
		if (outEpisode != nullptr)
		{
			outEpisode->trial -= ContentResetTableInfo->addCount;
		}

		if(RGameInstance->HeroRuneDungeonManager->ResetEpisodeID == RGameInstance->HeroRuneDungeonManager->RecentlyEpisode.episodeId)
		{
			RGameInstance->HeroRuneDungeonManager->RecentlyEpisode.trial -= ContentResetTableInfo->addCount;
		}

		Init();
	}
}

void UUP_HeroRune_SelectHero::CountResetStatusRp(int32 nResetCount)
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

}

void UUP_HeroRune_SelectHero::OnContentsResetButtonOK()
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	FContentResetTableInfo* ContentResetTableInfo = UTableManager::GetInstancePtr()->GetContentResetTableRow("HeroEpisodePlay");
	if (!ContentResetTableInfo)
		return;

	bool bAbleReset = false;
// 	EREWARD_TYPE eType = EREWARD_TYPE::REDSTONE;
// 
// 	if (ContentResetTableInfo->currencyType == "REDSTONE")
// 	{
// 		eType = EREWARD_TYPE::REDSTONE;
// 		if (RGameInstance->UserInfo->Avatar.redstone >= ContentResetTableInfo->currencyValue)
// 		{
// 			bAbleReset = true;
// 		}
// 	}
// 	else if (ContentResetTableInfo->currencyType == "GEMS")
// 	{
// 		eType = EREWARD_TYPE::GEM;
// 		if (RGameInstance->RInventory->GetGem()>= ContentResetTableInfo->currencyValue)
// 		{
// 			bAbleReset = true;
// 		}
// 	}

// 	if (bAbleReset)
// 	{
// 		UPacketFunctionLibrary::CONTENT_RESET_RQ(this, TEXT("HeroEpisodePlay"), RGameInstance->HeroRuneDungeonManager->ResetEpisodeID.ToString());
// 	}
// 	else
// 	{
// 		FName currencyType;
// 		switch (eType)
// 		{
// 		case EREWARD_TYPE::GEM:
// 			currencyType = TEXT("UI_Currency_Warning_Gem");
// 			break;
// 		case EREWARD_TYPE::SP1:
// 		case EREWARD_TYPE::REDSTONE:
// 			currencyType = TEXT("UI_Currency_Warning_SP1");
// 			break;
// 		default:
// 			break;
// 		}
// 
// 		UUIFunctionLibrary::ShowCommonPopup(this,
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
// 			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, currencyType),
// 			ECommonPopupType::VE_OK);
// 	}

}
