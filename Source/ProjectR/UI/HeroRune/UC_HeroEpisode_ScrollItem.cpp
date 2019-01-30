
#include "ProjectR.h"
#include "UI/HeroRune/UC_HeroEpisode_ScrollItem.h"

#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"

#include "GlobalIntegrated.h"



#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"

#include "UI/Common/UP_ContentsReset.h"
#include "UI/DeckManagement/UC_HeroDeck.h"
#include "UI/RWidgetManager.h"
#include "UI/HeroRune/HeroEpisode_ScrollItemData.h"

#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"


void UUC_HeroEpisode_ScrollItem::SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView)
{
 	UHeroEpisode_ScrollItemData* pData = Cast<UHeroEpisode_ScrollItemData>(item);
	if (pData == nullptr)
		return;

	HeroDeckInfo->SetHero(pData->HeroUD);

	if (pData->bEpisodeExist)
	{
		ReadyStateSwitcher->SetActiveWidgetIndex(0);
		CurrentCount->SetText(FText::AsNumber(pData->nCurrentCount));
		MaxCount->SetText(FText::AsNumber(pData->nMaxCount));
		bNeedCharge = (pData->nCurrentCount <= 0);
	}
	else
	{
		ReadyStateSwitcher->SetActiveWidgetIndex(1);
	}
	
	Super::SetScrollItemData(Index, item, ScrollView);
}

void UUC_HeroEpisode_ScrollItem::OnDeckClick(FString ud)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	FHERO* outData = RGameInstance->RInventory->GetHeroWithUD(ud);
	if (outData==nullptr)
		return;

	FCharacterTableInfo* pInfo = UTableManager::GetInstancePtr()->GetCharacterRow(outData->heroId);
	if (pInfo == nullptr)
		return;

	if(pInfo->HeroEpisode)
	{
		RGameInstance->HeroRuneDungeonManager->SelectHeroID = outData->heroId;
		if (bNeedCharge)
		{
			FString resetKey = FText::Format(FText::FromString("H_{0}"), FText::FromName(outData->heroId)).ToString();
			RGameInstance->HeroRuneDungeonManager->ResetEpisodeID = FName(*resetKey);
			UPacketFunctionLibrary::CONTENT_RESET_STATUS_RQ(TEXT("HeroEpisodePlay"), resetKey);
		}
		else
		{
			UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_HeroRune, 0);
		}
	}
}

void UUC_HeroEpisode_ScrollItem::OnContentsResetButtonOK()
{
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	FContentResetTableInfo* ContentResetTableInfo = UTableManager::GetInstancePtr()->GetContentResetTableRow("HeroEpisodePlay");
// 	if (!ContentResetTableInfo)
// 		return;
// 
// 	bool bAbleReset = false;
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
// 		if (RGameInstance->RInventory->GetGem() >= ContentResetTableInfo->currencyValue)
// 		{
// 			bAbleReset = true;
// 		}
// 	}
//
// 	if (bAbleReset)
// 	{
// 		FString resetKey = FText::Format(FText::FromString(TEXT("H_{0}")), FText::FromString(RGameInstance->HeroRuneDungeonManager->SelectHeroID.ToString())).ToString();
// 		UPacketFunctionLibrary::CONTENT_RESET_RQ(this, TEXT("HeroEpisodePlay"), resetKey);
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