// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UI/UIDefs.h"
#include "Table/ItemTableInfo.h"

#include "UC_ItemManufactureIcons.generated.h"


/**
 * BP: UC_ItemCraft_ScrollItem
 * referred from UP_ItemManufacture
 */

class UImage;

UCLASS()
class PROJECTR_API UUC_ItemManufactureIcons : public URScrollItem
{
	GENERATED_BODY()
public:
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	UFUNCTION(BlueprintCallable, Category = UUC_ItemManufactureIcons)
	void RefreshIcon(const FITEM_ICON_INFO& Item_Icon_Info, bool IsLargeIcon = false);

	UFUNCTION(BlueprintImplementableEvent, Category = UUC_ItemManufactureIcons)
	void RefreshItemIcon(const FITEM_ICON_INFO& Item_Icon_Info, bool IsLargeIcon = false);

	UFUNCTION()
	virtual void SetSelected(bool bSelected) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemManufactureIcons)
	UImage*					ItemImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemManufactureIcons)
	UImage*					Item_Grade_Image = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemManufactureIcons)
	//UCanvasPanel*			GemsPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemManufactureIcons)
	UWidgetSwitcher*		TypeSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemManufactureIcons)
	UTextBlock*				WeaponText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemManufactureIcons)
	UTextBlock*				ArmorText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemManufactureIcons)
	UTextBlock*				TypeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemManufactureIcons)
	UTextBlock*				GradeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemManufactureIcons|Ref")
	UTextBlock*				Text_Detail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemManufactureIcons|Ref")
	UTextBlock*				Text_Number = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemManufactureIcons)
	UTextBlock*				Item_Private_Text = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_ItemManufactureIcons)
	UVerticalBox*			selected = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemManufactureIcons|Ref")
	UWidgetSwitcher*		Switcher_Lock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemManufactureIcons|Ref")
	UTextBlock*				Text_Level = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemManufactureIcons|Ref")
	ULocalizingTextBlock*	LocalText_Prefix;

};
