// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_RuneItemSlot.h"

#include "GlobalIntegrated.h"


#include "GameInfo/RInventory.h"

#include "Network/PacketFunctionLibrary.h"

void UUC_RuneItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Equip)
	{
		Button_Equip->OnClicked.RemoveDynamic(this, &UUC_RuneItemSlot::OnClickRuneEquip);
		Button_Equip->OnClicked.AddDynamic(this, &UUC_RuneItemSlot::OnClickRuneEquip);
	}
	if (Button_Release)
	{
		Button_Release->OnClicked.RemoveDynamic(this, &UUC_RuneItemSlot::OnClickRuneRelease);
		Button_Release->OnClicked.AddDynamic(this, &UUC_RuneItemSlot::OnClickRuneRelease);
	}
}

void UUC_RuneItemSlot::NativeDestruct()
{
	if (Button_Equip)
		Button_Equip->OnClicked.RemoveDynamic(this, &UUC_RuneItemSlot::OnClickRuneEquip);
	if (Button_Release)
		Button_Release->OnClicked.RemoveDynamic(this, &UUC_RuneItemSlot::OnClickRuneRelease);

	Super::NativeDestruct();
}

void UUC_RuneItemSlot::Init(int32 InIndex, FString InCurrentItemUD)
{
	SlotIndex		=	InIndex;
	CurrentItemUD	=	InCurrentItemUD;
	RuneStatInfo();
}

void UUC_RuneItemSlot::SetEquipSelectRune(int32 InSelectRuneID)
{
	SelectRuneID	=	InSelectRuneID;
}

void UUC_RuneItemSlot::RuneEquip(int32 InSlot)
{
	if (InSlot < 0 || InSlot > 3)
		return;

	if (SelectRuneID == 0)
		return;
	 
	if (CurrentItemUD.IsEmpty() == true)
		return;

	FITEM* itemInfo = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(CurrentItemUD);

	if (itemInfo == nullptr)
		return;

	TArray<FName>		Rune;
	Rune.Init("", 4);

	Rune[InSlot] = RGAMEINSTANCE(this)->RInventory->RuneWallet[SelectRuneID - 1];
	// TODO: retire this class
	//UPacketFunctionLibrary::INVENTORY_RUNE_EQUIP_RQ(CurrentItemUD, Rune);
}

void UUC_RuneItemSlot::RuneRelease(int32 InSlot)
{
	TArray<bool> SendRuneRelease;
	SendRuneRelease.Init(false, 4);
	SendRuneRelease[InSlot] = true;

	FITEM* itemInfo = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(CurrentItemUD);

	if (itemInfo == nullptr)
		return;

	UPacketFunctionLibrary::INVENTORY_RUNE_UNEQUIP_RQ(CurrentItemUD, SendRuneRelease);
}

void UUC_RuneItemSlot::RuneStatInfo()
{
	FITEM*			itemInfo	=	RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(CurrentItemUD);

	if (itemInfo == nullptr)
		return;

	if (itemInfo->runes.Num() <= 0 ||
		itemInfo->runes.IsValidIndex(SlotIndex) == false ||
		itemInfo->runes.Num() < SlotIndex ||
		itemInfo->runes[SlotIndex].IsEqual("None") == true)
	{
		RuneOptionText->SetVisibility(ESlateVisibility::Hidden);
		RuneOption->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemInfo->itemId);

	if (ItemTableInfo == nullptr)
		return;

	int32			idx			=	RGAMEINSTANCE(this)->RInventory->RuneWallet.Find(itemInfo->runes[SlotIndex]);
	FRuneTable*		RuneTable	=	UTableManager::GetInstancePtr()->GetRuneOptionRowInt(idx + 1);

	if (RuneTable == nullptr)
		return;

	RuneOptionText->SetVisibility(ESlateVisibility::Visible);
	RuneOption->SetVisibility(ESlateVisibility::Visible);

	int		StatValue	=	RuneTable->GetAdditionalOptionValue(ItemTableInfo->EquipPosition);
	FText	StatFText	=	RuneTable->GetAdditionalOptionFText(ItemTableInfo->EquipPosition);

	RuneOptionText->SetText(StatFText);
	RuneOption->SetText(FText::AsNumber(StatValue));
}

void UUC_RuneItemSlot::OnClickRuneEquip()
{
	RuneEquip(SlotIndex);
}

void UUC_RuneItemSlot::OnClickRuneRelease()
{
	RuneRelease(SlotIndex);
}