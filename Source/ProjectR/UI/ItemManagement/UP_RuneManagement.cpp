// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_RuneManagement.h"

#include "Table/ItemTableInfo.h"

#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"

#include "GlobalIntegrated.h"
#include "UI/RWidgetManager.h"


#include "GameInfo/RInventory.h"

#include "UC_GenericPopup.h"
#include "UC_HeroItemInfo.h"
#include "UI/Common/UC_ItemIcon.h"
#include "UC_StatChange.h"
#include "UC_EnchantMaterialButton.h"
#include "UP_ItemEnchantResult_NewDesign.h"
#include "UP_GenericSplash.h"
#include "UC_ItemEnchant.h"
#include "SharedConstants/WalletConstants.h"
#include "UC_WalletItemInfo.h"
#include "UC_RewardsSplash.h"
#include "UC_ItemUpgradeResult.h"


void UUP_RuneManagement::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Equip->OnClicked.AddDynamic(this, &UUP_RuneManagement::OnButtonEquipClicked);
	Button_Unequip->OnClicked.AddDynamic(this, &UUP_RuneManagement::OnButtonUnequipClicked);

	for (int32 i = 0; i < RuneSockets.Num(); i++)
	{
		RuneSockets[i]->OnClicked.BindUObject(this, &UUP_RuneManagement::OnRuneSocketClicked, i);
	}

	Runes.Init(NAME_None, FRuneTable::MaxNumSockets);
	StagedRunes.Init(NAME_None, FRuneTable::MaxNumSockets);
}

void UUP_RuneManagement::NativeDestruct()
{
	for (int32 i = 0; i < RuneSockets.Num(); i++)
	{
		if (IsValid(RuneSockets[i]))
			RuneSockets[i]->OnClicked.Unbind();
	}

	if (IsValid(Button_Unequip))
		Button_Unequip->OnClicked.RemoveAll(this);
	if (IsValid(Button_Equip))
		Button_Equip->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUP_RuneManagement::SetItemUD(FString InItemUD)
{
	ItemUD = InItemUD;
}

void UUP_RuneManagement::InvalidateData()
{
	Super::InvalidateData();

	if (!ensure(!ItemUD.IsEmpty()))
		return;

	FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
	if (!ensure(ItemData != nullptr))
		return;
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (!ensure(ItemInfo != nullptr))
		return;

	ItemIcon->RefreshEquipmentIcon(ItemUD);
	Text_ItemName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, ItemInfo->ItemName));

	// invalidate rune sockets

	int32 MaxNumSockets = FRuneTable::MaxNumSockets;
	int32 NumSockets = FRuneTable::EquipPositionToNumSocketsByTable(ItemInfo);//FRuneTable::EquipPositionToNumSockets(ItemInfo->EquipPosition);

	if (!ensure(RuneSockets.Num() >= MaxNumSockets))
		return;

	// Save Runes <- ItemData.runes, padded to FRuneTable::MaxNumSockets
	for (int32 i = 0; i < ItemData->runes.Num(); i++)
	{
		if (!ensure(Runes.IsValidIndex(i)))
			break;

		Runes[i] = ItemData->runes[i];
	}

	InvalidateRuneSocketsAndStats(ItemInfo, ItemData);

	// determine HasRunes

	bool bHasRunes = false;
	for (int32 i = 0; i < Runes.Num(); i++)
	{
		if (!Runes[i].IsNone())
		{
			bHasRunes = true;
			break;
		}
	}
	if (!bHasRunes)
	{
		for (int32 i = 0; i < StagedRunes.Num(); i++)
		{
			if (!StagedRunes[i].IsNone())
			{
				bHasRunes = true;
				break;
			}
		}
	}

	Switcher_Description->SetActiveWidgetIndex(bHasRunes ? 1 : 0);

	switch (State)
	{
	case ERuneManagementState::Normal:
		Switcher_BottomButton->SetActiveWidgetIndex(1);
		break;
	case ERuneManagementState::Equipping:
		if (HasStagedRunes())
		{
			Switcher_BottomButton->SetActiveWidgetIndex(0);
		}
		else
		{
			Switcher_BottomButton->SetActiveWidgetIndex(1);
		}
		break;
	case ERuneManagementState::Unequipping:
	{
		Switcher_BottomButton->SetActiveWidgetIndex(2);
	
		FName Rune = Runes[CurrentSelectedSlot];
		check(!Rune.IsNone());

		int32 RuneGrade = FRuneTable::WalletKeyToRuneGrade(Rune);
		FRuneTable* RuneInfo = FRuneTable::LookUp(RuneGrade);
		check(RuneInfo != nullptr);

		Text_UnequipGemAmount->SetText(FText::AsNumber(RuneInfo->RuneReleaseCashCost));

		int32 OwnedGemAmount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_GEM);
		Button_Unequip->SetIsEnabled(OwnedGemAmount >= RuneInfo->RuneReleaseCashCost);
	}
	break;
	default:
		check(false);
		break;
	}
}

void UUP_RuneManagement::OnInventoryWalletItemSelected(FName SelectedWalletKey)
{
	if (State != ERuneManagementState::Equipping)
		return;

	check(CurrentSelectedSlot < FRuneTable::MaxNumSockets);

	if (StagedRunes[CurrentSelectedSlot] == SelectedWalletKey)
	{
		// toggle off
		StagedRunes[CurrentSelectedSlot] = NAME_None;
	}
	else
	{
		// switch
		StagedRunes[CurrentSelectedSlot] = SelectedWalletKey;
	}
	
	InvalidateData();

	check(IsValid(ItemInventoryPage));

	ItemInventoryPage->InvalidateData();
}

void UUP_RuneManagement::UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData)
{
	Super::UpdateScrollItemData_Material(ScrollItemData);

	if (State == ERuneManagementState::Equipping)
	{
		check(CurrentSelectedSlot < FRuneTable::MaxNumSockets);

		ScrollItemData->IsSelected = ScrollItemData->WalletKey == StagedRunes[CurrentSelectedSlot];
	}
	else
	{
		ScrollItemData->IsSelected = false;
	}
}

void UUP_RuneManagement::OnButtonEquipClicked()
{
	// Equip the StageRunes

	check(State == ERuneManagementState::Equipping);
	check(HasStagedRunes());

	TArray<FString> EquipRunes;
	for (int32 i = 0; i < StagedRunes.Num(); i++)
	{
		if (StagedRunes[i].IsNone())
		{
			EquipRunes.Add(TEXT("")); // TODO: ? or should I send "None" instead of ""? Javascript is confusing.
		}
		else
		{
			EquipRunes.Add(StagedRunes[i].ToString().ToUpper()); // ToUpper for FName to FString conversion safety
		}
	}
	
	RGAMEINSTANCE(this)->HttpClient->OnInventoryRuneEquip.AddUObject(this, &UUP_RuneManagement::OnInventoryRuneEquipRp);

	UPacketFunctionLibrary::INVENTORY_RUNE_EQUIP_RQ(ItemUD, EquipRunes);
}

void UUP_RuneManagement::OnButtonUnequipClicked()
{
	// Unequip rune

	TArray<bool> UnequippedSlots;
	UnequippedSlots.Init(false, CurrentSelectedSlot + 1);
	UnequippedSlots[CurrentSelectedSlot] = true;

	RGAMEINSTANCE(this)->HttpClient->OnInventoryRuneUnequip.AddUObject(this, &UUP_RuneManagement::OnInventoryRuneUnequipRp);

	UPacketFunctionLibrary::INVENTORY_RUNE_UNEQUIP_RQ(ItemUD, UnequippedSlots);
}

void UUP_RuneManagement::OnRuneSocketClicked(int32 Index)
{
	check(Index < FRuneTable::MaxNumSockets);
	
	switch (State)
	{
	case ERuneManagementState::Normal:
	{
		if (!Runes[Index].IsNone())
		{
			// enter unequipping state
			State = ERuneManagementState::Unequipping;
		}
		else
		{
			// enter equipping state
			State = ERuneManagementState::Equipping;
		}
	}
	break;
	case ERuneManagementState::Equipping:
	{
		if (Index == CurrentSelectedSlot)
		{
			State = ERuneManagementState::Normal;
		}
		else
		{
			if (!Runes[Index].IsNone())
			{
				State = ERuneManagementState::Unequipping;
			}
		}
	}
	break;
	case ERuneManagementState::Unequipping:
	{
		if (Index == CurrentSelectedSlot)
		{
			// if the selected rune is clicked again, exit to normal state
			State = ERuneManagementState::Normal;
		}
		else
		{
			if (Runes[Index].IsNone())
			{
				// enter equipping state
				State = ERuneManagementState::Equipping;
			}
		}
	}
	break;
	default:
		check(false);
		break;
	}

	CurrentSelectedSlot = Index;

	InvalidateData();

	check(IsValid(ItemInventoryPage));
	ItemInventoryPage->InvalidateData();
	
}

void UUP_RuneManagement::OnInventoryRuneEquipRp(bool bSuccess)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryRuneEquip.RemoveAll(this);

	if (bSuccess)
	{
		// Reset state
		State = ERuneManagementState::Normal;
		StagedRunes.Init(NAME_None, FRuneTable::MaxNumSockets);
		InvalidateData();

		check(IsValid(ItemInventoryPage));
		ItemInventoryPage->bInventoryUpdated = true;
		ItemInventoryPage->InvalidateData();

		// handle rune word

		FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD);
		if (!ensure(ItemData != nullptr))
			return;
		FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
		if (!ensure(ItemInfo != nullptr))
			return;
		FName RuneWordRowName = FRuneWordTable::CheckForRuneWord(ItemData->runes, FRuneTable::EquipPositionToNumSocketsByTable(ItemInfo)/*FRuneTable::EquipPositionToNumSockets(ItemInfo->EquipPosition)*/);
		if (!RuneWordRowName.IsNone())
		{
			// show splash
			auto Splash = UUIFunctionLibrary::ShowGenericSplashContent(EWidgetBluePrintEnum::WBP_Splash_RuneEquipResult);
			check(IsValid(Splash));
			auto SplashContent = Cast<UUC_RuneEquipResult>(Splash->GetContentWidget());
			check(IsValid(SplashContent));

			SplashContent->Refresh(RuneWordRowName);
		}
	}
	else
	{
		// TODO
		ensure(false);
	}
}

void UUP_RuneManagement::OnInventoryRuneUnequipRp(bool bSuccess)
{
	RGAMEINSTANCE(this)->HttpClient->OnInventoryRuneUnequip.RemoveAll(this);

	if (bSuccess)
	{
		// Reset state
		State = ERuneManagementState::Normal;
		// (design) reset StagedRunes after unequip
		StagedRunes.Init(NAME_None, FRuneTable::MaxNumSockets);

		InvalidateData();

		check(IsValid(ItemInventoryPage));
		ItemInventoryPage->bInventoryUpdated = true;
		ItemInventoryPage->InvalidateData();
	}
	else
	{
		// TODO
		ensure(false);
	}
}

void UUP_RuneManagement::InvalidateRuneSocketsAndStats(const FItemTableInfo* ItemInfo, const FITEM* ItemData)
{
	// Refresh RuneSockets and StatDiffs in tandem

	check(RuneSockets.Num() >= FRuneTable::MaxNumSockets);
	check(StatDiffs.Num() >= FRuneTable::MaxNumSockets);
	check(Runes.Num() >= FRuneTable::MaxNumSockets);
	check(StagedRunes.Num() >= FRuneTable::MaxNumSockets);

	int32 NumRuneSockets = FRuneTable::EquipPositionToNumSocketsByTable(ItemInfo);//FRuneTable::EquipPositionToNumSockets(ItemInfo->EquipPosition);

	for (int32 i = 0; i < FRuneTable::MaxNumSockets; i++)
	{
		RuneSockets[i]->IsSelected = State != ERuneManagementState::Normal &&  i == CurrentSelectedSlot;

		RuneSockets[i]->IsDisabled = true; // to be overwritten
		RuneSockets[i]->IsEmpty = true; // to be overwritten
		StatDiffs[i]->SetVisibility(ESlateVisibility::Hidden); // to be overwritten. Hidden because (design) wants the effect of that.

		if (i >= NumRuneSockets)
			continue;

		RuneSockets[i]->IsDisabled = false;

		FName RuneWalletKey = NAME_None;
		bool IsStaged = false;
		if (!Runes[i].IsNone())
		{
			RuneWalletKey = Runes[i];
			IsStaged = false;
		}
		else if (!StagedRunes[i].IsNone())
		{
			RuneWalletKey = StagedRunes[i];
			IsStaged = true;
		}
		else
		{
			RuneWalletKey = NAME_None;
		}

		if (RuneWalletKey.IsNone())
			continue;

		RuneSockets[i]->IsEmpty = false;
		RuneSockets[i]->IsStaged = IsStaged;
		StatDiffs[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FRuneTable* RuneInfo = UTableManager::GetInstancePtr()->GetRuneOptionRow(RuneWalletKey);
		if (!ensure(RuneInfo != nullptr))
			return;

		RuneSockets[i]->IconKey = RuneInfo->RuneInventoryImage;

		// filthy code ahead

		TArray<int32> ItemOptions;
		ItemOptions.AddZeroed((int32)EItemStatOption::Max);

		FITEM ItemDataForOneRune = *ItemData;
		ItemDataForOneRune.runes.Init(NAME_None, FRuneTable::MaxNumSockets);
		for (int32 j = 0; j < ItemDataForOneRune.runes.Num(); j++)
		{
			if (i == j)
			{
				ItemDataForOneRune.runes[j] = RuneWalletKey;
			}
			else
			{
				ItemDataForOneRune.runes[j] = NAME_None;
			}
		}

		FRuneTable::ApplyRunes(&ItemDataForOneRune, ItemOptions);

		for (int32 j = 0; j < ItemOptions.Num(); j++)
		{
			if (ItemOptions[j] != 0)
			{
				StatDiffs[i]->RefreshWithStaged((EItemStatOption)j, ItemOptions[j], IsStaged);
				break;
			}
		}
	}

	// Not invalidated from above
	for (int32 i = 0; i < FRuneTable::MaxNumSockets; i++)
	{
		RuneSockets[i]->InvalidateData();
	}
}

void UUP_RuneManagement::InvalidateStatChanges(const FITEM* ItemData)
{
	// with state variable Runes

	// provisional implementation.
	// TODO: spec

	for (UUC_StatDiff* StatDiff : StatDiffs)
	{
		StatDiff->SetVisibility(ESlateVisibility::Hidden);
	}

	int32 WidgetIndex = 0;
	for (int32 i = 0; i < Runes.Num(); i++)
	{
		if (i >= StatDiffs.Num())
			break;

		if (Runes[i].IsNone())
			continue;

		// filthy code ahead

		StatDiffs[WidgetIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		TArray<int32> ItemOptions;
		ItemOptions.AddZeroed((int32)EItemStatOption::Max);

		FITEM ItemDataForOneRune = *ItemData;
		for (int32 j = 0; j < ItemDataForOneRune.runes.Num(); j++)
		{
			if (i == j)
			{
				ItemDataForOneRune.runes[j] = Runes[i];
			}
			else
			{
				ItemDataForOneRune.runes[j] = NAME_None;
			}
		}

		FRuneTable::ApplyRunes(&ItemDataForOneRune, ItemOptions);

		for (int32 j = 0; j < ItemOptions.Num(); j++)
		{
			if (ItemOptions[j] != 0)
			{
				StatDiffs[WidgetIndex]->Refresh((EItemStatOption)j, ItemOptions[j]);
				break;
			}
		}

		WidgetIndex++;
	}
}

bool UUP_RuneManagement::HasStagedRunes() const
{
	for (int32 i = 0; i < StagedRunes.Num(); i++)
	{
		if (!StagedRunes[i].IsNone())
		{
			return true;
		}
	}
	return false;
}

void UUC_RuneSocket::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Socket->OnClicked.AddDynamic(this, &UUC_RuneSocket::OnButtonSocketClicked);
}

void UUC_RuneSocket::NativeDestruct()
{
	if (IsValid(Button_Socket))
		Button_Socket->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUC_RuneSocket::InvalidateData()
{
	Super::InvalidateData();

	if (IsDisabled)
	{
		Panel_Disabled->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		Panel_Item->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Panel_Disabled->SetVisibility(ESlateVisibility::Collapsed);

		if (IsEmpty)
		{
			Panel_Item->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			Panel_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ItemIconCommon->RefreshIcon(IconKey);

			ItemIconCommon->SetRenderOpacity(IsStaged ? IconRenderOpacityWhenStaged : 1.0f);
		}
	}


	Panel_Selected->SetVisibility(IsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UUC_RuneSocket::OnButtonSocketClicked()
{
	if (IsDisabled)
		return;

	OnClicked.ExecuteIfBound();
}

void UUC_RuneEquipResult::Refresh(FName RuneWordRowName)
{
	FRuneWordTable* RuneWordInfo = UTableManager::GetInstancePtr()->GetRuneWordRow(RuneWordRowName);
	check(RuneWordInfo != nullptr);

	int32 RowNumSockets = RuneWordInfo->GetRowNumSockets();
	TArray<FName> RowRunes = RuneWordInfo->GetRowRunes();

	FRuneWordSkillTable* RuneWordSkillInfo = UTableManager::GetInstancePtr()->GetRuneWordSkillRow(RuneWordInfo->RunewordSkillID);
	check(RuneWordSkillInfo != nullptr);
	
	if (!ensure(RuneIcons.Num() >= FRuneTable::MaxNumSockets))
		return;

	for (int32 i = 0; i < FRuneTable::MaxNumSockets; i++)
	{
		if (i >= RowNumSockets)
		{
			RuneIcons[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			FRuneTable* RuneInfo = UTableManager::GetInstancePtr()->GetRuneOptionRow(RowRunes[i]);
			check(RuneInfo != nullptr);

			RuneIcons[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			RuneIcons[i]->ItemIcon->RefreshIcon(RuneInfo->RuneInventoryImage);
		}
	}

	Text_RuneWordName->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Item, RuneWordInfo->Runeword_Name));

	//FText RuneWordSkillDesc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, RuneWordSkillInfo->Summary);
	//Text_RuneWordDescription->SetText(RuneWordSkillDesc);
}

void UUC_RuneManagement_StatDiff::NativeConstruct()
{
	Super::NativeConstruct();

	// save
	Color_NormalStatName = Text_Name->ColorAndOpacity;
	Color_NormalStatValue = Text_Value->ColorAndOpacity;
}

void UUC_RuneManagement_StatDiff::RefreshWithStaged(EItemStatOption ItemStatOption, int32 DiffValue, bool IsStaged)
{
	Super::Refresh(ItemStatOption, DiffValue);

	Text_Name->SetColorAndOpacity(IsStaged ? Color_StagedRuneStatName : Color_NormalStatName);
	Text_Value->SetColorAndOpacity(IsStaged ? Color_StagedRuneStatValue : Color_NormalStatValue);
}
