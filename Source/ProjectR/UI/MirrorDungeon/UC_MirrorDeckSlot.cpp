// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_MirrorDeckSlot.h"

#include "UI/Common/UC_Item_Icons.h"
#include "UI/MirrorDungeon/UP_MirrorDungeonLobby.h"

#include "UtilFunctionIntegrated.h"
#include "Global/TableManager.h"


void UUC_MirrorDeckSlot::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgetsWithCertainName<UUC_Item_Icons>(this, RewardIconList, TEXT("Reward0"));

	MatchingInfoButton->OnClicked.RemoveDynamic(this, &UUC_MirrorDeckSlot::OnMatchingInfoButtonClick);
	MatchingInfoButton->OnClicked.AddDynamic(this, &UUC_MirrorDeckSlot::OnMatchingInfoButtonClick);

	InitializeSlot();
}

void UUC_MirrorDeckSlot::InitializeSlot()
{
	CloseSlot();
}

void UUC_MirrorDeckSlot::OnMatchingInfoButtonClick()
{
	if(bSetData)
	{
		UUP_MirrorDungeonLobby* Lobby = Cast<UUP_MirrorDungeonLobby>(ParentWidget);
		if(Lobby)
			Lobby->SetMirrorSlotData(DeckIndex, MirrorData);
	}
}

void UUC_MirrorDeckSlot::SetSlotButtonEnable(bool bEnable)
{
	MatchingInfoButton->SetIsEnabled(bEnable);
}

void UUC_MirrorDeckSlot::SetDataAndOpenSlot(int32 InIndex, FMIRROR& InMirror)
{
	DeckIndex = InIndex;
	bSetData = true;
	MirrorData = InMirror;
	const int32 nActivateIndex = 0;
	SlotActiveSwitcher->SetActiveWidgetIndex(nActivateIndex);
	SetType(InMirror.mirrorType);
	SetReward(InMirror.rewards);
	SetSlotHP(InMirror);
}

void UUC_MirrorDeckSlot::CloseSlot()
{
	bSetData = false;
	const int32 nDeactivateIndex = 1;
	SlotActiveSwitcher->SetActiveWidgetIndex(nDeactivateIndex);
}

void UUC_MirrorDeckSlot::SetType(EMIRROR_TYPE InType)
{
	FText Timetext;
	switch (InType)
	{
	case EMIRROR_TYPE::PAST:
		Timetext = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Mirror_Name01")));
		break;
	case EMIRROR_TYPE::PRESENT:
		Timetext = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Mirror_Name02")));
		break;
	case EMIRROR_TYPE::FUTURE:
		Timetext = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Mirror_Name03")));
		break;
	}

	MirrorSlotTitleText->SetText(Timetext);
}

void UUC_MirrorDeckSlot::SetReward(TArray<FREWARD>& InRewards)
{
	TArray<FITEM_ICON_INFO> iconinfos;
	iconinfos.Empty();
	for (auto& Elem : InRewards)
	{
		FITEM_ICON_INFO RewardItemIconInfo;
		UUIFunctionLibrary::GetRewardIconByRewardType(Elem, RewardItemIconInfo);
		iconinfos.Emplace(RewardItemIconInfo);

	}

	for (int32 i = 0; i < RewardIconList.Num(); ++i)
	{
		if (!RewardIconList.IsValidIndex(i))
			break;

		if (!RewardSizeBoxList.IsValidIndex(i))
			break;

		UUC_Item_Icons* icon = RewardIconList[i];

		if (iconinfos.IsValidIndex(i))
		{
			RewardSizeBoxList[i]->SetVisibility(ESlateVisibility::Visible);
			icon->SetItemIconStructure(iconinfos[i]);
		}
		else
			RewardSizeBoxList[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_MirrorDeckSlot::SetSlotHP(FMIRROR& InMirror)
{
	MirrorHPProgressBar->SetPercent(1.0f);
	MirrorHPText->SetText(FText::AsPercent(1.0f));

	int32 nTotalhp = 0;
	int32 nTotalMaxhp = 0;
	if (InMirror.hpList.Num() == 0)
		return;

	for (int32 hp : InMirror.hpList)
	{
		nTotalhp += hp;
	}


	for (int i = 0; i < InMirror.user.heros.Num(); ++i)
	{
		FEQUIPED_HERO& rHero = InMirror.user.heros[i];
		nTotalMaxhp += GetMaxHP(rHero);
// 		nTotalMaxhp += GetItemHP(rHero);
	}

	float fPercent = (float)nTotalhp / (float)nTotalMaxhp;
	MirrorHPProgressBar->SetPercent(fPercent);
	MirrorHPText->SetText(FText::AsPercent(fPercent));
}

int32 UUC_MirrorDeckSlot::GetMaxHP(FEQUIPED_HERO& InHero)
{
	int32 nMaxHP = 0;
	UCharacterBattleInfo* CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromEquipedHero(InHero);
	if (CharacterBattleInfo)
	{
		nMaxHP = CharacterBattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);
		CharacterBattleInfo = nullptr;
	}
	return nMaxHP;
}

int32 UUC_MirrorDeckSlot::GetItemHP(FEQUIPED_HERO& InHero)
{

	int32 nItemHp = 0;
	for (FITEM& item : InHero.equipedItems)
	{
		if (item.itemId.IsNone() || !item.itemId.IsValid())
			continue;
		TArray<int32> ItemStatOptions;
		ItemStatOptions.AddZeroed((int32)EItemStatOption::Max);
		URenewal_BattleFunctionLibrary::SetCharacterItemBattleInfoFromTable(ItemStatOptions, item);

		nItemHp += ItemStatOptions[(int32)EItemStatOption::MaxHP];
	}

// 	int32 nTotalHp = 0;
// 	for (FITEM& item : InHero.equipedItems)
// 	{
// 		if (item.itemId.IsNone() || !item.itemId.IsValid())
// 			continue;
// 
// 		FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(item.itemId);
// 		checkf(ItemInfo != nullptr, TEXT("[%s]ItemInfo is Empty"), __FUNCTION__);
// 		nTotalHp = ItemInfo->MaxHP;
// 	}

	return nItemHp;
}