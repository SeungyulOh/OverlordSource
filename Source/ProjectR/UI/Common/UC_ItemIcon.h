// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"
#include "Table/CurrencyPropertyTableInfo.h"

#include "UC_ItemIcon.generated.h"


class UImage;
class UPanelWidget;
class UTextBlock;
class ULocalizingTextBlock;
class UUC_HeroGrade;

/*
* icon frame and image that is common to all items.
*/
UCLASS()
class PROJECTR_API UUC_ItemIconCommon : public URBaseWidget
{
	GENERATED_BODY()
public:
	//// Interface
	UFUNCTION(BlueprintCallable, Category = UC_ItemIconCommon)
	void RefreshIcon(FName IconImageKey);
	UFUNCTION(BlueprintCallable, Category = UC_ItemIconCommon)
	void RefreshGradeFrame(EItemGradeEnum Grade);

	/** just displays frame and background. used for empty equipment slot in UP_ItemUpgrade */
	UFUNCTION(BlueprintCallable, Category = UC_ItemIconCommon)
	void RefreshEmpty();

	//// BP Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemIconBase|Ref")
	UImage* Image_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemIconBase|Ref")
	UImage* Image_GradeFrame;
};

/**
 * icon for Equipment item
 */
UCLASS()
class PROJECTR_API UUC_EquipmentIcon : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	/** IsInLockMode is a hacky param that causes to toggle the lock icon on bSelected */
	void Refresh(const FItemTableInfo* ItemInfo, const FITEM* ItemData, bool bSelected, bool IsInLockMode);
	//temp
	void SetRuneUI(FName IconKey, int32 OwnedAmount, bool IsSelected, int32 Grade, int32 detailGrade);
	void SetVisiblitySwap(bool IsItem);
	void SetSelected(bool bSelected);

	//// BP Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
	UUC_ItemIconCommon* ItemIconCommon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
		UPanelWidget* Panel_Grade = nullptr;

	// Grade
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
	UTextBlock* Text_Grade;

	// Level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
	UPanelWidget* Panel_Level;

	/** Without "Lv." */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
		UTextBlock* TextBlock_LVText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
	UTextBlock* Text_ItemLevel;

	// Enchant
	/** "+9" */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
	UTextBlock* Text_EnchantLevel;

	// Equipping Hero
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
	UWidget* Panel_Holder;

	// Lock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
	UImage* Image_Lock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
	UWidget* Panel_Selected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
		UHorizontalBox* HBox_Tier = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
		UHorizontalBox* HBox_RuneGrade = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
		UTextBlock* Text_TierNumeric = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
		UTextBlock* Text_RuneGText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_EquipmentIcon|Ref")
		UTextBlock* Text_RuneGNumeric = nullptr;

	//// BP Configs

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemIcon")
	FSlateColor Color_EnchantLevelUnenchantable;

private:
	// saved
	UPROPERTY()
	FSlateColor Color_EnchantLevelNormal;
};

/**
 * icon for HeroPiece item
 */
UCLASS()
class PROJECTR_API UUC_HeroPieceIcon : public URBaseWidget
{
	GENERATED_BODY()
public:

	//// Interface
	void Refresh(const FWalletTableInfo* WalletInfo, int32 OwnedAmount);

	//// BP Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroPieceIcon|Ref")
	UUC_ItemIconCommon* ItemIconCommon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroPieceIcon|Ref")
	UUC_HeroGrade* HeroGrade;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroPieceIcon|Ref")
	ULocalizingTextBlock* Text_Continent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroPieceIcon|Ref")
	UProgressBar* ProgressBar_Amount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroPieceIcon|Ref")
	UTextBlock* Text_Amount;
};

/**
* generic icon for material items
*/
UCLASS()
class PROJECTR_API UUC_MaterialIcon : public URBaseWidget
{
	GENERATED_BODY()
public:

	//// Interface

	/** SelectedAmount of 0 would be rendered as 'not selected' */
	UFUNCTION(BlueprintCallable, Category = UUC_MaterialIcon)
	void Refresh(FName IconKey, int32 OwnedAmount, bool IsSelected, int32 SelectedAmount);
	void SetSelected(bool bSelected);
	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UUC_ItemIconCommon* ItemIconCommon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UTextBlock* Text_Amount;

	// Selected
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UWidget* Panel_Selected;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UWidget* Panel_SelectedAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UTextBlock* Text_SelectedAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UTextBlock* Text_OwnedAmount;
};

/**
 * Switching between icon widgets to represent any item
 */
UCLASS()
class PROJECTR_API UUC_GenericItemIcon : public URBaseWidget
{
	GENERATED_BODY()
public:

	//// Interface
	void RefreshEquipmentIcon(const FItemTableInfo* ItemInfo, const FITEM* ItemData, bool bSelected = false, bool IsInLockMode = false);
	void RefreshWalletIcon(FName WalletKey, const FWalletTableInfo* WalletInfo, int32 OwnedAmount, bool IsSelected = false, int32 SelectedAmount = 0);
	void RefreshHeroIcon(FString ItemUD);

	// dubious: Helper
	UFUNCTION(BlueprintCallable, Category = UUC_GenericItemIcon)
	void RefreshEquipmentIcon(FString ItemUD);

	void SetSelectedPanel(bool bSelected, bool isEquipItem);
	//// BP Widgets

	/** 0: EquipmentItem, 1: HeroPieceIcon, 2: MaterialIcon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericItemIcon|Ref")
	UWidgetSwitcher* Switcher_Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericItemIcon|Ref")
	UUC_EquipmentIcon* EquipmentIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericItemIcon|Ref")
	UUC_HeroPieceIcon* HeroPieceIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericItemIcon|Ref")
	UUC_MaterialIcon* MaterialIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericItemIcon|Ref")
	class URenewal_BaseHeroIcon* HeroIcon = nullptr;
	UPROPERTY()
	UOverlay* Lock;
};


UCLASS()
class PROJECTR_API UUC_GenericItemIconWithFlare : public UUC_GenericItemIcon
{
	GENERATED_BODY()
public:
	//// Interface

	UFUNCTION(BlueprintImplementableEvent, Category = "UC_GenericItemIconWithFlare")
	void NoShine();
	UFUNCTION(BlueprintImplementableEvent, Category = "UC_GenericItemIconWithFlare")
	void Shine();
	UFUNCTION(BlueprintImplementableEvent, Category = "UC_GenericItemIconWithFlare")
	void Flare();

	//// BP Widgets
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_GenericItemIconWithFlare|Ref")
	//UImage* Image_Flare;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_GenericItemIconWithFlare|Ref")
	//UImage* Image_Glisten;
};
