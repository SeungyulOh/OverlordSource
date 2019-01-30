// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RInventory.h"

#include "GlobalIntegrated.h"



#include "CustomStructures/GameEnvironmentVar.h"
#include "GameInfo/PlayStateInfo.h"
#include "NotifyList/NotifyEventList.h"
#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "UI/HeroInfo/HeroManager/UP_HeroManager.h"
#include "Table/CharacterTableInfo.h"
#include "Network/PacketDataEnum.h"
#include "SharedConstants/SharedGlobalConstants.h"
#include "UP_GenericScrollview.h"

#define REFILL_CHECK_PERIOD		10.0f
void URInventory::Clear()
{
	NewHeroArray.Empty();
	GOLD = 0;
	GEM = 0;
	STAMINA = 0;
	STAMINA_PVP = 0;
	HERO_RUNE = 0;
	PVP_TOKEN = 0;
	ARENA_DAILYBONUSTOKEN = 0;
	ARENA_DEFENDERRESETTOKEN = 0;
	MIRROR_DEFENDERRESET_FREE = 0;
	MIRROR_ENTRANCE = 0;
	SUMMONTICKET01 = 0;
	SUMMONTICKET02 = 0;
	MILEAGE = 0;
	FLOORSTORE_TICKET = 0;
	FLOORSTORE_RESETTOKEN = 0;
	FLOORSTORE_RESETTOKEN_GEM = 0;
	MISSION_ADD_SCROLL = 0;
	HEROUPGRADEMATERIAL = 0;
	S_TOWERBOSS_F1 = 0;

	UE_LOG(LogRInventory, Log, TEXT("URInventory::Clear"));
}

void URInventory::StartRefillCheck()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

// 	if(RefillList.Num() == 0)
	RefillList.Empty();
	ResetTimeList.Empty();
	{
		UTableManager* TM = UTableManager::GetInstancePtr();
		if (TM)
		{
			TArray<FName> WalletNames = TM->GetWalletRowNames();

			for (FName& name : WalletNames)
			{
				FWalletTableInfo* pWallet = TM->GetWalletRow(name);

				if (pWallet)
				{
					if (0 < pWallet->RefillPeriod)
					{
						if (!RefillList.Contains(pWallet->RefillPeriod))
						{
							RefillList.Emplace(pWallet->RefillPeriod);
							ResetTimeList.Emplace(pWallet->RefillPeriod, (int32)RGameInstance->UTC_DiffTimespan.GetTotalSeconds());
						}

						RefillList[pWallet->RefillPeriod].Emplace(name);
					}
				}
			}
		}
	}

	RGameInstance->TimerManager->ClearTimer(RefillHandler);
	RGameInstance->TimerManager->SetTimer(RefillHandler, this, &URInventory::CheckWalletRefill, REFILL_CHECK_PERIOD, true);

}

void URInventory::InitInventory(FINVENTORY& inventory)
{
	InitCurrency(inventory);
	SetHeroData(inventory.heros);
	SetItemData(inventory.items);
	SetDecks(inventory.decks);

	/*Inventory Size Update*/
	if (inventory.size.IsValidIndex(0))
		INVENTORY_ITEM_CURRENTCOUNT = inventory.size[0];
	if (inventory.size.IsValidIndex(1))
		INVENTORY_ITEM_MAXCOUNT = inventory.size[1];
	if (inventory.size.IsValidIndex(2))
		INVENTORY_HERO_CURRENTCOUNT = inventory.size[2];
	if (inventory.size.IsValidIndex(3))
		INVENTORY_HERO_MAXCOUNT = inventory.size[3];
	/*Inventory Size Update End*/
	UE_LOG(LogRInventory, Log, TEXT("URInventory::InitInventory"));
}

void URInventory::SetDecks(FDECKS InDecks)
{
	decks = InDecks;
}

void URInventory::CheckWalletRefill()
{
	UTableManager* TM = UTableManager::GetInstancePtr();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	RGameInstance->UTC_DiffTimespan += FTimespan::FromSeconds(REFILL_CHECK_PERIOD);

// 	UE_LOG(LogUI, Log, TEXT("second %d"), (int32)RGameInstance->UTC_DiffTimespan.GetTotalSeconds());

	TArray<int32> Keys;
	RefillList.GetKeys(Keys);

	int32 serverTime = (int32)RGameInstance->UTC_DiffTimespan.GetTotalSeconds();

	for (int32 period : Keys)
	{
		if(!ResetTimeList.Contains(period))
			continue;

		int32& walletUpdate = ResetTimeList[period];
		int32 diff = serverTime - walletUpdate;

		if (diff >= (int32)period)
		{
			for(FName name : RefillList[period])
			{
				FWalletTableInfo* pWallet = TM->GetWalletRow(name);
				TArray<int32>* currentPtr = nullptr;
				switch (pWallet->ArrGroup)
				{
				case 1:
					currentPtr = &Currency1;
					break;
				case 2:
					currentPtr = &Currency2;
					break;
				case 3:
					currentPtr = &Currency3;
					break;
				}

				if (currentPtr->IsValidIndex(pWallet->ArrIndex))
				{
					int32& rCurrent = (*currentPtr)[pWallet->ArrIndex];
					int32 nRefillMax = GetRefillLimit(pWallet);
					if (rCurrent >= nRefillMax)
					{
						// can't over Refillmax
					}
					else if (rCurrent + pWallet->RefillAmount >= nRefillMax)
					{
						rCurrent = nRefillMax;
					}
					else
					{
						rCurrent += pWallet->RefillAmount;
					}
				}
			}
			UpdateCurrency();
			RGameInstance->EventManager->OnUpdateUserInfo.Broadcast();
			walletUpdate = (int32)RGameInstance->UTC_DiffTimespan.GetTotalSeconds();
		}
	}
}

int32 URInventory::GetRefillLimit(FWalletTableInfo* InWalletInfo)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (!InWalletInfo)
		return 0;

	if (InWalletInfo->CommonStringKey == FName(TEXT("Stamina")) ||
		InWalletInfo->CommonStringKey == FName(TEXT("Stamina_pvp")))
	{
		FAVATAR& Avatar = RGameInstance->UserInfo->GetAvatarData();
		FLevelGrowthTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(Avatar.level)));
		if (tableInfo)
		{
			bool bStamina = InWalletInfo->CommonStringKey == FName(TEXT("Stamina"));
			return bStamina ? tableInfo->StaminaMax : tableInfo->StaminaPVPMax;
		}
		
		return 0;
	}
	else
	{
		return InWalletInfo->RefillMax;
	}


}

int32 URInventory::GetGold() const
{
	return GOLD;
}

int32 URInventory::GetGem() const
{
	return GEM;
}

int32 URInventory::GetStamina() const
{
	return STAMINA;
}

int32 URInventory::GetStaminaPVP() const
{
	return STAMINA_PVP;
}

int32 URInventory::GetHeroRune() const
{
	return HERO_RUNE;
}

int32 URInventory::GetPVPToken() const
{
	return PVP_TOKEN;
}

int32 URInventory::GetArenaDailyBonusToken() const
{
	return ARENA_DAILYBONUSTOKEN;
}

int32 URInventory::GetArenaDefenderResetToken() const
{
	return ARENA_DEFENDERRESETTOKEN;
}

int32 URInventory::GetMIRROR_DEFENDERRESET_FREE() const
{
	return MIRROR_DEFENDERRESET_FREE;
}

int32 URInventory::GetMIRROR_ENTRANCE() const
{
	return MIRROR_ENTRANCE;
}

int32 URInventory::GetSummonTicket01() const
{
	return SUMMONTICKET01;
}

int32 URInventory::GetSummonTicket02() const
{
	return SUMMONTICKET02;
}

int32 URInventory::GetMileage() const
{
	return MILEAGE;
}

int32 URInventory::GetFloorStoreTicket() const
{
	return FLOORSTORE_TICKET;
}

int32 URInventory::GetFloorStoreResetToken() const
{
	return FLOORSTORE_RESETTOKEN;
}

int32 URInventory::GetFloorStoreResetTokenGem() const
{
	return FLOORSTORE_RESETTOKEN_GEM;
}

int32 URInventory::GetMISSION_ADD_SCROLL() const
{
	return MISSION_ADD_SCROLL;
}

int32 URInventory::GetHeroUpgradeMaterial() const
{
	return HEROUPGRADEMATERIAL;
}

int32 URInventory::GetS_TOWERBOSS_F1() const
{
	return S_TOWERBOSS_F1;
}

TArray<int32>* URInventory::GetRune()
{
	return &RuneInventory;
}

FDECKS URInventory::GetHeroDeck()
{
	return decks;
}

bool URInventory::IsEmptyDefenceDeck()
{
	bool bEmpty = true;
	for (FString& ud : decks.defense)
	{
		if (!ud.IsEmpty())
		{
			bEmpty = false;
			break;
		}
	}
	return bEmpty;
}

int32 URInventory::GetCurrencyWithKey(FName WalletKey)
{
	FWalletTableInfo* walletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WalletKey);

	return GetCurrencyData(walletInfo);
}

int32 URInventory::GetRuneWithGrade(int32 RuneGrade)
{
	int32 Index = RuneGrade - 1;
	if (!ensure(RuneInventory.IsValidIndex(Index)))
		return 0;
	return RuneInventory[Index];
}

FName URInventory::RuneGradeToWalletKey(int32 RuneGrade)
{
	int32 Index = RuneGrade - 1;
	if (!ensure(RuneWallet.IsValidIndex(Index)))
		return NAME_None;
	return RuneWallet[Index];
}

void URInventory::UpdateInventory(FINVENTORY_UPDATE& inven_update, bool bCheckHeroUpdate)
{
	UpdateCurrency(inven_update);
	UpdateHeroData(inven_update.heros_update, bCheckHeroUpdate);
	DeleteHeroData(inven_update.heros_delete);
	UpdateItemData(inven_update.items_update);
	DeleteItemData(inven_update.items_delete);
	SetDecks(inven_update.decks);
	UpdateRuneInventory();
	UpdateFloorCoinInventory();

	/*Inventory Size Update*/
	if (inven_update.size.IsValidIndex(0))
		INVENTORY_ITEM_CURRENTCOUNT = inven_update.size[0];
	if (inven_update.size.IsValidIndex(1))
		INVENTORY_ITEM_MAXCOUNT = inven_update.size[1];
	if (inven_update.size.IsValidIndex(2))
		INVENTORY_HERO_CURRENTCOUNT = inven_update.size[2];
	if (inven_update.size.IsValidIndex(3))
		INVENTORY_HERO_MAXCOUNT = inven_update.size[3];
	/*Inventory Size Update End*/

	UE_LOG(LogRInventory, Log, TEXT("URInventory::UpdateInventory"));
}


void URInventory::Filter_Class(TArray<FString>& ItemList)
{
	TArray<FString> FilterItemList;

	for (FString& ud : ItemList)
	{
		FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ud);
		if (!ItemData)
			continue;

		FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
		if (!ItemInfo)
			continue;

		for (EAttackTypeEnum eType : AttackTypeFilter)
		{
			if (eType == ItemInfo->EquipLimit)
			{
				FilterItemList.Emplace(ud);
				break;
			}
		}
	}
	ItemList = FilterItemList;
}

void URInventory::Filter_Grade(TArray<FString>& ItemList)
{
	TArray<FString> FilterItemList;

	for (FString& ud : ItemList)
	{
		FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ud);
		if (!ItemData)
			continue;

		FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
		if (!ItemInfo)
			continue;

		for (EItemGradeEnum eType : itemGradeFilter)
		{
 			if (eType == ItemInfo->ItemGrade)
 			{
 			 	FilterItemList.Emplace(ud);
 			 	break;
 			}
		}
	}
	ItemList = FilterItemList;
}

void URInventory::Filter_Tier(TArray<FString>& ItemList)
{
	TArray<FString> FilterItemList;

	for (FString& ud : ItemList)
	{
		FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ud);
		if (!ItemData)
			continue;

		FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
		if (!ItemInfo)
			continue;

		for (int32 eType : itemTierFilter)
		{
			if (eType == ItemInfo->ItemLevel)
			{
				FilterItemList.Emplace(ud);
				break;
			}
		}
	}
	ItemList = FilterItemList;
}

void URInventory::Filter_EquipType(TArray<FString>& ItemList)
{
	TArray<FString> FilterItemList;

	for (FString& ud : ItemList)
	{
		FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ud);
		if (!ItemData)
			continue;

		FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
		if (!ItemInfo)
			continue;

		/*if (EquipTypeFilter.Find(ItemInfo->EquipType) != INDEX_NONE)
		{
			FilterItemList.Emplace(ud);
		}*/
		for (EItemEquipTypeEnum eType : EquipTypeFilter)
		{
			if (eType == ItemInfo->EquipType)
			{
				FilterItemList.Emplace(ud);
				break;
			}
		}
	}
	ItemList = FilterItemList;
}

void URInventory::Filter_EnchantType(TArray<FString>& ItemList)
{
	TArray<FString> FilterItemList;

	for (FString& ud : ItemList)
	{
		FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ud);
		if (!ItemData)
			continue;

		FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
		if (!ItemInfo)
			continue;
		
		for (EItemEnchantStateEnum eType : EnchantStateFilter)
		{
			if (eType == EItemEnchantStateEnum::NoEnchant)
			{
				if (ItemData->enchant == 0)
				{
					FilterItemList.Emplace(ud);
					break;
				}
			}
			else if (eType == EItemEnchantStateEnum::HaveEnchant)
			{
				if (ItemData->enchant > 0 && ItemData->enchant < ItemInfo->MaxEnchantLevel)
				{
					FilterItemList.Emplace(ud);
					break;
				}
			}
			else if (eType == EItemEnchantStateEnum::MaxEnchant)
			{
				if (ItemData->enchant == ItemInfo->MaxEnchantLevel)
				{
					FilterItemList.Emplace(ud);
					break;
				}
			}
			else if (eType == EItemEnchantStateEnum::CantEnchant)
			{
				if (ItemData->enchantLocked)
				{
					FilterItemList.Emplace(ud);
					break;
				}
			}
		}
	}
	ItemList = FilterItemList;
}

void URInventory::Filter_UseStateType(TArray<FString>& ItemList)
{
	TArray<FString> FilterItemList;

	for (FString& ud : ItemList)
	{
		FITEM* ItemData = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ud);
		if (!ItemData)
			continue;

		FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
		if (!ItemInfo)
			continue;

		for (EItemUseStateEnum eType : UseStateFilter)
		{
			if (eType == EItemUseStateEnum::Using)
			{
				if (!(ItemData->equipedHeroUD).IsEmpty())
				{
					FilterItemList.Emplace(ud);
					break;
				}
			}
			else if (eType == EItemUseStateEnum::Locking)
			{
				if (ItemData->locked)
				{
					FilterItemList.Emplace(ud);
					break;
				}
			}
			else if (eType == EItemUseStateEnum::EquipedRune)
			{
				if((ItemData->runes).Num() > 0)
				{
					FilterItemList.Emplace(ud);
					break;
				}
			}
		}
	}
	ItemList = FilterItemList;
}

void URInventory::SetHeroData(TArray<FHERO>& Heros)
{
	HeroInventoryMap.Reset();
//	HeroInventoryMap.Reserve(Heros.Num());
	UpdateHeroData(Heros, false);
	UE_LOG(LogRInventory, Log, TEXT("URInventory::SetHeroData, Count : %d"), Heros.Num());
}

void URInventory::DeleteHeroData(TArray<FString>& heros_delete)
{
	if (heros_delete.GetAllocatedSize() <= 0)
		return;
	for (int32 i = 0; i < heros_delete.Num(); i++)
		HeroInventoryMap.Remove(heros_delete[i]);
}

void URInventory::SetItemData(TArray<FITEM>& Items)
{
	ItemInventoryMap.Reset();
	ItemInventoryMap.Reserve(Items.Num());
	UpdateItemData(Items);
	UE_LOG(LogRInventory, Log, TEXT("URInventory::SetItemData, Count : %d"), Items.Num());
}

void URInventory::DeleteItemData(TArray<FString>& items_delete)
{
	if (items_delete.GetAllocatedSize() <= 0)
		return;
	for (int32 i = 0; i < items_delete.Num(); i++)
		ItemInventoryMap.Remove(items_delete[i]);
}

int32 URInventory::GetCurrencyData(FWalletTableInfo* walletInfo)
{
	if (walletInfo == nullptr)
		return 0;
	if (walletInfo->ArrGroup != 1 && walletInfo->ArrGroup != 2 && walletInfo->ArrGroup != 3)
		return 0;
	TArray<int32>* currentPtr = nullptr;
	switch (walletInfo->ArrGroup)
	{
	case 1:
		currentPtr = &Currency1;
		break;
	case 2:
		currentPtr = &Currency2;
		break;
	case 3:
		currentPtr = &Currency3;
		break;
	}
	if (currentPtr == nullptr)
		return 0;
	if (currentPtr->GetAllocatedSize() == 0)
		return 0;
	if (currentPtr->IsValidIndex(walletInfo->ArrIndex))
		return (*currentPtr)[walletInfo->ArrIndex];
	return 0;
}

void URInventory::InitRuneInventory()
{
	RuneWallet.Reset(20);
	RuneWallet.Emplace(WALLET_RUNE001);
	RuneWallet.Emplace(WALLET_RUNE002);
	RuneWallet.Emplace(WALLET_RUNE003);
	RuneWallet.Emplace(WALLET_RUNE004);
	RuneWallet.Emplace(WALLET_RUNE005);
	RuneWallet.Emplace(WALLET_RUNE006);
	RuneWallet.Emplace(WALLET_RUNE007);
	RuneWallet.Emplace(WALLET_RUNE008);
	RuneWallet.Emplace(WALLET_RUNE009);
	RuneWallet.Emplace(WALLET_RUNE010);
	RuneWallet.Emplace(WALLET_RUNE011);
	RuneWallet.Emplace(WALLET_RUNE012);
	RuneWallet.Emplace(WALLET_RUNE013);
	RuneWallet.Emplace(WALLET_RUNE014);
	RuneWallet.Emplace(WALLET_RUNE015);
	RuneWallet.Emplace(WALLET_RUNE016);
	RuneWallet.Emplace(WALLET_RUNE017);
	RuneWallet.Emplace(WALLET_RUNE018);
	RuneWallet.Emplace(WALLET_RUNE019);
	RuneWallet.Emplace(WALLET_RUNE020);

	UTableManager* tblMgr = UTableManager::GetInstancePtr();

	if (tblMgr == nullptr)
		return;

	FWalletTableInfo* walletInfo = tblMgr->GetWalletRow(WALLET_GOLD);
	if (walletInfo == nullptr)
		return;

	RuneInventory.Reset(20);
	for (int32 i = 0; i < 20; i++)
	{
		walletInfo = tblMgr->GetWalletRow(RuneWallet[i]);
		int32 Count = GetCurrencyData(walletInfo);
		
		RuneInventory.Emplace(Count);
	}
}

void URInventory::UpdateRuneInventory()
{
	InitRuneInventory();
}

void URInventory::InitFloorCoinInventory()
{
	FloorCoinWallet.Reset(20);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN05);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN06);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN07);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN08);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN09);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN10);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN11);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN12);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN13);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN14);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN15);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN16);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN17);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN18);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN19);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN20);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN21);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN22);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN23);
	FloorCoinWallet.Emplace(WALLET_FLOORCOIN24);

	UTableManager* tblMgr = UTableManager::GetInstancePtr();

	if (tblMgr == nullptr)
		return;

	FWalletTableInfo* walletInfo = nullptr;

	FloorCoinInventory.Reset(0);
	for (int i = 0; i < FloorCoinWallet.Num(); i++)
	{
		walletInfo = tblMgr->GetWalletRow(FloorCoinWallet[i]);

		if(walletInfo == nullptr)
			continue;

		int32 Count = GetCurrencyData(walletInfo);

		FloorCoinInventory.Emplace(Count);
	}
}

void URInventory::UpdateFloorCoinInventory()
{
	InitFloorCoinInventory();
}

int32 URInventory::GetFloorCoin(FName InCurrencyType)
{
	int32	Index = FloorCoinWallet.Find(InCurrencyType);
	
	if (FloorCoinInventory.IsValidIndex(Index))
		return FloorCoinInventory[Index];

	return 0;
}

int32 URInventory::GetCurrency(FName InCurrencyType)
{
	if (InCurrencyType == WALLET_GOLD)
		return GetGold();
	else if (InCurrencyType == WALLET_GEM)
		return GetGem();
	else if (InCurrencyType == WALLET_STAMINA)
		return GetStamina();
	else if (InCurrencyType == WALLET_STAMINA_PVP)
		return GetStaminaPVP();
	else if (InCurrencyType == WALLET_HERO_RUNE)
		return GetHeroRune();
	else if (InCurrencyType == WALLET_PVPTOKEN)
		return GetHeroRune();
	else if (InCurrencyType == WALLET_CASH)
		return 0;
	else
		return GetFloorCoin(InCurrencyType);

	return 0;
}

void URInventory::UpdateHeroData(TArray<FHERO>& Heros, bool bCheckAttackPower)
{
	for (FHERO& HeroEntity : Heros)
	{
		UpdateHeroData(HeroEntity, false, bCheckAttackPower);
	}

//	RGAMEINSTANCE(this)->EventManager->OnUI_HeroInvenUpdate.Broadcast("");
	UE_LOG(LogRInventory, Log, TEXT("URInventory::UpdateHeroData()"));
}

void URInventory::UpdateHeroData(FHERO& HeroEntity, bool InvokeDelegate, bool bCheckAttackPower)
{
	if (HeroEntity.heroId.IsNone())
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FCharacterTableInfo* CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroEntity.heroId);
	checkf(CharacterInfo != nullptr, TEXT("URInventory::UpdateHeroData[CharacterInfo is Empty(%s)]"), *HeroEntity.heroId.ToString());

	int32 Prevpower = 0;
	if (bCheckAttackPower)
		Prevpower = UClient_BattleFunctionLibrary::GetHeroCombatStat(HeroEntity.heroUD);
	HeroInventoryMap.Emplace(HeroEntity.heroUD, HeroEntity);
	CheckSkillActivate(HeroEntity.heroUD);

	if (bCheckAttackPower)
	{
		if (IsValid(RGameInstance->RWidgetManager) && RGameInstance->RWidgetManager->GetCurrentUIPage() != EUIPageEnum::UIPage_ShopHero)
		{
			int32 Currentpower = UClient_BattleFunctionLibrary::GetHeroCombatStat(HeroEntity.heroUD);
			UUIFunctionLibrary::ShowChangeAttackPowerSplash(HeroEntity.heroId, Currentpower, Prevpower);
		}
	}
	UE_LOG(LogRInventory, Log, TEXT("URInventory::UpdateHeroData() UD : %s, ID : %s"), *(HeroEntity.heroUD), *(HeroEntity.heroId.ToString()));

}

void URInventory::UpdateItemData(TArray<FITEM>& Items)
{
	for (auto& ItemEntity : Items)
	{
		UpdateItemData(ItemEntity, false);
	}

	UE_LOG(LogRInventory, Log, TEXT("URInventory::UpdateItemData()"));
}

void URInventory::UpdateItemData(FITEM& ItemEntity, bool InEventBroadcast /*= true*/)
{
	if (ItemEntity.itemId.IsNone())
		return;

	ItemInventoryMap.Emplace(ItemEntity.itemUD, ItemEntity);
	UE_LOG(LogRInventory, Log, TEXT("URInventory::UpdateItemData() UD : %s, ID : %s"), *(ItemEntity.itemUD), *(ItemEntity.itemId.ToString()));
}

void URInventory::RemoveHeroData(const FString& HeroHashKey)
{
	HeroInventoryMap.Remove(HeroHashKey);
	UE_LOG(LogRInventory, Log, TEXT("URInventory::RemoveHeroData() UD : %s"), *(HeroHashKey));
}

FHERO* URInventory::GetHeroWithUD(const FString& HeroHashKey)
{
	return HeroInventoryMap.Find(HeroHashKey);
}

FHERO* URInventory::GetHeroWithID(const FName& HeroID)
{
	FString HeroUD = "";
	for (auto& Elem : HeroInventoryMap)
	{
		if (Elem.Value.heroId == HeroID)
		{
			HeroUD = Elem.Value.heroUD;
			break;
		}
	}
	return GetHeroWithUD(HeroUD);
}

FITEM* URInventory::GetItemDataWithUD(const FString& ItemHashKey)
{
	return ItemInventoryMap.Find(ItemHashKey);
}

FITEM* URInventory::GetItemDataWithID(const FName& ItemID)
{
	FString ItemUD = "";
	for (auto& Elem : ItemInventoryMap)
	{
		if (Elem.Value.itemId == ItemID)
		{
			ItemUD = Elem.Value.itemUD;
			break;
		}
	}
	FITEM* Item = GetItemDataWithUD(ItemUD);
	if (Item == nullptr)
		return nullptr;
	return Item;
}

bool URInventory::GetHeroAndTableData(const FString& HeroUD, FHERO& outData, FWorkedCharacterTableInfo& outTableData, bool InNextUpgrade/* = false*/, int32 InNextLevel/* = 0*/)
{
	FHERO* HeroInvenData = GetHeroWithUD(HeroUD);
	if (HeroInvenData == nullptr)
		return false;
	FName	HeroID = HeroInvenData->heroId;
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
	if (CharacterTableInfo == nullptr)
		return false;
	
	if (InNextUpgrade)
		HeroID = CharacterTableInfo->UpgradeID;

	outData = *HeroInvenData;
	
	bool FindRow = URenewal_BattleFunctionLibrary::GetCharacterTableWorkedInfo(outTableData, ECharacterCategoryTypeEnum::VE_Hero, HeroID, HeroInvenData->level + InNextLevel);
	if (FindRow == false)
		return false;

	return true;
}

bool URInventory::GetSkillDataFromHeroUD(const FString& HeroUD, TArray<FHERO_SKILL>& outData)
{
	FHERO* HeroInvenData = GetHeroWithUD(HeroUD);
	if (HeroInvenData == nullptr)
		return false;
	
	outData.Reserve(HeroInvenData->skills.Num());
	int32 index = 0;
	
	for (auto& skillLevel : HeroInvenData->skills)
	{
		FHERO_SKILL heroSkill;
		heroSkill.level = skillLevel;
		heroSkill.skillId = UCharacterHelpFunctionLibrary::GetHeroSkillIDByIndex(HeroInvenData->heroId, index ).ToString();
		heroSkill.activated = true;
		heroSkill.slotNo = index++;
		outData.Emplace( heroSkill );
	}

	return true;
}

bool URInventory::GetHerosWithID(const FName& HeroID, FString& HeroHashKey)
{
	for (auto& Elem : HeroInventoryMap)
	{
		if (Elem.Value.heroId == HeroID)
		{
			HeroHashKey = Elem.Value.heroUD;
			return true;
		}
	}

	return false;
}

int32 URInventory::GetHeroCountInName()
{
	TArray<FName> tempList;
	for (auto hero : HeroInventoryMap)
	{
		FCharacterTableInfo* CT = UTableManager::GetInstancePtr()->GetCharacterRow(hero.Value.heroId);
		if(CT->IsHero)
			tempList.AddUnique(CT->DisplayName);
	}

	return tempList.Num();
}

void URInventory::GetHeroUDs(TArray<FString>& InHeroUDList, EClassTypeEnum ClassType /* = VE_ClassTypeMax */, EHeroInventoryType eHeroType/* = EHeroInventoryType::VE_TypeHero*/)
{
	if(EClassTypeEnum::VE_ClassTypeMax == ClassType && eHeroType == EHeroInventoryType::VE_InventoryTypeAll)
	{
		InHeroUDList.Empty(HeroInventoryMap.Num());
		HeroInventoryMap.GetKeys(InHeroUDList);
	}
	else
	{
		HeroChangeClassFilter(InHeroUDList, ClassType, eHeroType);
	}
}

void URInventory::GetHeroUDArray(TArray<FString>& InHeroUDList)
{
	InHeroUDList.Empty(HeroInventoryMap.Num());
	HeroInventoryMap.GetKeys(InHeroUDList);
}

void URInventory::GetSpiritUDs(TArray<FString>& InHeroUDList, int32 ListCountMax, bool InbExceptforLock /*= false*/)
{
	if (InHeroUDList.Num() == 0)
		GetHeroUDs(InHeroUDList, EClassTypeEnum::VE_ClassTypeMax, EHeroInventoryType::VE_InventoryTypeAll);
	int32	Count	=	0;
	bool	bHero	=	false;
	TArray<FString> FilterHeroUDList;
	for (auto const& HeroUD : InHeroUDList)
	{
		FHERO* HeroInvenData = GetHeroWithUD(HeroUD);
		if (!HeroInvenData)
			return;
		auto HeroTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
		if (HeroTableInfo == nullptr)
			continue;

		if (InbExceptforLock)
		{
			if(HeroInvenData->locked)
				continue;
		}

		if (HeroTableInfo->IsHero == bHero)
		{
			FilterHeroUDList.Emplace(HeroUD);
			Count++;
		}
		if (Count == ListCountMax)
			break;
	}
	InHeroUDList = FilterHeroUDList;
}

int32 URInventory::GetHeroInventoryCount(bool InIsHero/* = true*/)
{
	int32	Count = 0;
	TArray<FString> FilterHeroUDList;
	for (auto const& Elem : HeroInventoryMap)
	{
		FHERO* HeroInvenData = GetHeroWithUD(Elem.Value.heroUD);

		if (HeroInvenData == nullptr)
			continue;

		auto HeroTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);

		if (HeroTableInfo == nullptr)
			continue;

		if (HeroTableInfo->IsHero == InIsHero)
			Count++;
	}
	return Count;
}

FString URInventory::ChangeItemEquipment(const FString& HeroUD, EItemEquipPositionEnum Position, const FString& ItemHashKey)
{
	FString OldItem = "";

	FHERO* HeroInvenData = GetHeroWithUD(HeroUD);
	if (HeroInvenData == nullptr)
		return OldItem;

	HeroInvenData->equipedItemUDs[(int32)Position] = ItemHashKey;

	return OldItem;
}

void URInventory::CheckSkillActivate(const FString& HeroUD)
{
	auto HeroInvenData = GetHeroWithUD(HeroUD);

	TArray< FHERO_SKILL > OutHeroSkill;
	GetSkillDataFromHeroUD(HeroUD, OutHeroSkill);

	for (auto& HeroSkillElem : OutHeroSkill)
	{
		if (HeroSkillElem.level < 1)
		{
			HeroSkillElem.activated = false;
		}
		else
		{
			HeroSkillElem.activated = true;
		}
	}
}

bool URInventory::IsBagCapacityExceeded()
{
	// Editor override
#if WITH_EDITOR
	bool IgnoreBagCapacityExceeded = RGAMEINSTANCE(this)->GameEnvironmentVar.IgnoreBagCapacityExceeded;
	if (IgnoreBagCapacityExceeded)
		return false;
#endif

	// overcapacity if the items count EXCEEDS the bag capacity
	int32 OwnedItemsCount = GetOwnedItemsCount();
	int32 BagCapacity = GetBagCapacity();

	return OwnedItemsCount > BagCapacity;
}

int32 URInventory::GetOwnedItemsCount()
{
	// (tentative design) count one for one slot; stackable item is counted as 1 if there is at least one.
	// for each inventory (equipment, runes, hero pieces, materials)
	int32 CountEquipmentItems = RGAMEINSTANCE(this)->RInventory->GetItemCount();

	// get CountWalletTypeItems. room for optimization
	int32 CountWalletTypeItems = 0;
	TArray<FName> WalletKeys = UTableManager::GetInstancePtr()->DTWallet->GetRowNames();
	for (FName WalletKey : WalletKeys)
	{
		FWalletTableInfo* WalletInfo = UTableManager::GetInstancePtr()->GetWalletRow(WalletKey);
		if (!ensure(WalletInfo != nullptr))
			continue;

		int32 Amount = GetCurrencyWithKey(WalletKey);
		if (Amount > 0)
		{
			CountWalletTypeItems++;
		}
	}

	return CountEquipmentItems + CountWalletTypeItems;
}

int32 URInventory::GetBagCapacity()
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (!IsValid(GenericScrollview))
		return 0;

	bool bHeroInventory = GenericScrollview->Variables.State <= EScrollviewType::VE_ITEM_WEAPON;
	
	int32 InventorySlotTokenCount = bHeroInventory? GetCurrencyWithKey(WALLET_HERO_INVENTORY_SLOT_TOKEN) : GetCurrencyWithKey(WALLET_ITEM_INVENTORY_SLOT_TOKEN);

	// I don't like that these values built in to the client...
	FConstantVariablesTableInfo* ConstantInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("ITEM_INVENTORY_START_SIZE")));
	check(ConstantInfo != nullptr);

	int32 InventoryStartSize = 0;
	ConstantInfo->GetValue(InventoryStartSize);

	ConstantInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("ITEM_INVENTORY_AddRate")));
	check(ConstantInfo != nullptr);
	int32 InventoryAddRate = 0;
	ConstantInfo->GetValue(InventoryAddRate);

	return InventoryStartSize + InventoryAddRate * InventorySlotTokenCount;
}

bool URInventory::CanBagBeExtendedFurther()
{
	FConstantVariablesTableInfo* ConstantInfo = UTableManager::GetInstancePtr()->GetConstantVariablesRow(FName(TEXT("ITEM_INVENTORY_AddMax")));
	check(ConstantInfo != nullptr);

	int32 InventoryAddMax = 0;
	ConstantInfo->GetValue(InventoryAddMax);

	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (!IsValid(GenericScrollview))
		return false;

	bool bHeroInventory = GenericScrollview->Variables.State <= EScrollviewType::VE_ITEM_WEAPON;

	int32 InventorySlotTokenCount = bHeroInventory ? GetCurrencyWithKey(WALLET_HERO_INVENTORY_SLOT_TOKEN) : GetCurrencyWithKey(WALLET_ITEM_INVENTORY_SLOT_TOKEN);

	return InventorySlotTokenCount < InventoryAddMax;
}

int32 URInventory::NextBagExtensionBluestoneCost()
{
	// (design) NOT CERTAIN the formula 10 + 10*tokens ???
	//int32 CurrentBluestoneCount = RGAMEINSTANCE(this)->RInventory->GetCurrencyWithKey(WALLET_GEM);
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (!IsValid(GenericScrollview))
		return 0;

	bool bHeroInventory = GenericScrollview->Variables.State <= EScrollviewType::VE_ITEM_WEAPON;

	int32 CurrentSlotTokensCount = bHeroInventory ? GetCurrencyWithKey(WALLET_HERO_INVENTORY_SLOT_TOKEN) : GetCurrencyWithKey(WALLET_ITEM_INVENTORY_SLOT_TOKEN);

	return 10 + 10 * CurrentSlotTokensCount;
}

bool URInventory::GetFlagWithCompareValue(int32 ValueA, int32 ValueB, bool Flag)
{
	if (ValueA > ValueB)
		return Flag;
	else if (ValueA < ValueB)
		return !Flag;
	else
		return Flag;
}

bool URInventory::GetNewHeroData(FString& HeroUD)
{
	if (NewHeroArray.Num() > 0)
	{
		HeroUD = NewHeroArray.Pop();
		return true;
	}

	return false;
}

void URInventory::GetProtectedHeros(TArray<FString>& outHeroUDs)
{
	for (auto& Element : HeroInventoryMap)
	{
		if (Element.Value.locked)
			outHeroUDs.AddUnique(Element.Key);
	}
}


bool URInventory::IsHaveNewHeroData()
{
	return (NewHeroArray.Num() > 0 ? true : false);
}

void URInventory::TestNewHeroData(FString& HeroUD)
{
	NewHeroArray.Emplace(HeroUD);
}

void URInventory::GetHerosID(const FName& HeroID, TArray<FString>& HeroHashKey)
{
	for (auto& Elem : HeroInventoryMap)
	{
		if (Elem.Value.heroId == HeroID)
		{
			HeroHashKey.Emplace(Elem.Value.heroUD);
			continue;
		}
	}
}


void URInventory::InitCurrency(FINVENTORY& inventory)
{
//	Currency1.Reset(0);
	Currency1 = TArray<int32>(inventory.currency1);
	Currency2 = TArray<int32>(inventory.currency2);
	Currency3 = TArray<int32>(inventory.currency3);

	UpdateCurrency();

	InitRuneInventory();
	InitFloorCoinInventory();
}

void URInventory::UpdateCurrency(FINVENTORY_UPDATE& update_inven)
{
	Currency1 = TArray<int32>(update_inven.currency1);
	Currency2 = TArray<int32>(update_inven.currency2);
	Currency3 = TArray<int32>(update_inven.currency3);

	UpdateCurrency();
}

void URInventory::UpdateCurrency()
{
	UTableManager* tblMgr = UTableManager::GetInstancePtr();

	FWalletTableInfo* walletInfo = tblMgr->GetWalletRow(WALLET_GOLD);
	GOLD = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_GEM);
	GEM = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_STAMINA);
	STAMINA = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_STAMINA_PVP);
	STAMINA_PVP = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_HERO_RUNE);
	HERO_RUNE = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_PVPTOKEN);
	PVP_TOKEN = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_ARENA_DAILYBONUSTOKEN);
	ARENA_DAILYBONUSTOKEN = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_ARENA_DEFENDERRESETTOKEN);
	ARENA_DEFENDERRESETTOKEN = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_MIRROR_DEFENDERRESET_FREE);
	MIRROR_DEFENDERRESET_FREE = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_MIRROR_ENTRANCE);
	MIRROR_ENTRANCE = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_SUMMONTICKET01);
	SUMMONTICKET01 = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_SUMMONTICKET02);
	SUMMONTICKET02 = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_MILEAGE);
	MILEAGE = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_FLOORSTORETICKET);
	FLOORSTORE_TICKET = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_FLOORSTORE_RESETTOKEN);
	FLOORSTORE_RESETTOKEN = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_FLOORSTORE_RESETTOKEN_GEM);
	FLOORSTORE_RESETTOKEN_GEM = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_MISSION_ADD_SCROLL);
	MISSION_ADD_SCROLL = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_HERO_UPGRADEMATERIAL);
	HEROUPGRADEMATERIAL = GetCurrencyData(walletInfo);

	walletInfo = tblMgr->GetWalletRow(WALLET_S_TOWERBOSS_F1);
	S_TOWERBOSS_F1 = GetCurrencyData(walletInfo);
}

void URInventory::SetHeroSort(TArray<FString>& InHeroUDList, EFilterSortTypeEnum InSortType, bool bIsDescending)
{
 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
//  	if (IsValid(RGameInstance) == false)
//  		return;
 
 	TArray<FHERO> heroList;
 	TArray<FString> sortUdList;
	TMap<FString, int32> assistList;
 	heroList.Reset(InHeroUDList.Num());
 	sortUdList.Reset(InHeroUDList.Num());
	assistList.Reset();
	assistList.Reserve(InHeroUDList.Num());
 	for (FString& ud : InHeroUDList)
 	{
		FHERO* outData = GetHeroWithUD(ud);
		if (outData != nullptr)
		{
			UCharacterBattleInfo* UICharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(ud);
			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(outData->heroId);
			if (InSortType == EFilterSortTypeEnum::VE_AttackPower)
			{
				int32 power = UClient_BattleFunctionLibrary::GetHeroCombatStat(ud);
				assistList.Emplace(ud, power);
			}
			else if (InSortType == EFilterSortTypeEnum::VE_Damage)
			{
				int32 val = (UICharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower) > UICharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MagicAttackPower) ?
					UICharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_PhysicalAttackPower) :
					UICharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MagicAttackPower));
				assistList.Emplace(ud, val);
			}
			else if (InSortType == EFilterSortTypeEnum::VE_Grade)
			{
				int32 val = CharacterTableInfo->Grade;
				assistList.Emplace(ud, val);
			}
			else if (InSortType == EFilterSortTypeEnum::VE_PhysicalDefense)
			{
				int32 val = UICharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_Armor);
				assistList.Emplace(ud, val);
			}
			else if (InSortType == EFilterSortTypeEnum::VE_MagicDefense)
			{
				int32 val = UICharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MagicResistance);
				assistList.Emplace(ud, val);
			}
			else if (InSortType == EFilterSortTypeEnum::VE_Health)
			{
				int32 val = UICharacterBattleInfo->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
				assistList.Emplace(ud, val);
			}
			else if (InSortType == EFilterSortTypeEnum::VE_SortingOrder)
			{
				auto HeroTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(outData->heroId);
				if (HeroTableInfo)
					assistList[ud] = HeroTableInfo->Sorting_Order;
				else
					assistList[ud] = 0;
			}
			UICharacterBattleInfo = nullptr;
			heroList.Emplace(*outData);
		}
	}

	switch (InSortType)
	{
		case EFilterSortTypeEnum::VE_Id:
		{
			heroList.Sort([&](const FHERO& A, const FHERO& B) {
				return FCString::Atoi(*A.heroId.ToString()) < FCString::Atoi(*B.heroId.ToString());
			});
		}
		break;
		case EFilterSortTypeEnum::VE_Level:
		{
			heroList.Sort([&](const FHERO& A, const FHERO& B) {
				if (A.level > B.level)
				{
					return bIsDescending;
				}
				else if (A.level < B.level)
				{
					return !bIsDescending;
				}
				else
				{
					int32 lEnchantStat = 0;
					int32 rEnchantStat = 0;
					for (int32 i = 0; i < A.enchantStat.Num(); i++)
						lEnchantStat += A.enchantStat[i];

					for (int32 i = 0; i < B.enchantStat.Num(); i++)
						rEnchantStat += B.enchantStat[i];

					if (lEnchantStat > rEnchantStat)
						return true;
					else if (lEnchantStat < rEnchantStat)
						return false;
					else
					{
						FCharacterTableInfo* LD = UTableManager::GetInstancePtr()->GetCharacterRow(A.heroId);
						FCharacterTableInfo* RD = UTableManager::GetInstancePtr()->GetCharacterRow(B.heroId);
						if (LD->Grade > RD->Grade)
							return true;
						else if (LD->Grade < RD->Grade)
							return false;
						else
						{
							int32 LP = UClient_BattleFunctionLibrary::GetHeroCombatStat(A.heroUD);
							int32 RP = UClient_BattleFunctionLibrary::GetHeroCombatStat(B.heroUD);
							if (LP > RP)
								return true;
							else if (LP < RP)
								return false;
							else
								return FCString::Atoi(*A.heroId.ToString()) < FCString::Atoi(*B.heroId.ToString());
						}
					}
				}
			});
		}
		break;
		case EFilterSortTypeEnum::VE_AttackPower:
		case EFilterSortTypeEnum::VE_Damage:
		case EFilterSortTypeEnum::VE_PhysicalDefense:
		case EFilterSortTypeEnum::VE_MagicDefense:
		case EFilterSortTypeEnum::VE_Health:
		{
			heroList.Sort([&](const FHERO& A, const FHERO& B) {
				if (assistList[A.heroUD] > assistList[B.heroUD])
				{
					return bIsDescending;
				}
				else if (assistList[A.heroUD] < assistList[B.heroUD])
				{
					return !bIsDescending;
				}
				else
				{
					if (A.level > B.level)
						return true;
					else if (A.level < B.level)
						return false;
					else
					{
						int32 lEnchantStat = 0;
						int32 rEnchantStat = 0;
						for (int32 i = 0; i < A.enchantStat.Num(); i++)
							lEnchantStat += A.enchantStat[i];
						
						for (int32 i = 0; i < B.enchantStat.Num(); i++)
							rEnchantStat += B.enchantStat[i];

						if (lEnchantStat > rEnchantStat)
							return true;
						else if (lEnchantStat < rEnchantStat)
							return false;
						else
						{
							FCharacterTableInfo* LD = UTableManager::GetInstancePtr()->GetCharacterRow(A.heroId);
							FCharacterTableInfo* RD = UTableManager::GetInstancePtr()->GetCharacterRow(B.heroId);
							if (LD->Grade > RD->Grade)
								return true;
							else if (LD->Grade < RD->Grade)
								return false;
							else
								return FCString::Atoi(*A.heroId.ToString()) < FCString::Atoi(*B.heroId.ToString());
						}
					}
				}
			});
		}
		break;
		case EFilterSortTypeEnum::VE_Grade:
		{
			heroList.Sort([&](const FHERO& A, const FHERO& B) {
				if (assistList[A.heroUD] > assistList[B.heroUD])
				{
					return bIsDescending;
				}
				else if (assistList[A.heroUD] < assistList[B.heroUD])
				{
					return !bIsDescending;
				}
				else
				{
					if (A.level > B.level)
						return true;
					else if (A.level < B.level)
						return false;
					else
					{
						int32 lEnchantStat = 0;
						int32 rEnchantStat = 0;
						for (int32 i = 0; i < A.enchantStat.Num(); i++)
							lEnchantStat += A.enchantStat[i];

						for (int32 i = 0; i < B.enchantStat.Num(); i++)
							rEnchantStat += B.enchantStat[i];

						if (lEnchantStat > rEnchantStat)
							return true;
						else if (lEnchantStat < rEnchantStat)
							return false;
						else
						{
							int32 LP = UClient_BattleFunctionLibrary::GetHeroCombatStat(A.heroUD);
							int32 RP = UClient_BattleFunctionLibrary::GetHeroCombatStat(B.heroUD);
							if (LP > RP)
								return true;
							else if (LP < RP)
								return false;
							else
								return FCString::Atoi(*A.heroId.ToString()) < FCString::Atoi(*B.heroId.ToString());
						}
					}
				}
			});
		}
		break;
		case EFilterSortTypeEnum::VE_SortingOrder:
		{
			heroList.StableSort([&](const FHERO& A, const FHERO& B) {
				return (assistList[A.heroUD] < assistList[B.heroUD]);
			});
		}
		break;
	}

	for (FHERO& hero : heroList)
	{
		sortUdList.Emplace(hero.heroUD);
	}

	InHeroUDList = sortUdList;
}

void URInventory::SetItemSort(TArray<FString>& ItemList, EFilterSortTypeEnum InSortType, bool SortAscending)
{
	auto TM = UTableManager::GetInstancePtr();

	ItemList.Sort([&](const FString& ItemUD_A, const FString& ItemUD_B) {
		FITEM* ItemData_A = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD_A);
		check(ItemData_A != nullptr);
		FItemTableInfo* ItemInfo_A = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData_A->itemId);
		check(ItemInfo_A != nullptr);

		FITEM* ItemData_B = RGAMEINSTANCE(this)->RInventory->GetItemDataWithUD(ItemUD_B);
		check(ItemData_B != nullptr);
		FItemTableInfo* ItemInfo_B = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData_B->itemId);
		check(ItemInfo_B != nullptr);

		int32 Grade_A = (int32)ItemInfo_A->ItemGrade;
		int32 Grade_B = (int32)ItemInfo_B->ItemGrade;

		int32 CombatPower_A = FItemTableInfo::GetCombatPower(ItemData_A);
		int32 CombatPower_B = FItemTableInfo::GetCombatPower(ItemData_B);

		int32 CreationNumber_A = ItemUDToCreationNumber(ItemUD_A);
		int32 CreationNumber_B = ItemUDToCreationNumber(ItemUD_B);

		switch (InSortType)
		{
		case EFilterSortTypeEnum::VE_Grade:
		{
			// Grade
			if (Grade_A != Grade_B)
			{
				return GetFlagWithCompareValue(Grade_A, Grade_B, SortAscending);
			}
			else
			{
				//combatpower
				if (CombatPower_A != CombatPower_B)
				{
					return GetFlagWithCompareValue(CombatPower_A, CombatPower_B, SortAscending);
				}
				else
				{
					//CreationNumber
					if (CreationNumber_A != CreationNumber_B)
					{
						return GetFlagWithCompareValue(CreationNumber_A, CreationNumber_B, SortAscending);
					}
					else
					{
						return true;
					}
				}
			}
		}
			break;
		case EFilterSortTypeEnum::VE_AttackPower:
		{
			// combatpower
			if (CombatPower_A != CombatPower_B)
			{
				return GetFlagWithCompareValue(CombatPower_A, CombatPower_B, SortAscending);
			}
			else
			{
				// Grade
				if (Grade_A != Grade_B)
				{
					return GetFlagWithCompareValue(Grade_A, Grade_B, SortAscending);
				}
				else
				{
					//CreationNumber
					if (CreationNumber_A != CreationNumber_B)
					{
						return GetFlagWithCompareValue(CreationNumber_A, CreationNumber_B, SortAscending);
					}
					else
					{
						return true;
					}
				}
			}
		}
			break;
		case EFilterSortTypeEnum::VE_SortingOrder:
		{
			//CreationNumber
			if (CreationNumber_A != CreationNumber_B)
			{
				return GetFlagWithCompareValue(CreationNumber_A, CreationNumber_B, SortAscending);
			}
			else
			{
				// Grade
				if (Grade_A != Grade_B)
				{
					return GetFlagWithCompareValue(Grade_A, Grade_B, SortAscending);
				}
				else
				{
					// CombatPower
					if (CombatPower_A != CombatPower_B)
					{
						return GetFlagWithCompareValue(CombatPower_A, CombatPower_B, SortAscending);
					}
					else
					{
						return true;
					}
				}
			}
		}
			break;
		default:
			return true;
			break;
		}
	});
}
int32 URInventory::ItemUDToCreationNumber(const FString& InItemUD)
{
	FString _Left;
	FString Right;
	if (!InItemUD.Split(TEXT("-"), &_Left, &Right))
	{
		UE_LOG(LogUI, Log, TEXT("UUC_ItemInventoryBase::ItemUDToCreationNumber: Unrecognized ItemUD format: %s"), *InItemUD);
		return 0;
	}

	return FCString::Atoi(*Right);
}

void URInventory::GetHerosWithGrade(TArray<FString>& InHeroUDList, int32 InGrade, EHeroInventoryType InventoryType, bool IsLockCollect /* = true */)
{
	for (auto Elem : HeroInventoryMap)
	{
		FHERO* Hero = GetHeroWithUD(Elem.Key);
		if (Hero == nullptr)
			continue;
		
		FCharacterTableInfo* HeroTable = UTableManager::GetInstancePtr()->GetCharacterRow(Hero->heroId);

		if(HeroTable == nullptr)
			continue;

		if (InventoryType == EHeroInventoryType::VE_TypeHero)
		{
			if (HeroTable->IsHero == false)
				continue;
		}
		else if (InventoryType == EHeroInventoryType::VE_TypeSpirit)
		{
			if (HeroTable->IsHero)
				continue;
		}
		
		if (HeroTable->Grade == InGrade)
		{
			if (IsLockCollect)
			{
				InHeroUDList.Emplace(Elem.Value.heroUD);
			}
			else
			{
				if(Hero->locked == false)
					InHeroUDList.Emplace(Elem.Value.heroUD);
			}
		}
			
	}
}

void URInventory::HeroChangeClassFilter(TArray<FString>& InHeroUDList, EClassTypeEnum InClass, EHeroInventoryType eHeroType/* = EHeroInventoryType::VE_TypeHero*/)
{
	bool bHero = (eHeroType == EHeroInventoryType::VE_TypeHero);
	
	TArray<FString> FilterHeroUDList;
	for (auto const& HeroUD : InHeroUDList)
	{
		FHERO* HeroInvenData = GetHeroWithUD(HeroUD);
		if (!HeroInvenData)
			return;
		auto HeroTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
		if (HeroTableInfo == nullptr)
			continue;

		if (InClass == EClassTypeEnum::VE_ClassTypeMax || InClass == HeroTableInfo->Class)
		{
			if (HeroTableInfo->IsHero == bHero || eHeroType == EHeroInventoryType::VE_InventoryTypeAll)
			{
				FilterHeroUDList.Emplace(HeroUD);
			}
		}
	}
	InHeroUDList = FilterHeroUDList;
}

void URInventory::HeroChangeFilter(TArray<FString>& InHeroUDList, EHeroListFilterEnum InFilter, EHeroInventoryType eHeroType /*= EHeroInventoryType::VE_TypeHero*/, bool bIsDescending)
{
	HeroChangeClassFilter(InHeroUDList, EClassTypeEnum::VE_ClassTypeMax, eHeroType);
	switch (InFilter)
	{
		case EHeroListFilterEnum::VE_All:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Id, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_HaveHero:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Level, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_Tanker:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Level, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_RangeDealer:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Level, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_MeleeDealer:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Level, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_MagicDealer:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Level, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_Supporter:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Level, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_AttackPower:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_AttackPower, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_Level:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Level, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_Grade:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Grade, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_Damage:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Damage, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_PhysicalDefense:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_PhysicalDefense, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_MagicDefense:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_MagicDefense, bIsDescending);
			break;
		case EHeroListFilterEnum::VE_Health:
			SetHeroSort(InHeroUDList, EFilterSortTypeEnum::VE_Health, bIsDescending);
			break;
	}
}

FItemTableInfo* URInventory::GetItemTableData(const FString& ItemHashKey)
{
	FITEM* ItemInvenData = GetItemDataWithUD(ItemHashKey);
	if (ItemInvenData == nullptr)
		return nullptr;

	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemInvenData->itemId);
	if (ItemTableInfo == nullptr)
		return nullptr;

	return ItemTableInfo;
}

void URInventory::GetItemKeys(TArray<FString>& ItemKeys)
{
	ItemInventoryMap.GenerateKeyArray(ItemKeys);
}

int32 URInventory::GetItemCount()
{
	return ItemInventoryMap.Num();
}

void URInventory::GetItemEquipmentKeys(TArray<FString>& ItemKeys, EItemTypeEnum ItemType, EItemEquipPositionEnum EquipPosition, EItemEquipTypeEnum EquipType)
{
	ItemKeys.Reset(ItemInventoryMap.Num());
	for (auto& Elem : ItemInventoryMap)
	{
		FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(Elem.Value.itemId);
		if (ItemTableInfo == nullptr)
			continue;

		if (ItemType != EItemTypeEnum::VE_None && ItemTableInfo->ItemType != ItemType)
			continue;

		if (EquipPosition != EItemEquipPositionEnum::VE_Max && ItemTableInfo->EquipPosition != EquipPosition)
			continue;

		if (EquipType != EItemEquipTypeEnum::VE_None && ItemTableInfo->EquipType != EquipType)
			continue;

		ItemKeys.Emplace(Elem.Key);
	}
}


void URInventory::GetWeaponKeys(TArray<FString>& ItemKeys, EAttackTypeEnum AttackType /*= EAttackTypeEnum::VE_None*/)
{
	ItemKeys.Reset(ItemInventoryMap.Num());
	for (auto& Elem : ItemInventoryMap)
	{
		FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(Elem.Value.itemId);
		if (ItemTableInfo == nullptr)
			continue;

		if (ItemTableInfo->ItemType != EItemTypeEnum::VE_EquipItem)
			continue;

		if (ItemTableInfo->EquipPosition != EItemEquipPositionEnum::VE_Weapon)
			continue;

		if (AttackType != EAttackTypeEnum::VE_None && ItemTableInfo->EquipLimit != AttackType)
			continue;

		ItemKeys.Emplace(Elem.Key);
	}
}

void URInventory::GetSellableItems(TArray<FString>& ItemKeys, EItemTypeEnum ItemType /*= EItemTypeEnum::VE_None*/, EItemEquipPositionEnum EquipPosition /*= EItemEquipPositionEnum::VE_Max*/, EItemEquipTypeEnum EquipType /*= EItemEquipTypeEnum::VE_None*/)
{
	ItemKeys.Reset(ItemInventoryMap.Num());
	for (auto& Elem : ItemInventoryMap)
	{
		FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(Elem.Value.itemId);
		if (ItemTableInfo == nullptr)
			continue;

		if (ItemType != EItemTypeEnum::VE_None && ItemTableInfo->ItemType != ItemType)
			continue;

		if (EquipPosition != EItemEquipPositionEnum::VE_Max && ItemTableInfo->EquipPosition != EquipPosition)
			continue;

		if (EquipType != EItemEquipTypeEnum::VE_None && ItemTableInfo->EquipType != EquipType)
			continue;

		if (Elem.Value.locked)
			continue;

		if (!Elem.Value.equipedHeroUD.IsEmpty())
			continue;

		ItemKeys.Emplace(Elem.Key);
	}
}

void URInventory::GetUpgradableItems(TArray<FString>& ItemKeys, FItemTableInfo Itemtable)
{
	ItemKeys.Reset(ItemInventoryMap.Num());
	for (auto& Elem : ItemInventoryMap)
	{
		FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(Elem.Value.itemId);
		if (ItemTableInfo == nullptr)
			continue;

		if (Elem.Value.locked)
			continue;

		if (!Elem.Value.equipedHeroUD.IsEmpty())
			continue;

		if (ItemTableInfo->ItemLevel != Itemtable.ItemLevel)
			continue;

		if (ItemTableInfo->ItemGrade != Itemtable.ItemGrade)
			continue;

		ItemKeys.Emplace(Elem.Key);
	}
}

void URInventory::ChangeItemEquipment(const FString& ItemHashKey, const FString& HeroHashKey)
{
	FITEM* ItemInvenData = GetItemDataWithUD(ItemHashKey);
	if (ItemInvenData == nullptr)
		return;

	ItemInvenData->equipedHeroUD = HeroHashKey;
}

float URInventory::GetItemValue(FName tableKey, int32 enchantLevel, int32 InItemLevel /*= 1*/)
{
	return 0.0;
}