// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"



#include "NetWork/PacketFunctionLibrary.h"

#include "GameInfo/RInventory.h"

#include "UI/RWidgetManager.h"
#include "UI/RuneItem/UC_RuneScrollItem.h"

#include "UP_RuneItemEquip.h"

#include "UtilFunctionIntegrated.h"

void UUP_RuneItemEquip::NativeConstruct()
{
	Super::NativeConstruct();
	
	BaseRuneScrollView->Init();

	if (RuneOption)
		RuneOption->SetInValidData(CurrentItemUD);

	RuneItemUpdate();

	DelegateAdd();
}
void UUP_RuneItemEquip::NativeDestruct()
{
	DelegateRemove();

	Super::NativeDestruct();
}

void UUP_RuneItemEquip::SetInValidData()
{
//	RGAMEINSTANCE(this)->EventManager->OnSetItemUD.Broadcast(CurrentItemUD);

	if (RuneItemInfo)
		RuneItemInfo->SetInValidData(CurrentItemUD);

	if (RuneOption)
		RuneOption->SetInValidData(CurrentItemUD);

	TArray<int32>* RuneInventory = RGAMEINSTANCE(this)->RInventory->GetRune();

	if (BaseRuneScrollView)
		BaseRuneScrollView->InventoryDataReComposition(*(RuneInventory));
}

void UUP_RuneItemEquip::RuneItemUpdate()
{
	SetInValidData();

	FITEM* itemInfo = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(CurrentItemUD);

	FRuneWordTable* RuneWord = UTableManager::GetInstancePtr()->GetRuneWordFind(itemInfo->runes);

	if (RuneWord == nullptr)
	{
		if(TextBlock_RuneWord)
			TextBlock_RuneWord->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (TextBlock_RuneWord)
	{
		TextBlock_RuneWord->SetVisibility(ESlateVisibility::Visible);
		//TextBlock_RuneWord->SetText(RuneWord->Runeword_Name);
	}
}

void UUP_RuneItemEquip::RuneItemAllRelease()
{
	TArray<bool> SendRuneRelease;
	SendRuneRelease.Init(false, 4);
	FITEM* itemInfo = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(CurrentItemUD);

	for (int32 i = 0; i < itemInfo->runes.Num(); i++)
	{
		if (itemInfo->runes[i].IsEqual("None") == false)
			SendRuneRelease[i] = true;
	}

	UPacketFunctionLibrary::INVENTORY_RUNE_UNEQUIP_RQ(CurrentItemUD, SendRuneRelease);
}

void UUP_RuneItemEquip::DelegateAdd()
{
	if (BaseRuneScrollView)
	{
		BaseRuneScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_RuneItemEquip::OnClick_RuneIcon);
		BaseRuneScrollView->OnScrollViewSelectedItemData.AddDynamic(this, &UUP_RuneItemEquip::OnClick_RuneIcon);
	}
	if (Button_RuneMix)
	{
		Button_RuneMix->OnClicked.RemoveDynamic(this, &UUP_RuneItemEquip::OnClickedGoRuneMix);
		Button_RuneMix->OnClicked.AddDynamic(this, &UUP_RuneItemEquip::OnClickedGoRuneMix);
	}
// 	RGAMEINSTANCE(this)->EventManager->OnRuneItemUpdate.RemoveDynamic(this, &UUP_RuneItemEquip::RuneItemUpdate);
// 	RGAMEINSTANCE(this)->EventManager->OnRuneItemUpdate.AddDynamic(this, &UUP_RuneItemEquip::RuneItemUpdate);

	if (Button_AllRelease)
	{
		Button_AllRelease->OnClicked.RemoveDynamic(this, &UUP_RuneItemEquip::RuneItemAllRelease);
		Button_AllRelease->OnClicked.AddDynamic(this, &UUP_RuneItemEquip::RuneItemAllRelease);
	}
}

void UUP_RuneItemEquip::DelegateRemove()
{
	if (BaseRuneScrollView)
		BaseRuneScrollView->OnScrollViewSelectedItemData.RemoveDynamic(this, &UUP_RuneItemEquip::OnClick_RuneIcon);

	if (Button_RuneMix)
		Button_RuneMix->OnClicked.RemoveDynamic(this, &UUP_RuneItemEquip::OnClickedGoRuneMix);

//	RGAMEINSTANCE(this)->EventManager->OnRuneItemUpdate.RemoveDynamic(this, &UUP_RuneItemEquip::RuneItemUpdate);

	if (Button_AllRelease)
		Button_AllRelease->OnClicked.RemoveDynamic(this, &UUP_RuneItemEquip::RuneItemAllRelease);
}

void UUP_RuneItemEquip::OnClickedGoRuneMix()
{
	RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_RuneMix);
}

void UUP_RuneItemEquip::OnClick_RuneIcon(URScrollItemData* InSelectedHeroScrollItemData)
{
	if (IsValid(InSelectedHeroScrollItemData) == false)
		return;

	auto HeroScrollItemData = Cast<URuneScrollItemData>(InSelectedHeroScrollItemData);

	if (IsValid(HeroScrollItemData) == false)
		return;
	if (RuneOption)
		RuneOption->SetSelectRune(HeroScrollItemData->RuneID);
}

