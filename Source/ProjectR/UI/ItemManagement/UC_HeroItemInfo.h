// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UC_GenericPopupContent.h"
#include "SharedConstants/GlobalConstants.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Components/TextBlock.h"
#include "Table/ItemTableInfo.h"
#include "UC_ItemInventoryBase.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UC_HeroItemInfo.generated.h"

class UUC_GenericItemIcon;
class URScrollView;
class UButton;
class UWidgetSwitcher;
class ULocalizingTextBlock;
class UUC_ItemInfo_ItemStat;
/**
* Button with customized look for disabled state. A component of UC_HeroItemInfo.
*/
UCLASS()
class PROJECTR_API UUC_ItemInfo_Button : public URBaseWidget
{
	GENERATED_BODY()


public:
	void NativePreConstruct() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// BP Interface

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClicked);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UUC_ItemInfo_Button")
	FOnClicked OnClicked;

	//// Interface
	UFUNCTION(BlueprintCallable, Category = "UUC_ItemInfo_Button")
	void SetDisabled(bool bDisabled);

	//// Delegates

	UFUNCTION()
	void OnButtonEnabledClicked();
	

	//// BP Configs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_Button")
	FOptionalUIStringKey ButtonLabel;

	//// BP Widgets
	/** 0: enabled, 1: disabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_Button|Ref")
	UWidgetSwitcher* Switcher_Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_Button|Ref")
	UButton* Button_Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_Button|Ref")
	ULocalizingTextBlock* Text_LabelEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_Button|Ref")
	ULocalizingTextBlock* Text_LabelDisabled;
};

/**
* Widget for displaying one item stat. A component of UC_HeroItemInfo.
*/
UCLASS()
class PROJECTR_API UUC_ItemInfo_ItemStat : public URBaseWidget
{
	GENERATED_BODY()


public:
	void Refresh(EItemStatOption ItemStatOption, int32 Value, bool IsAdditionalStat);

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_ItemStat|Ref")
	ULocalizingTextBlock* Text_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_ItemStat|Ref")
	UTextBlock* Text_Value;
};

/**
* Widget for displaying one item stat. A component of UC_HeroItemInfo.
*/
UCLASS()
class PROJECTR_API UUC_ItemInfo_RuneOption : public URBaseWidget
{
	GENERATED_BODY()


public:
	void RefreshEmpty();
	void RefreshSocketed(FName ImageKey, EItemStatOption ItemStatOption, int32 Value);

	//// BP Widgets
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_RuneOption|Ref")
	UImage* Image_RuneIcon;
	
	/** 0: socketed, 1: empty */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_RuneOption|Ref")
	UWidgetSwitcher* Switcher_SocketState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_RuneOption|Ref")
	ULocalizingTextBlock* Text_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemInfo_RuneOption|Ref")
	UTextBlock* Text_Value;
};

/*
* HeroItemInfo
* 오버로드_장비아이템_변경안.docx.3 <아이템 상세 정보 이미지>
* 아이템 UI_김현주(7).pdf.III. 영웅 관리 - 장비 아이템 상세 정보
*
* 아이템 UI_가방화면_김현주.pdf.Ⅲ. 가방 - 장비 아이템 상세 정보
*/
UCLASS()
class PROJECTR_API UUC_HeroItemInfo : public UUC_GenericPopupContent
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	void InvalidateData() override;

	FString ItemUD;

	// As opposed to being opened in UP_Bag
	// (design) differences between being opened in UP_Bag and HeroManager:
	// HeroManager: does not show Equipment Items that the current hero is equipped with
	// HeroManager: show equip/unequip buttons for Equipment Items
	bool IsBagMode = false;
	bool bLeft = false;

	// of the hero manager.
	FString CurrentHeroUD;

	bool EquippedByCurrentHero = false;

	//// BP Config
	/**
	* "{floor}층에 도달하면 아래의 효과가 발동합니다."
	* "The effect below is activated once you reach the floor {floor}."
	* - compare: "The effect below is activated once you reach the 9th floor." We might need internationalized ordinal numbers?
	* - how about the languages Arabic or Hebrew?
	* bloody hell
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo")
	FOptionalUIStringKey TowerOptionGuideText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo")
	FSlateColor Color_TowerOptionLabelTextWhenInactive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo")
	FSlateColor Color_TowerOptionDescriptionTextWhenInactive;

	///

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UUC_GenericItemIcon* ItemIcon;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	//UImage* Image_EquipType;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	//UTextBlock* Text_NamePrefix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	ULocalizingTextBlock* Text_ItemName;

	/** actually EquipPosition */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	ULocalizingTextBlock* Text_EquipType;

	/** 0: currently unlocked, 1: locked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UWidgetSwitcher* Switcher_Lock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UButton* Button_Lock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UButton* Button_Unlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UButton* Button_Exit;
	

	// Power
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UPanelWidget* Panel_Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	ULocalizingTextBlock* Text_PowerLabel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UTextBlock* Text_PowerValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UTextBlock* Text_Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UTextBlock* Text_Enchant;

	// Basic stats and substats
	// 4 of them
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	TArray<UUC_ItemInfo_ItemStat*> BasicStats;
	// 2 of them
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	TArray<UUC_ItemInfo_ItemStat*> Substats;

	// Rest of the options
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UScrollBox* ScrollBox_Options;

	// Additional Item Options
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UPanelWidget* Panel_AdditionalOptions;
	// 4 of them
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	TArray<UUC_ItemInfo_ItemStat*> AdditionalStats;
	
	// Special Item Options
	/** 0: RuneOptions, 1: TowerOptions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UWidgetSwitcher* Switcher_SpecialOptions;

	// Rune Options
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	TArray<UUC_ItemInfo_RuneOption*> RuneStats;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UPanelWidget* Panel_RuneWord;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	ULocalizingTextBlock* Text_RuneWordName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UTextBlock* Text_RuneWordDescription;

	// Tower Options
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UTextBlock* Text_TowerOptionLabel;
	/** "9층에 도전하면 아래의 효과가 발동합니다." */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UTextBlock* Text_TowerOptionGuide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UTextBlock* Text_TowerOptionDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	URenewal_BaseHeroIcon* EquipHeroIcon;

	// Side buttons
	// interaction driven by BP

	/** 0: Unique Equipment, 1: Unique Accessary, 2: Tower Equipment, 3: rest, (invisible): Tower Accessary */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UWidgetSwitcher* Switcher_SideButtons;

	/** ItemUpgrade button is disabled if the item is locked or currently equipped by hero */
	UFUNCTION(BlueprintImplementableEvent, Category = "UC_HeroItemInfo")
	void SetButtonUpgradeIsEnabled(bool InEnabled);
	UFUNCTION(BlueprintImplementableEvent, Category = "UC_HeroItemInfo")
	void SetButtonEnchantIsEnabled(bool InEnabled);
	UFUNCTION(BlueprintImplementableEvent, Category = "UC_HeroItemInfo")
	void SetButtonLevelUpIsEnabled(bool InEnabled);
	
	/** ItemOptionReset button is disabled if the item is locked or currently equipped by hero. Not sure where in the design document */
	

	UFUNCTION(BlueprintCallable, Category = "UC_HeroItemInfo")
	void OnButtonEnchantClicked();
	UFUNCTION(BlueprintCallable, Category = "UC_HeroItemInfo")
	void OnButtonLevelUpClicked();
	UFUNCTION(BlueprintCallable, Category = "UC_HeroItemInfo")
	void OnButtonRuneSetting();
	UFUNCTION(BlueprintCallable, Category = "UC_HeroItemInfo")
	void OnButtonUpgradeClicked();
	UFUNCTION(BlueprintCallable, Category = "UC_HeroItemInfo")
	void OnButtonOptionResetClicked();
	UFUNCTION(BlueprintCallable, Category = "UC_HeroItemInfo")
	void OnButtonExitClicked();


	// Bottom panel buttons
	/** 0: Equip, 1: Unequip, 2: Close */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UButton* Button_Equip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UButton* Button_Unequip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroItemInfo|Ref")
	UButton* Button_Close;

	//// Delegate functions

	UFUNCTION()
	void OnButtonCloseClicked();
	UFUNCTION()
	void OnButtonEquipClicked();
	UFUNCTION()
	void OnButtonUnequipClicked();
	
	UFUNCTION()
	void OnButtonLockClicked();
	UFUNCTION()
	void OnButtonUnlockClicked();
	
	UFUNCTION()
	void OnInventoryHeroEquipRp(bool bSuccess);
	UFUNCTION()
	void OnInventoryHeroUnequipRp(bool bSuccess);

	// coming from item lock, enchant, level up, ... ?
	UFUNCTION()
	void OnInventoryItemHeroLock(bool bSuccess);

	FOnItemInventoryUpdated OnItemInventoryUpdatedDelegate;
	
	// temp
	static FText EquipTypeToString(EItemEquipTypeEnum EquipType);

private:
	// InvalidateData helpers
	void InvalidateBasicStats(const TArray<int32>& BasicOptions);
	void InvalidateSubstats(const TArray<int32>& BasicOptions);
	void InvalidateAdditionalOptions(const FItemTableInfo* ItemInfo, const FITEM* ItemData);
	void InvalidateRuneOptions(const TArray<FName>& Runes, const FItemTableInfo* ItemInfo);
	
	// ?
	static FName ItemStatOptionEnumToHeroPropertyUIStringKey(EItemStatOption ItemStatOption);

	// saved on NativeConstruct
	FSlateColor Color_TowerOptionLabelTextWhenActive;
	FSlateColor Color_TowerOptionDescriptionTextWhenActive;
};
