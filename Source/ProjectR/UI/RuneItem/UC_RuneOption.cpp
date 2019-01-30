// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GameInfo/RInventory.h"

#include "GlobalIntegrated.h"



#include "Table/LevelAppliedTables/WorkedCharacterTableInfo.h"
#include "Table/LevelAppliedTables/WorkedItemTableInfo.h"

#include "Network/PacketFunctionLibrary.h"

#include "UtilFunctionIntegrated.h"

#include "UC_RuneOption.h"

void UUC_RuneOption::NativeConstruct()
{
	Super::NativeConstruct();
	DelegateAdd();
	RuneSlotList.Add(RuneSlot_01);
	RuneSlotList.Add(RuneSlot_02);
	RuneSlotList.Add(RuneSlot_03);
	RuneSlotList.Add(RuneSlot_04);
}

void UUC_RuneOption::NativeDestruct()
{
	DelegateRemove();

	Super::NativeDestruct();
}

void UUC_RuneOption::SetInValidData(FString InCurrentItemUD)
{
	CurrentItemUD = InCurrentItemUD;
	RuneSlotUpdate();
	RuneSlot_01->Init(0, CurrentItemUD);
	RuneSlot_02->Init(1, CurrentItemUD);
	RuneSlot_03->Init(2, CurrentItemUD);
	RuneSlot_04->Init(3, CurrentItemUD);
}

void UUC_RuneOption::SetSelectRune(int32 InSelectRune)
{
	SelectRuneID = InSelectRune;

	RuneSlot_01->SetEquipSelectRune(SelectRuneID);
	RuneSlot_02->SetEquipSelectRune(SelectRuneID);
	RuneSlot_03->SetEquipSelectRune(SelectRuneID);
	RuneSlot_04->SetEquipSelectRune(SelectRuneID);
}

void UUC_RuneOption::RuneSlotUpdate()
{
	SelectRuneID = 0;
	RuneSlot_01->SetEquipSelectRune(SelectRuneID);
	RuneSlot_02->SetEquipSelectRune(SelectRuneID);
	RuneSlot_03->SetEquipSelectRune(SelectRuneID);
	RuneSlot_04->SetEquipSelectRune(SelectRuneID);
	FITEM* itemInfo = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(CurrentItemUD);

	if (itemInfo == nullptr)
		return;

	if (itemInfo->runes.Num() == 0)
	{

	}
	else
	{
		for (int32 i = 0; i < itemInfo->runes.Num(); i++)
		{
			int32 idx = RGAMEINSTANCE(this)->RInventory->RuneWallet.Find(itemInfo->runes[i]);
			FRuneTable*		RuneTable = UTableManager::GetInstancePtr()->GetRuneOptionRowInt(idx + 1);
			if (idx != INDEX_NONE)
			{
				if (RuneTable)
				{
					UObject* object = UUIFunctionLibrary::GetIconImageFromTable(RuneTable->RuneMountingImage);
					UUIFunctionLibrary::SetImageBrush(object, RuneSlotList[i]->RuneSlotImage);
				}
				if (RuneSlotList[i])
					RuneSlotList[i]->WidgetSwitcher_Button->SetActiveWidgetIndex(1);
			}
			else
			{
				if (RuneSlotList[i])
					RuneSlotList[i]->WidgetSwitcher_Button->SetActiveWidgetIndex(0);
			}
		}
	}
}

void UUC_RuneOption::DelegateAdd()
{
// 	RGAMEINSTANCE(this)->EventManager->OnRuneSlotUpdate.RemoveDynamic(this, &UUC_RuneOption::RuneSlotUpdate);
// 	RGAMEINSTANCE(this)->EventManager->OnRuneSlotUpdate.AddDynamic(this, &UUC_RuneOption::RuneSlotUpdate);
}

void UUC_RuneOption::DelegateRemove()
{
//	RGAMEINSTANCE(this)->EventManager->OnRuneSlotUpdate.RemoveDynamic(this, &UUC_RuneOption::RuneSlotUpdate);
}