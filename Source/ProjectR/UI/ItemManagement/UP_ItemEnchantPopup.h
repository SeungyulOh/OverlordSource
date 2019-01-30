// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "DelegateCombinations.h"
#include "UP_ItemEnchantPopup.generated.h"


class ULocalizingTextBlock;
class UUC_Item_Icons;
class UTextBlock;
class UCanvasPanel;


/**
 * BP: UP_Popup_Item_Enchant
 * WBP_Popup_ItemEnchant
 */

UCLASS()
class PROJECTR_API UUP_ItemEnchantPopup : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	bool Initialize() override;
	
	UFUNCTION(BlueprintCallable, Category = UP_ItemEnchantPopup)
	void						OnClickOK();

	UFUNCTION(BlueprintCallable, Category = UP_ItemEnchantPopup)
	void						OnClickClose();

	UFUNCTION(BlueprintCallable, Category = UP_ItemEnchantPopup)
	void						RefreshPopup();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
		FString					ItemUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	int32 beforeItemCombatPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
		EItemGradeEnum beforeItemGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	FITEM_ICON_INFO					CurItemIconInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	UUC_Item_Icons*					UC_Item_Icons = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
		UTextBlock*						Text_Grade = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
		UTextBlock*						Text_DisplayName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	UTextBlock*						Text_EnchantLvBefore = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	UTextBlock*						Text_EnchantLvAfter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	ULocalizingTextBlock*			Text_CombatValue_Before = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	ULocalizingTextBlock*			Text_CombatValue_Extra = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
		UButton*				Button_OK;
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	ULocalizingTextBlock*			LocText_MagicProperty_Before = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	ULocalizingTextBlock*			LocTxtMagicPropertyValueBefore = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	ULocalizingTextBlock*			LocText_MagicPropertyValue_After = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantPopup)
	UCanvasPanel*					SubPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UPanelWidget*					Panel_MainProperty2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UPanelWidget*			Panel_Option1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UPanelWidget*			Panel_Option2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UPanelWidget*			Panel_Option3 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UTextBlock*				Text_FixedItemOption1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UTextBlock*				Text_FixedItemOption2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UTextBlock*				Text_FixedItemOption3 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UTextBlock*				Text_FixedItemOptionPlus1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UTextBlock*				Text_FixedItemOptionPlus2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UTextBlock*				Text_FixedItemOptionPlus3 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UTextBlock*				Text_FixedItemOptionValue1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UTextBlock*				Text_FixedItemOptionValue2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UTextBlock*				Text_FixedItemOptionValue3 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemEnchantPopup|Ref")
	UImage*					Image_CaptureMat = nullptr;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_InvalidUpgrade);
	FDelegate_InvalidUpgrade inInvalidUpgrade;

private:
	void SetCaptureSequence(bool InVisible);

private:
	bool							IsUIEvent = false;
};	
