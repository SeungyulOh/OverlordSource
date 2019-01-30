// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"

#include "Table/LevelGrowth.h"
#include "Table/CharacterTableInfo.h"
#include "UI/ItemManagement/UC_ItemInventoryBase.h"

#include "UC_HeroBaseInfo.generated.h"


class UUC_HeroInfoStat;
class UUC_Hero_Skill_Icon;
class UUC_HeroLevelUp_Popup;
class UUC_HeroUpgrade_Popup;
class UUC_HeroEnchant_Popup;
class ACharacterSetCameraActor;
class UCharacterBattleInfo;
class URGameInstance;
struct FCharacterTableInfo;
class UCheckBox_Radio;
class UWidgetSwitcher;
class URScrollView;
class UUC_Bag_Tab;
class UHorizontalBox_RadioGroup;
class URScrollItem;
class UUC_GenericItemIcon;

/**
 * BP: UC_Hero_Info
 */
UCLASS()
class PROJECTR_API UUC_HeroBaseInfo : public UUC_ItemInventoryBase
{
	GENERATED_BODY()
	

public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;
	
	void InvalidateData() override;

	UFUNCTION()
	void InvalidateByHeroUD(const FString& InHeroUD);

	UFUNCTION()
	void UpdateHeroInfo();

//	FORCEINLINE UCharacterBattleInfo* GetCharacterBattleInfo() { return UICharacterBattleInfo; }

	UFUNCTION(BlueprintCallable, Category = UUC_HeroBaseInfo)
	void OnClick_OpenHeroDetailStat();

	UFUNCTION(BlueprintCallable, Category = UUC_HeroBaseInfo)
	void OnClick_GoHeroShop();

	UFUNCTION(BlueprintCallable, Category = UUC_HeroBaseInfo)
	void OnClick_LevelUp();

	UFUNCTION(BlueprintCallable, Category = UUC_HeroBaseInfo)
	void OnClick_GradeUp();

	UFUNCTION(BlueprintCallable, Category = UUC_HeroBaseInfo)
	void OnClick_Enchant();

	UFUNCTION(BlueprintCallable, Category = UUC_HeroBaseInfo)
	void OnClick_HeroBaseInfo_FilterOn();
	
	UFUNCTION(BlueprintCallable, Category = UUC_HeroBaseInfo)
	void OnClick_HeroBaseInfo_Skills();

	//UFUNCTION()
	//void OnRefreshHeroLevelUp(FString InHeroHashKey);

public:
	/*
	Blueprint Widget Component
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_HeroName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_HeroNickName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_HeroSummary1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_Desc1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_HeroLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_HeroMaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_NeedHeroRune1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_NeedGold1;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	//UTextBlock*			Text_Exp_Percent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	ULocalizingTextBlock*			ULocalizingTextCombatStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	USlider*			Exp_Slider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UImage*				Image_HeroJob;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_HeroJob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_HeroRuneCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UImage*				Image_WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UWidgetSwitcher*	WidgetSwitcher_Toggle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UWidgetSwitcher*	WidgetSwitcher_Level;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	//UProgressBar*		Progressbar_Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UTextBlock*			Text_EquipArmorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UImage*				Image_EquipArmorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroBaseInfo)
	UWidgetSwitcher*	WidgetSwitcher_Growth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UButton*			Button_LevelUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UButton*			Button_Upgrade;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UButton*			Button_Enchant;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UButton*			Button_DetailStat_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UButton*			Button_Awaken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UButton*			Button_HeroShop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UButton*			Button_FilterOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UWidgetSwitcher*	Switcher_FilterOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UButton*			Button_Skills;

	// Hero basic stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UUC_HeroInfoStat* InfoStat_PhysicalAttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UUC_HeroInfoStat* InfoStat_MagicAttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UUC_HeroInfoStat* InfoStat_Armor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UUC_HeroInfoStat* InfoStat_MagicResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UUC_HeroInfoStat* InfoStat_HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UWidgetSwitcher*		WidgetSwitcher_Star = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	TArray<UImage*>							HeroStarArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	TArray<UUC_Hero_Skill_Icon*>			HeroSkillIcons;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
// 	UUC_HeroLevelUp_Popup*					HeroLevelUpPopup;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
// 	UUC_HeroUpgrade_Popup*					HeroUpgradePopup;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
// 	UUC_HeroEnchant_Popup*					HeroEnchantPopup;
	
	//// ItemInventory

	/** 0 for ItemInventory, 1 for Hero Info */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroBaseInfo|Ref")
	UWidgetSwitcher* Switcher_ItemInventory;

	//// Delegates for UP_Mediator_HeroManagement to hook up on

	/** cue to invalidate */
	DECLARE_MULTICAST_DELEGATE(FOnItemInventoryUpdated);
	FOnItemInventoryUpdated OnItemInventoryUpdatedDelegate;

	/** request to display right-side ItemInfo for equipment item */
	DECLARE_DELEGATE_OneParam(FOnEquipmentItemSelected, FString /* ItemUD */);
	FOnEquipmentItemSelected OnEquipmentItemSelected;

	//// ItemInventoryBase interface
	
	/*
	* Tabs come in order
	* * Hero Info
	* * Weapons
	* * Defense
	* * Accessories
	* * Runes
	* * Apparels
	*/

	void OnItemInventoryUpdated(EItemInventoryUpdate Result, bool bWidgetFinished, URScrollItem* UpdatedScrollItem, EWidgetBluePrintEnum WidgetReturnedFrom) override;

protected:
	void OnTabCheckStateChanged(bool bIsChecked, int32 Index) override;

	// overrides
	void OnInventoryScrollViewSelectedItem_EquipmentItem(UUC_ItemInventory_ScrollItem* SelectedItem) override;

	EItemInventoryEnum TabIndexToInventoryEnum(int32 Index) override;

	bool IsIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData) override;

private:
	void Refresh();	
	
	void Refresh_HeroName();
	void Refresh_CombatStat();	
	void Refresh_HeroLevel();	
	//void Refresh_HeroExp();
	//void Refresh_HeroExpSlide();
	void Refresh_HeroClassTypeImage();
	void Refresh_HeroWeaponTypeImage();
	//void Refresh_HeroExpProgressBar();
	void Refresh_HeroWeaponType();
	void Refresh_HeroRuneCount();
	void Refresh_HeroClassType();
	void Refresh_HeroDetail();
	void Refresh_HeroSkill();
	void ClearData();

	void SetDetail(ECharacterPropertyTypeEnum InPropertyType, UUC_HeroInfoStat* HeroInfoStat);

	FText GetClassTypeText(EItemEquipArmorTypeEnum InType);

	bool IsShowingItemInventory();

private:
	UPROPERTY()
	URGameInstance*	RGameInstance = nullptr;
	UPROPERTY()
	FString					CurrentHeroUD;
	/*
	Data Info
	*/
// 	UPROPERTY()
// 	UCharacterBattleInfo*	UICharacterBattleInfo = nullptr;
	FCharacterTableInfo*	UICharacterTableInfo = nullptr;
};
