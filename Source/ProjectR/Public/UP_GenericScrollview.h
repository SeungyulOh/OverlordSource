// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"
#include "UP_GenericScrollview.generated.h"


UENUM(BlueprintType)
enum class EScrollviewType : uint8
{
	VE_HERO_PROTECTED,
	VE_HERO_NORMAL,
	VE_HERO_NORMAL_GRIND,
	VE_HERO_UPGRADE,
	VE_HERO_ENCHANT,
	VE_HERO_DECK,
	VE_HERO_DECK_SHOWHP,
	VE_HERO_DECK_FAVORITE,
	VE_SPIRIT,
	VE_ITEM_WEAPON,
	VE_ITEM_ARMOR_TOP,
	VE_ITEM_ARMOR_BOT,
	VE_ITEM_RING,
	VE_ITEM_AMULET,
	VE_ITEM_UPGRADE,
	VE_BAG_WEAPON,
	VE_BAG_WEAPON_SELL,
	VE_BAG_ARMOR_TOP,
	VE_BAG_ARMOR_TOP_SELL,
	VE_BAG_ARMOR_BOT,
	VE_BAG_ARMOR_BOT_SELL,
	VE_BAG_RING,
	VE_BAG_RING_SELL,
	VE_BAG_AMULET,
	VE_BAG_AMULET_SELL,
	VE_BAG_MISC,
	VE_END
};

USTRUCT(BlueprintType)
struct PROJECTR_API FVariable_GenericScrollview
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UVerticalBox* VerticalBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UImage* ModalImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UButton* BackButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UWidgetSwitcher* SwitcherScrollview;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UWidgetSwitcher* SwitcherExe;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	TArray<UOverlay*> OverlayArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	TArray<UButton*> ButtonArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	TArray<UImage*> TabImageArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	TArray<UImage*>	UnSelectedImageArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	TArray<UImage*>	UnSelectedImageArray2;

	/*UC*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	class UUC_Hero_SortingFilter*			UC_Sorting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	class UUC_Hero_MultiFilter*				UC_Filter;
	/*Buttons*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UButton*				Button_Sorting_Show;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UButton*				Button_Sorting_Hide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UButton*				Button_Filter_Show;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UButton*				Button_InventorySizeUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UButton*				Button_Exe;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UButton*				Button_Done;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UButton*				Button_SelectAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UButton*				Button_HeroinvenAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UTextBlock*				CurrentHeroCountText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UTextBlock*				TotalHeroCountText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UTextBlock*				ButtonName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UTextBlock*				CurrentSelectedSoulText;

	
	/*Switchers*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UWidgetSwitcher*		Switcher_Sorting;
	/*Panels*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UHorizontalBox*			FilterPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	UHorizontalBox*			SelectedSoulBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	class URScrollView* ScrollView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FVariable_GenericScrollview")
	class UUC_Popup_Item_Sell* Popup_ItemSell;

	UPROPERTY()
	EScrollviewType State = EScrollviewType::VE_END;
	UPROPERTY()
	EScrollviewType PrevState = EScrollviewType::VE_END;
	UPROPERTY()
	TArray<class UUC_GenericScrollItemData*> CachedItemDataList;

	UPROPERTY()
	TArray<FString> CachedItemUDList;

	UPROPERTY()
	TArray<UUserWidget*> DerivedPopup;

	TArray<FName> TabNameArray;
	TArray<EScrollviewType> CandidateTypeArray;

	bool bAllSelected = false;

	int32 MAXENCHANTSPIRIT = 5;
};
/**
 * 
 */


USTRUCT(BlueprintType)
struct PROJECTR_API FRenderer_GenericScrollview
{
	GENERATED_USTRUCT_BODY()

	void Render();

	void LayoutSettings();
	void ScrollSettings();

	FText GetFTextFromStringKey(FName name);

public:
	FVariable_GenericScrollview* variables;
};

UCLASS()
class PROJECTR_API UUC_GenericScrollItem : public URScrollItem
{
	GENERATED_BODY()
public:
	void InvalidateData() override;
	void Selected() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericScrollItem|Ref")
	UWidgetSwitcher* Switcher_Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericScrollItem|Ref")
	class URenewal_BaseHeroIcon* HeroIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericItemIcon|Ref")
	class UUC_EquipmentIcon* EquipmentIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericItemIcon|Ref")
	class UUC_MaterialIcon*  MaterialIcon;
};

UCLASS()
class PROJECTR_API UUC_GenericScrollItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString ItemUD;
};

UCLASS()
class PROJECTR_API UUP_GenericScrollview : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/*Delegates*/
	UFUNCTION()
	void OnClick_Backbutton();
	UFUNCTION()
	void OnClickedTab1();
	UFUNCTION()
	void OnClickedTab2();
	UFUNCTION()
	void OnClickedTab3();
	UFUNCTION()
	void OnClickedTab4();
	UFUNCTION()
	void OnClickedTab5();
	UFUNCTION()
	void OnClickedTab6();
	UFUNCTION()
	void OnClick_ButtonSortingShow();
	UFUNCTION()
	void OnClick_ButtonSortingHide();
	UFUNCTION()
	void OnClick_ButtonFilterShow();
	UFUNCTION()
	void OnClick_ButtonExe();
	UFUNCTION()
	void OnClick_ButtonDone();
	UFUNCTION()
	void OnClick_ButtonSelectAll();
	UFUNCTION()
	void OnClick_InventorySizeup();
	

	UFUNCTION()
	void Update_StateController();

	DECLARE_DELEGATE(FOnDisappear)
	FOnDisappear OnDisappear;

	void Appear(bool bShow);
	void Reset();

	void SetState(EScrollviewType eType);
	void SetState_Internal(int32 iTabIdx);
	void Recompose_Itemdatalist(TArray<FString> HeroUDs);
	bool isInsideViewport();

	
public:
	/*Variables*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_GenericScrollview")
	FVariable_GenericScrollview Variables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_GenericScrollview")
	FRenderer_GenericScrollview Renderer;

	/*State*/
	UPROPERTY()
	FDeckManager_Blackboard Blackboard;
	UPROPERTY()
	FUI_StateController StateController;
	/*Variables End*/


	
};
