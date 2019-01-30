// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UP_ItemManufacturePopup.generated.h"


class ULocalizingTextBlock;
class UUC_Item_Icons;
class UTextBlock;
class URRichTextBlock;

/**
 * BP: UP_Popup_Item_CraftResult
 * WBP_Popup_ItemCraftResult
 *
 * Pretty.
 */

UCLASS()
class PROJECTR_API UUP_ItemManufacturePopup : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetItem(FITEM item);

public:			
	UFUNCTION(BlueprintCallable, Category = UP_ItemManufacturePopup)
	void						OnClickOK();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacturePopup|Ref")
	UUC_Item_Icons*				ItemIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemManufacturePopup)
	UPanelWidget*					RootPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemManufacturePopup)
	UWidgetSwitcher*				ItemTypeSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemManufacturePopup)
	UTextBlock*						ItemWeaponText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemManufacturePopup)
	UTextBlock*						ItemArmorText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemManufacturePopup)
	UWidgetSwitcher*				ItemTypeDetailSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemManufacturePopup)
	UTextBlock*						ItemDetailText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemManufacturePopup)
	UPanelWidget*					EnchantInfoPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemManufacturePopup)
	UWidgetSwitcher*				EnchantInfoSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemManufacturePopup)
	UTextBlock*						EnchantCurText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemManufacturePopup)
	UTextBlock*						EnchantMaxText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacturePopup|Ref")
	ULocalizingTextBlock*		LocalText_Prefix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacturePopup|Ref")
	UPanelWidget*			Panel_PassiveSkill = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacturePopup|Ref")
	UPanelWidget*			Panel_SkillCoolTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacturePopup|Ref")
	UTextBlock*				Text_SkillCoolTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacturePopup|Ref")
	UTextBlock*				Text_PassiveSkillDesc = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacturePopup|Ref")
	URRichTextBlock*		RText_PopupDesc = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacturePopup|Ref")
	UButton*				Button_Click = nullptr;

	//FITEM_ICON_INFO					CurItemIconInfo;	
};	
