

#include "ProjectR.h"
#include "UP_MirrorDungeonResult.h"

#include "GlobalIntegrated.h"


#include "GameInfo/RInventory.h"

#include "Network/PacketDataStructures.h"

#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/Common/ItemBaseScrollItemData.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/Control/RScrollView.h"
#include "UI/RWidgetManager.h"
#include "UI/Result/UC_ResultEffect.h"
#include "GameState/TowerLobbyState.h"

void UUP_MirrorDungeonResult::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgetsWithCertainName<URenewal_BaseHeroIcon>(this, HeroList, TEXT("UC_Icon_Hero"));

	Button_HeroManager->OnClicked.RemoveDynamic(this, &UUP_MirrorDungeonResult::OnMainLobby);
	Button_HeroManager->OnClicked.AddDynamic(this, &UUP_MirrorDungeonResult::OnMainLobby);
	
	Button_GotoMenu->OnClicked.RemoveDynamic(this, &UUP_MirrorDungeonResult::OnRetry);
	Button_GotoMenu->OnClicked.AddDynamic(this, &UUP_MirrorDungeonResult::OnRetry);

	FindChildWidgetsWithCertainName<UUC_Item_Icons>(this, ItemIcon, TEXT("ItemIcon_0"));

	for (int32 idx = 0; idx < ItemIcon.Num(); idx++)
	{
		if (ItemIcon.IsValidIndex(idx))
			ItemIcon[idx]->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	PlayAni("Display_Ani");
	PlayAni("BG");

	SetResultDisplay();
	SetHeroHPState();
	SetRewards();
	SetTimeText();

// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	UMirrorDungeonManager* MirrorManager = RGameInstance->MirrorDungeonManager;
// 	UIGamePlayMode* GamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();

// 	FMIRROR_STATUS result = MirrorManager->GetMirrorInfoResult();
	// 		for(rMyHeroList)
	// 		result.resetLeftTime;
	// 		result.heroHPs;
	// 		result.mirrors;


}


void UUP_MirrorDungeonResult::OnMainLobby()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_MainLobby);
		RGameInstance->EventManager->ClearEvent();

		UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(RGameInstance->GameStates.Find(EGameStateEnum::VE_Lobby)));
		if (TowerState)
			TowerState->SetIndoorStart(true);

		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}
}

void UUP_MirrorDungeonResult::OnRetry()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_MirrorDungeon);
		RGameInstance->EventManager->ClearEvent();
		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
	}

// 	UUIFunctionLibrary::GoContents(EUIMenuIDEnum::VE_MirrorDungeon);
}

void UUP_MirrorDungeonResult::SetResultDisplay()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UMirrorDungeonManager* MirrorManager = RGameInstance->MirrorDungeonManager;
	bool bWin = MirrorManager->IsMyWin();
	if (bWin)
	{
		ResultDisplay->PlayAni("Victory");
	}
	else
	{
		ResultDisplay->PlayAni("Defeated");
	}
}

void UUP_MirrorDungeonResult::SetHeroHPState()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UMirrorDungeonManager* MirrorManager = RGameInstance->MirrorDungeonManager;

	if (IsValid(MirrorManager))
	{
		TArray<FString>& rMyHeroUDList = MirrorManager->m_MirrorMyHeroUDs;
		TArray<int32> hplist;
		TArray<int32> maxhplist;
		MirrorManager->GetMyHeroHpList(hplist);
		MirrorManager->GetMyHeroMaxHPList(maxhplist);

		for (URenewal_BaseHeroIcon* icon : HeroList)
		{
			if (IsValid(icon))
			{
				icon->SetVisibility(ESlateVisibility::Collapsed);
				icon->SetDummyHero(true);
			}
		}

		int32	Count = 0; 
		for (int32 i = 0; i < rMyHeroUDList.Num(); ++i)
		{
			if (ARENA_MAX_HERO_COUNT <= Count)
				break;


			if(RGameInstance->RInventory->GetHeroWithUD(rMyHeroUDList[i]) == nullptr)
				continue;

			FHERO_ICON_INFO* pIconinfo = nullptr;
			FEQUIPED_HERO DeckHeroData;

			URenewal_BaseHeroIcon* icon = HeroList[Count];
			pIconinfo = icon->GetHeroIconInfoRef();
			pIconinfo->bIsHp = true;
			pIconinfo->bIsDead = false;
			pIconinfo->HpRate = 0.0f;
			if (hplist.IsValidIndex(i) && maxhplist.IsValidIndex(i))
			{
				if(maxhplist[i] != 0)
					pIconinfo->HpRate = (float)hplist[i] / (float)maxhplist[i];
			}
			if (pIconinfo->HpRate <= 0.0f)
				pIconinfo->bIsDead = true;


			if (rMyHeroUDList.IsValidIndex(i))
			{
				icon->SetDummyHero(false);
				icon->SetHeroIconInfoWithUD(rMyHeroUDList[i], EHeroOwnershipType::VE_Me);
				icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Count++;
			}
		}

	}

}

void UUP_MirrorDungeonResult::SetRewards()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UMirrorDungeonManager* MirrorManager = RGameInstance->MirrorDungeonManager;
	if (!MirrorManager->IsMyWin())
	{
		RewardBox->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	TArray<FREWARD> rewardlist;
	MirrorManager->GetGameResultRewards(rewardlist);

	int32 Idx = 4;
	for (FREWARD& reward : rewardlist)
	{
		if (Idx < 0)
			return;
		if (ItemIcon.IsValidIndex(Idx) == false)
			continue;

		FITEM_ICON_INFO itemiconinfo;
		UUIFunctionLibrary::GetRewardIconByRewardType(reward, itemiconinfo);

		ItemIcon[Idx]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemIcon[Idx]->SetItemIconStructure(itemiconinfo);
		Idx--;
	}
}

void UUP_MirrorDungeonResult::SetTimeText()
{
	URGameInstance*		GameInstance = RGAMEINSTANCE(this);

	float	ClearTime = GameInstance->PlayStateInfo->GetClearTime();

	if (IsValid(Text_ResultTime))
	{
		int32 nMinute = ClearTime / 60;
		int32 nSecond = ClearTime - (nMinute * 60);

		FString Minute_Str = FString::FromInt(nMinute * 0.1f);
		Minute_Str += FString::FromInt(nMinute % 10);

		FString Second_Str = FString::FromInt(nSecond * 0.1f);
		Second_Str += FString::FromInt(nSecond % 10);

		Minute_Str += FString(":");
		Minute_Str += Second_Str;

		Text_ResultTime->SetText(FText::FromString(Minute_Str));
	}
}
