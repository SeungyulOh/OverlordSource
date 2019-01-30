// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Table/LevelAppliedTables/WorkedCharacterTableInfo.h"
#include "Table/LevelAppliedTables/WorkedItemTableInfo.h"
#include "Network/PacketDataStructures.h"
#include "Global/EventManager.h"
#include "Table/ItemTableInfo.h"
#include "RInventory.generated.h"

struct FWalletTableInfo;

//Skill
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_OnSkillLevelUp);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnHeroLevelUp, FString, HeroHashKey );


UCLASS()
class PROJECTR_API URInventory : public UObject
{
	GENERATED_BODY()
	
public:
	void								Clear(void);
	void								InitInventory(FINVENTORY& inventory);
	void								SetDecks(FDECKS InDecks);

	void StartRefillCheck();

	//	Currency Control Function
	void								CheckWalletRefill(); //check every one minute
	int32								GetRefillLimit(FWalletTableInfo* InWalletInfo);

	int32								GetGold() const;
	int32								GetGem() const;
	int32								GetStamina() const;
	int32								GetStaminaPVP() const;
	int32								GetHeroRune() const;
	int32								GetPVPToken() const;
	int32								GetArenaDailyBonusToken() const;
	int32								GetArenaDefenderResetToken() const;
	int32								GetMIRROR_DEFENDERRESET_FREE() const;
	int32								GetMIRROR_ENTRANCE() const;
	int32								GetSummonTicket01() const;
	int32								GetSummonTicket02() const;
	int32								GetMileage() const;
	int32								GetFloorStoreTicket() const;
	int32								GetFloorStoreResetToken() const;
	int32								GetFloorStoreResetTokenGem() const;
	int32								GetMISSION_ADD_SCROLL() const;
	int32								GetHeroUpgradeMaterial() const;
	int32								GetS_TOWERBOSS_F1() const;

	FORCEINLINE int32					GetMaxHeroInventoryCount() { return INVENTORY_HERO_MAXCOUNT; }
	FORCEINLINE int32					GetCurrentHeroInventoryCount() { return INVENTORY_HERO_CURRENTCOUNT; }
	FORCEINLINE int32					GetMaxItemInventoryCount() { return INVENTORY_ITEM_MAXCOUNT; }
	FORCEINLINE int32					GetCurrentItemInventoryCount() { return INVENTORY_ITEM_CURRENTCOUNT; }


	TArray<int32>*						GetRune();
	FDECKS								GetHeroDeck();
	bool								IsEmptyDefenceDeck();

	int32 GetCurrencyWithKey(FName WalletKey);
	int32 GetRuneWithGrade(int32 RuneGrade);

	FName RuneGradeToWalletKey(int32 RuneGrade);

	void								UpdateInventory(FINVENTORY_UPDATE& inven_update, bool bCheckHeroUpdate = true);
	//////////////////////////////////////////////////////////////////////////////////////////

	//	Item
	void								UpdateItemData(TArray<FITEM>& Items);
	void								UpdateItemData(FITEM& ItemEntity, bool InEventBroadcast = true);
	//void								RemoveItemData(const FString& ItemHashKey);

	FITEM*								GetItemDataWithUD(const FString& ItemHashKey);
	FITEM*								GetItemDataWithID(const FName& ItemID);

	FItemTableInfo*						GetItemTableData(const FString& ItemHashKey);
	int32								GetItemCount();
	UFUNCTION(BlueprintCallable, Category = "ItemInventory")
	void								GetItemKeys(TArray<FString>& ItemKeys);

	//UFUNCTION(BlueprintCallable, Category = "ItemInventory")
	//void								GetItemInvenKeys(TArray<FString>& ItemKeys, EItemInventoryTypeEnum InventoryType, EItemInvenTypeEnum ItemInvenType, bool bOpenBatchRegisterWidget = false);

	UFUNCTION(BlueprintCallable, Category = "ItemInventory")
	void								GetItemEquipmentKeys(TArray<FString>& ItemKeys,
			EItemTypeEnum ItemType = EItemTypeEnum::VE_None, EItemEquipPositionEnum EquipPosition = EItemEquipPositionEnum::VE_Max,
			EItemEquipTypeEnum EquipType = EItemEquipTypeEnum::VE_None);

	UFUNCTION(BlueprintCallable, Category = "ItemInventory")
		void								GetWeaponKeys(TArray<FString>& ItemKeys, EAttackTypeEnum AttackType = EAttackTypeEnum::VE_None);

	UFUNCTION(BlueprintCallable, Category = "ItemInventory")
		void								GetSellableItems(TArray<FString>& ItemKeys,
			EItemTypeEnum ItemType = EItemTypeEnum::VE_None, EItemEquipPositionEnum EquipPosition = EItemEquipPositionEnum::VE_Max,
			EItemEquipTypeEnum EquipType = EItemEquipTypeEnum::VE_None);

	UFUNCTION(BlueprintCallable, Category = "ItemInventory")
		void								GetUpgradableItems(TArray<FString>& ItemKeys, FItemTableInfo Itemtable);

	//UFUNCTION(BlueprintCallable, Category = "ItemInventory")
	//void								GetItemEquipmentValues(TArray<FITEM>& ItemValues, EItemInventoryTypeEnum InventoryType = EItemInventoryTypeEnum::VE_Normal,
	//		EItemTypeEnum ItemType = EItemTypeEnum::VE_None, EItemEquipPositionEnum EquipPosition = EItemEquipPositionEnum::VE_None,
	//		EItemEquipTypeEnum EquipType = EItemEquipTypeEnum::VE_None);


	FORCEINLINE void					SetItemSortDescType(bool InType) { ItemInventorySortDescType = InType; }
	FORCEINLINE void					SetItemSortStandType(int32 InType) { ItemInventorySortStandType = InType; }
	FORCEINLINE void					SetItemPrevEnchantLevel(int32 InType) { ItemPrevEnchantLevel = InType; }
	FORCEINLINE void					SetItemIsSpecialEnchant(bool InState) { IsSpecialEnchant = InState; }
	FORCEINLINE void					SetItemIsProtectionEnchant(bool InState) { IsProtectionEnchant = InState; }
	FORCEINLINE void					SetItemSelectID(FName& InSelectID) { SelectID = InSelectID; }
	FORCEINLINE void					SetItemCurrentIgnoreUD(FString& InItemUD) { CurrentIgnoreUD = InItemUD; }

	FORCEINLINE bool					GetItemSortDescType() { return ItemInventorySortDescType; }
	FORCEINLINE int32					GetItemSortStandType() { return ItemInventorySortStandType; }
	FORCEINLINE int32					GetItemPrevEnchantLevel() { return ItemPrevEnchantLevel; }
	FORCEINLINE bool					GetItemIsSpecialEnchant() { return IsSpecialEnchant; }
	FORCEINLINE bool					GetItemIsProtectionEnchant() { return IsProtectionEnchant; }

	//bool								IsValidItemInventoryType(EItemInventoryTypeEnum InventoryType, FItemTableInfo* ItemTableInfo);
	//bool								IsValidItemInvenType(EItemInvenTypeEnum ItemInvenType, FItemTableInfo* ItemTableInfo);
	//bool								IsValidItemExFilter(EItemInventoryTypeEnum InventoryType, FItemTableInfo* ItemTableInfo, FITEM& Item);

	void								ChangeItemEquipment(const FString& ItemHashKey, const FString& HeroHashKey);
	static float						GetItemValue(FName tableKey, int32 enchantLevel, int32 InItemLevel = 1);
	//////////////////////////////////////////////////////////////////////////////////////////

	//	Hero
	void								UpdateHeroData(TArray<FHERO>& Heros, bool bCheckAttackPower = true);
	void								UpdateHeroData(FHERO& HeroEntity, bool InvokeDelegate = true, bool bCheckAttackPower = true);

	void								RemoveHeroData(const FString& HeroHashKey);

	//		왠만하면 UD로 검색하기
	//		ID는 UD보다 속도가 떨어짐
	FHERO*								GetHeroWithUD(const FString& HeroHashKey);
	FHERO*								GetHeroWithID(const FName& HeroID);

	UFUNCTION(BlueprintCallable, Category = "HeroInventory")
	bool								GetHeroAndTableData(const FString& HeroUD, FHERO& outData, FWorkedCharacterTableInfo& outTableData, bool InNextUpgrade = false, int32 InNextLevel = 0);
	UFUNCTION(BlueprintCallable, Category = "HeroInventory")
	bool								GetSkillDataFromHeroUD(const FString& HeroUD, TArray<FHERO_SKILL>& outData);
	bool								GetHerosWithID(const FName& HeroID, FString& HeroHashKey);
	int32 GetHeroCountInName();
	FORCEINLINE int32					GetHeroCount() { return HeroInventoryMap.Num(); }
	int32								GetHeroInventoryCount(bool InIsHero/* = true*/);
	UFUNCTION(BlueprintCallable, Category = "HeroInventory")
	void								GetHeroUDs(TArray<FString>& InHeroUDList, EClassTypeEnum ClassType = EClassTypeEnum::VE_ClassTypeMax, EHeroInventoryType eHeroType = EHeroInventoryType::VE_TypeHero);
	void								GetHeroUDArray(TArray<FString>& InHeroUDList);
	void								GetSpiritUDs(TArray<FString>& InHeroUDList, int32 ListCountMax, bool InbExceptforLock = false);
	bool								GetNewHeroData(FString& HeroUD);

	void								GetProtectedHeros(TArray<FString>& outHeroUDs);

	FString								ChangeItemEquipment(const FString& HeroUD, EItemEquipPositionEnum Position, const FString& ItemHashKey);
	bool								IsHaveNewHeroData();
	void								TestNewHeroData(FString& HeroUD);
	
	void								GetHerosID(const FName& HeroID, TArray<FString>& HeroHashKey);
	void								HeroChangeClassFilter(TArray<FString>& InHeroUDList, EClassTypeEnum InClass, EHeroInventoryType eHeroType = EHeroInventoryType::VE_TypeHero);
	void								HeroChangeFilter(TArray<FString>& InHeroUDList, EHeroListFilterEnum InFilter, EHeroInventoryType eHeroType = EHeroInventoryType::VE_TypeHero, bool bIsDescending = true);
	void								SetHeroSort(TArray<FString>& InHeroUDList, EFilterSortTypeEnum InSortType, bool isDescending);
	void								SetItemSort(TArray<FString>& ItemList, EFilterSortTypeEnum InSortType, bool isDescending);
	int32 ItemUDToCreationNumber(const FString& InItemUD);
	void								GetHerosWithGrade(TArray<FString>& InHeroUDList, int32 InGrade, EHeroInventoryType InventoryType, bool IsLockCollect = true);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = URInventory)
		FDelegate_OnOneParamInt32			OnClickInvenSortingButton;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = URInventory)
		FDelegate_NoParam			OnClickInvenFilterButton;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = URInventory)
		FDelegate_NoParam			CloseFilterPopup;
	//////////////////////////////////////////////////////////////////////////////////////////

	//	Skills
	void								CheckSkillActivate(const FString& HeroUD);
	//////////////////////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* Bag Capacity                                                         */
	/************************************************************************/

	/** Block reward-able contents if true. ... 오버로드_인벤토리가꽉찬경우.xlsx 장비 인벤토리가 꽉찬 경우 */
	bool								IsBagCapacityExceeded();

	int32								GetOwnedItemsCount();
	int32								GetBagCapacity();
	bool								CanBagBeExtendedFurther();
	int32								NextBagExtensionBluestoneCost();
	private:
		bool GetFlagWithCompareValue(int32 ValueA, int32 ValueB, bool Flag);

public:
	UPROPERTY()
	TMap<FString, FString>				NewHeroMap;
	UPROPERTY()
	TArray<FString>						NewHeroArray;
	UPROPERTY()
	TArray<FName>						RuneIDArray;

	TArray<FName>						RuneWallet;
	TArray<FName>						FloorCoinWallet;

	//UPROPERTY()
	TMap< int32, TArray<FName> >		RefillList;
	TMap<int32, int32>					ResetTimeList;
	UPROPERTY()
	FTimerHandle						RefillHandler;

	//filter check data
	bool isDescending = true;
	EHeroListFilterEnum sortFilter = EHeroListFilterEnum::VE_AttackPower;
	TArray<EClassTypeEnum> classFilter;
	TArray<int32> gradeFilter;
	TArray<EContinentTypeEnum> continentFilter;

	//item filter
	void Filter_Class(TArray<FString>& ItemList);
	void Filter_Grade(TArray<FString>& ItemList);
	void Filter_Tier(TArray<FString>& ItemList);
	void Filter_EquipType(TArray<FString>& ItemList);
	void Filter_EnchantType(TArray<FString>& ItemList);
	void Filter_UseStateType(TArray<FString>& ItemList);

	TArray<EAttackTypeEnum> AttackTypeFilter;
	TArray<EItemGradeEnum> itemGradeFilter;
	TArray<int32> itemTierFilter;
	TArray<EItemEquipTypeEnum> EquipTypeFilter;
	TArray<EItemEnchantStateEnum> EnchantStateFilter;
	TArray<EItemUseStateEnum> UseStateFilter;

	void								SetHeroData(TArray<FHERO>& Heros);
	void								SetItemData(TArray<FITEM>& Items);
protected:
	void								InitCurrency(FINVENTORY& inventory);
	void								UpdateCurrency(FINVENTORY_UPDATE& update_inven);
	void								UpdateCurrency();
	void								DeleteHeroData(TArray<FString>& heros_delete);
	void								DeleteItemData(TArray<FString>& items_delete);
private:
	int32								GOLD;
	int32								GEM;
	int32								STAMINA;
	int32								STAMINA_PVP;
	int32								HERO_RUNE;
	int32								PVP_TOKEN;
 	int32								ARENA_DAILYBONUSTOKEN;
	int32								ARENA_DEFENDERRESETTOKEN;
	int32								MIRROR_DEFENDERRESET_FREE;
	int32								MIRROR_ENTRANCE;
	int32								SUMMONTICKET01;
	int32								SUMMONTICKET02;
	int32								MILEAGE;
	int32								FLOORSTORE_TICKET;
	int32								FLOORSTORE_RESETTOKEN;
	int32								FLOORSTORE_RESETTOKEN_GEM;
	int32								MISSION_ADD_SCROLL;
	int32								HEROUPGRADEMATERIAL;

	int32								INVENTORY_HERO_MAXCOUNT;
	int32								INVENTORY_HERO_CURRENTCOUNT;
	int32								INVENTORY_ITEM_MAXCOUNT;
	int32								INVENTORY_ITEM_CURRENTCOUNT;
	int32								S_TOWERBOSS_F1;
// 	int32								PVP_TOKEN;
	FDECKS								decks;

	TArray<int32>						RuneInventory;
	TArray<int32>						FloorCoinInventory;
	
	UPROPERTY()
	TMap<FString, FHERO>				HeroInventoryMap;



	int32 GetCurrencyData(FWalletTableInfo* walletInfo);
	void								InitRuneInventory();
	void								UpdateRuneInventory();

	void								InitFloorCoinInventory();
	void								UpdateFloorCoinInventory();
	int32								GetFloorCoin(FName InCurrencyType);
	
	
	//	Item
public:
	UPROPERTY()
	TMap<FString, FString>				NewItemMap;
	bool								bOpenBatchRegisterWidget = false;
	bool								bConsumableWidget = false;
	bool								IsTempManageState = false;
private:
	UPROPERTY()
	TArray<int32>						Currency1;
	UPROPERTY()
	TArray<int32>						Currency2;
	UPROPERTY()
	TArray<int32>						Currency3;

	UPROPERTY()
	int32								ItemBaseCount = 100;
	UPROPERTY()
	int32								ItemPrevEnchantLevel = 0;
	UPROPERTY()
	FName								SelectID;
	UPROPERTY()
	FString								CurrentIgnoreUD;
	UPROPERTY()
	FString								CurrentAddUD;
	bool								ItemInventorySortDescType = true;
	int32								ItemInventorySortStandType = 0;
	bool								IsSpecialEnchant = false;
	bool								IsProtectionEnchant = false;
	UPROPERTY()
	TMap<FString, FITEM>				ItemInventoryMap;


	
public:
	int32 GetCurrency(FName InCurrencyType);
};
