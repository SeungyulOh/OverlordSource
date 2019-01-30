//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/Common/Popup/UP_Popup_Warning_Splash.h"
#include "UC_Popup_HeroBreakUp.h"
#include "UI/Common/Popup/UC_Popup_HeroInfo_Lock.h"
#include "UI/HeroInfo/UC_Popup_HeroBreakUp_Splash.h"
#include "UI/HeroManagement/UC_HeroFilterButton.h"
#include "UI/HeroManagement/Popup/UC_Popup_SoulInfo.h"
#include "Global/EventManager.h"
#include "UI/Common/HeroScrollItemData.h"
#include "UP_HeroInventory.generated.h"


/**
* 영웅 인벤토리
* HeroInventoryScrollView hosts UC_HeroInventory_ScrollItem and HeroInventory_ScrollData
*/

class UImage;
class UWidgetSwitcher;
class UTextBlock;
class URScrollView;
class URScrollItem;
class UCheckBox_Radio;
//Control Object Type Enum

UENUM(BlueprintType)
enum class EHeroInventoryObject : uint8
{
	VE_Hero				UMETA("Hero"),
	VE_Spirit			UMETA("Spirit"),
};

//Hero Function Enum
UENUM(BlueprintType)
enum class EHeroInventoryFunction : uint8 {
	VE_View				UMETA("HeroView"),
	VE_Grind			UMETA("HeroGrind"),
	VE_Lock				UMETA("HeroLock"),
	VE_Select			UMETA("HeroSelect"),
};

UCLASS()
class PROJECTR_API  UUC_Hero_Filter_Check : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	// UMG Widget Handler
	UFUNCTION() 
		void OnClicked_Button_Reset();
	UFUNCTION()	
		void OnClicked_Button_Close();
	UFUNCTION()
		void SetHeroFilterMenu();
	UFUNCTION()
		void SetSoulFilterMenu();
	UFUNCTION()
		void SetFilterMenuReset();
	UFUNCTION()	
	void FilteringAllConditions(bool isChecked);
	//
	void CheckBoxReset();
	//
	void Filter_Class(TArray<FString>& InHeroList, TArray<EClassTypeEnum>& InFilter);
	void Filter_Grade(TArray<FString>& InHeroList, TArray<int32>& InFilter);
	void Filter_Continent(TArray<FString>& InHeroList, TArray<EContinentTypeEnum>& InFilter);

	//property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UButton*						Button_Reset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UButton*						Button_Close;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Tanker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_MeleeDealer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_RangeDealer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_MagicDealer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Supporter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Hero;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Spirit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Spirit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Grade_One;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Grade_One;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Grade_Two;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Grade_Two;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Grade_Three;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Grade_Three;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Grade_Four;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Grade_Four;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Grade_Five;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Grade_Five;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Grade_Six;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Grade_Six;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Grade_Seven;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Grade_Seven;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Grade_Eight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Grade_Eight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Grade_Nine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Grade_Nine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher*						WidgetSwitcher_Grade_Ten;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Grade_Ten;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Lux;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Solona;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Runa;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UCheckBox*						CheckBox_Nox;
	//event
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UUP_HeroInventory)
		FDelegate_OnOneParamFStringArray	OnSelectedHeroDetailFilter;

public:
	UPROPERTY()
	TArray<FString>					FilteringHeroUDList;
};

UCLASS()
class PROJECTR_API UUP_HeroInventory_LightFilter: public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	// BP Widget Evnet Handlers
	UFUNCTION()		void OnClick_HeroInventory_LightFilter_AttackPower();
	UFUNCTION()		void OnClick_HeroInventory_LightFilter_Level();
	UFUNCTION()		void OnClick_HeroInventory_LightFilter_Grade();
	UFUNCTION()		void OnClick_HeroInventory_LightFilter_Damage();
	UFUNCTION()		void OnClick_HeroInventory_LightFilter_PhysicalDefense();
	UFUNCTION()		void OnClick_HeroInventory_LightFilter_MagicDefense();
	UFUNCTION()		void OnClick_HeroInventory_LightFilter_Health();

	//etc functions
	UFUNCTION()
	void SetSelectedSortingBtnColorNSwitcher(EHeroListFilterEnum currentFilter, bool isdescending);
	UFUNCTION()
	void ResetSortingBtnColor();
	UFUNCTION()
	void ResetDescendingBtn();
	
	//	UMG Widget Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	UUC_HeroFilterButton*						Button_AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	UUC_HeroFilterButton*						Button_Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	UUC_HeroFilterButton*						Button_Grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	UUC_HeroFilterButton*						Button_Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	UUC_HeroFilterButton*						Button_PD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	UUC_HeroFilterButton*						Button_MD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	UUC_HeroFilterButton*						Button_Health;
	//event
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
		FDelegate_OnOneParamInt32			OnSelectedHeroLightFilter;
};

UCLASS()
class PROJECTR_API UUC_HeroInventory_SoulFilter : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()		void OnClick_HeroInventory_SoulFilter_Grade();
	UFUNCTION()		void SetWidgetSwitcher(bool isDescending);
	UFUNCTION()		void ResetBtn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UButton*						Button_Grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UWidgetSwitcher* GradeSwitcher;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EventManager)
		FDelegate_OnOneParamInt32			OnSelectedHeroLightFilter;
};

UCLASS()
class PROJECTR_API UHeroInventory_ScrollData : public UHeroScrollItemData
{
	GENERATED_BODY()

public:
	FString HeroUD;
	bool	bIsSpirit = false;
	bool	bIsInArenaDefence = false;
	EHeroInventoryFunction eHeroInventoryFunction = EHeroInventoryFunction::VE_View;
};

UCLASS()
class PROJECTR_API UUC_HeroInventory_ScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:
	virtual void	NativeConstruct()	override;
	virtual void	InvalidateData()	override;
	void			SetHero(FString heroUD);
	void			SetLock(bool IsLocked);
	const FString&	GetCurHeroUD() const;
	UFUNCTION()
	virtual void	SetSelected(bool b) override;

	bool			Initialize_FloorBossCard(int32 CurrentFloor);
public:
	bool			bSelected = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
	UButton*				Button_HeroCard = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*				Hero_Lock = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UOverlay*				Hero_Check = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*				Icon_Check = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*				Image_Check = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*				Grade_BG = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*				Grade_10 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*				ImageBack = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*				Image_TowerBg = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*				Image_Gray = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UWidgetSwitcher*		WidgetSwitcher_Star = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UWidgetSwitcher*		WidgetSwitcher_Select = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*					Image_HeroCard = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*					Image_Continent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*					Image_HeroJob = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UImage*					Image_Selecting = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UHorizontalBox*				Text_Hero_Level = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UTextBlock*				TextBlock_HeroLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UTextBlock*				TextBlock_HeroName = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UTextBlock*				TextBlock_BossName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UVerticalBox*			HeroLevelStarInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		TArray<UImage*>			HeroStarArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UTextBlock*				Text_Strong;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UTextBlock*				FloorText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UVerticalBox*				Vertical_Job;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UCanvasPanel*				CanvasPanel_BossName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
		UCanvasPanel*				CanvasPanelRoot;

private:
	FString					CurHeroUD;
	FName					CurHeroID;
	int32					CurHeroLevel;
	EHeroInventoryFunction  eHeroInventoryFunction = EHeroInventoryFunction::VE_View;
};

UCLASS()
class PROJECTR_API UUC_Popup_InventoryPlus : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//function
	UFUNCTION()
		void OnClick_ButtonClose();
	UFUNCTION()
		void OnClick_ButtonCancel();
	UFUNCTION()
		void OnClick_ButtonOK();
	UFUNCTION()
		void BindingText(int32 startSize, int32 slotRate, int32 tokenMax, int32 currentToken);

	//property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_InventoryPlus)
		ULocalizingTextBlock* TextBlock_Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_InventoryPlus)
		ULocalizingTextBlock* TextBlock_CurrentAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_InventoryPlus)
		ULocalizingTextBlock* TextBlock_NextAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_InventoryPlus)
		ULocalizingTextBlock* Text_ActiveCurrency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_InventoryPlus)
		UButton*						Button_Close;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_InventoryPlus)
		UButton*						Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_InventoryPlus)
		UButton*						Button_OK;

private:
	int32 token = 0;

};

UCLASS()
class PROJECTR_API UUP_HeroInventory : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	// Control Functions
	void InvalidateData() override;
	void SetMainTab();
	void SetControlBar();
	void ResetVars(bool bScrollViewMultiSelect);
	void ShowDetailFilter();
	void SetCountText();
	void SetAmountData();

	//셋팅된 필터링 변수로 필터링 된 UD리스트를 반환
	void GetHeroUDsWithFiltering(TArray<FString>& HeroList, EHeroInventoryType eHeroType = EHeroInventoryType::VE_TypeHero);
	void GetSpiritUDsWithFiltering(TArray<FString>& SpiritList);
	UFUNCTION()
	void OnRecevie_Hero_LightFilter_Result(int32 eClassTypeEnum);
	UFUNCTION()
	void OnRecevie_Hero_DetailFilter_Result(TArray<FString> HeroList);
	// 결과 통지 받는 함수
	UFUNCTION()
	void OnUpdate_HeroInventory(bool bResult);
	UFUNCTION()
	void OnRecevie_Hero_Grind_Result(const TArray<FREWARD>& Inrewards);
	UFUNCTION()
		void OnReceive_Inventory_Upgrade_Result(bool bResult);
	// BP Widget Evnet Handlers
	UFUNCTION()
	void OnClick_HeroInventory_MainTab_Hero(bool InChecked);
	UFUNCTION()
	void OnClick_HeroInventory_MainTab_Soul(bool InChecked);
	UFUNCTION()
		void OnClick_HeroInventory_Hero_BatchSelect();
	UFUNCTION()
	void OnClick_HeroInventory_Hero_Select();
	UFUNCTION()
		void OnClick_HeroInventory_Hero_SelectCancel();
	UFUNCTION()
		void OnClick_HeroInventory_Hero_SelectGrind();
	UFUNCTION()
		void OnClick_HeroInventory_Hero_SelectLocked();

	UFUNCTION()
	void OnClick_HeroInventory_Hero_Locked_Confirm();
	UFUNCTION()
	void OnClick_HeroInventory_Hero_LightFilter();
	UFUNCTION()
	void OnClick_HeroInventory_Hero_DetailFilter();
	UFUNCTION()
		void OnClick_HeroInventory_Amount();

	UFUNCTION()
	void OnSelectedItem_HeroInventory_ScrollView(URScrollItem* SelectedItem);
	// 히어로 기능별 Select 함수들
	void OnSelectedItem_HeroInventory_ScrollView_HeroView(URScrollItem* SelectedItem);
	void OnSelectedItem_HeroInventory_ScrollView_HeroGrind(URScrollItem* SelectedItem);
	void OnSelectedItem_HeroInventory_ScrollView_HeroLock(URScrollItem* SelectedItem);
	bool IsInSelectedList(FString HeroUD);
	void SetSelectingAllItem();
	//	UMG Widget Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UUP_HeroInventory_LightFilter*	UC_Hero_FilterList_new;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	URScrollView*					HeroInventoryScrollView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		ULocalizingTextBlock* TB_CurrentHeroCnt_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		ULocalizingTextBlock* TB_MaxHeroCnt_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		ULocalizingTextBlock* TB_CurrentHeroCnt_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		ULocalizingTextBlock* TB_MaxHeroCnt_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UWidgetSwitcher*				Switcher_LightFilter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UButton*						Button_LightFilter_Show;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UButton*						Button_LightFilter_Hide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UButton*						Button_DetailFilter_Show;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UButton*						Button_HeroInvenAmount_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UButton*						Button_HeroInvenAmount_2;
	// Main Tab Controls
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UCheckBox_Radio*				Radio_Hero;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UWidgetSwitcher*				Switcher_Hero;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UCheckBox_Radio*				Radio_Soul;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UWidgetSwitcher*				Switcher_Soul;
	// Control Switcher
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UWidgetSwitcher*				Switcher_ControlPanel;
	// Main Control Butttons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UButton*						Button_BatchSelect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UButton*						Button_Select;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UButton*						Button_SelectCancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UButton*						Button_SelectGrind;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UButton*						Button_SelectLocked;
	// Grind Function Controls

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UUC_Hero_Filter_Check* UC_hero_filter_check;
	// Page Function 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UP_Popup_Warning_Splash* UC_Popup_HeroInfo_Warning_Splash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UUC_Popup_HeroBreakUp* UC_Popup_HeroBreakUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UUC_Popup_HeroInfo_Lock* UC_Popup_HeroInfo_Lock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UUC_Popup_HeroBreakUp_Splash* UC_Popup_HeroBreakUp_Splash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UUC_Popup_InventoryPlus* UC_Popup_InventoryPlus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UUC_HeroInventory_SoulFilter* UC_SoulInfo_FilterList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UUC_Popup_SoulInfo* UC_Popup_SoulInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
		UImage*				Image_BlockInput = nullptr;

	UPROPERTY()
	TArray<URScrollItemData*> CachedItemDataList;

	EHeroInventoryFunction		eHeroInventoryFunction =	EHeroInventoryFunction::VE_View;
	EHeroInventoryObject		eHeroInventoryObject	=	EHeroInventoryObject::VE_Hero;
	EHeroListFilterEnum			eHeroInventoryFilter	=	EHeroListFilterEnum::VE_AttackPower;

	//data for binding
	int32 IStart_HeroSlot_Size;
	int32 ICurrent_Hero_TokenCnt;
	int32 IHeroSlot_Rate;
	int32 IHeroSlot_Max;

	// 사용하는 변수들
	TArray<FString>					FilteringHeroList;
	TArray<FString>					SelectedHeros;
	TArray<FString>					SelectedSouls;
	bool							multiSelectMode = false;
	bool							bIsVisibleDetailFilter = false;
	bool							bIsDescending = true;

};
