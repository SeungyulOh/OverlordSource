// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_Popup_HeroInfo_Lock.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/HeroManagement/UP_HeroInventory.h"


void UUC_Popup_HeroInfo_Lock::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_Ok))
		Button_Ok->OnClicked.AddDynamic(this, &UUC_Popup_HeroInfo_Lock::OnClickOK);
	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_Popup_HeroInfo_Lock::OnClickCancel);
	if (IsValid(Button_Close))
		Button_Close->OnClicked.AddDynamic(this, &UUC_Popup_HeroInfo_Lock::OnClickCancel);
}

void UUC_Popup_HeroInfo_Lock::NativeDestruct()
{
	if (IsValid(Button_Cancel))
		Button_Cancel->OnClicked.Clear();

	if (IsValid(Button_Ok))
		Button_Ok->OnClicked.Clear();

	if (IsValid(Button_Ok))
		Button_Close->OnClicked.Clear();

	Super::NativeDestruct();
}

void UUC_Popup_HeroInfo_Lock::SetDataList(TArray<FString> lockList, TArray<FString> unlockList, bool IsItem /* = false */)
{
	if (IsItem)
	{
		if (lockList.Num() > 0)
			LockedItems = lockList;
		if (unlockList.Num() > 0)
			UnLockedItems = unlockList;
	}
	else
	{
		if (lockList.Num() > 0)
			LockedHeros = lockList;
		if (unlockList.Num() > 0)
			UnLockedHeros = unlockList;
	}

	if (Text_LockCount)
		Text_LockCount->SetText(FText::AsNumber(lockList.Num()));

	if (Text_UnlockCount)
		Text_UnlockCount->SetText(FText::AsNumber(unlockList.Num()));
}

void UUC_Popup_HeroInfo_Lock::OnClickCancel()
{
	//for heroinventory iteminventory both
	if (!OnItemInventoryUpdated.ExecuteIfBound(EItemInventoryUpdate::NoChange, true))
		this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Popup_HeroInfo_Lock::OnClickOK()
{
	UPacketFunctionLibrary::INVENTORY_ITEM_HERO_LOCK_RQ(LockedItems, UnLockedItems, LockedHeros, UnLockedHeros);
	OnClickCancel();
}