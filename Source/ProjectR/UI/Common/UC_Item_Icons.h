// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "UI/UIDefs.h"
#include "Table/ItemTableInfo.h"

#include "UC_Item_Icons.generated.h"


class UImage;
class UUC_Notify;

/*
* An icon of an item, singular
* BP: UC_Item_Icons
*/
UCLASS()
class PROJECTR_API UUC_Item_Icons : public URScrollItem
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = ScrollItem)
	virtual void		SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;


	UFUNCTION(BlueprintCallable, Category = Item_Icons)
	void SetItemIconStructure(const FITEM_ICON_INFO& InItemIconInfo, bool InIsLargeIcon = false);
	FORCEINLINE void SetIsItemDetailIcon(bool InType) { IsItemDetailIcon = InType; }

	UFUNCTION(BlueprintCallable, Category = Item_Icons)
	void SetSelectedItem(bool bSelected);

	UFUNCTION(BlueprintCallable, Category = Item_Icons)
	void SetNotifyState(ENotifyStateFlagEnum InNotifyState);

	void InvalidateData() override;

	void SetVisibleBasicIcon();
	void SetVisibleGemIcons();
	void SetVisibleCompare();
	void SetEmptyImage();
	void SetToolTipInfo();
	void SetEnableIcon(bool bEnable);
	void SetToogleIcon(bool bToogle);

	bool IsValidItemData(const FITEM_ICON_INFO& Item_Icon_Info);

	// bLargeSize causes the widget to be larger
	UFUNCTION()
	void OnSetWidgetSizeType(bool bLargeSize);

	FORCEINLINE bool IsVisibleEquipInfo() {return bIsVisibleEquipInfo;}

	UFUNCTION(BlueprintCallable, Category = Item_Icons)
	void SetVisibleEquipInfo(bool bVisible);

	UFUNCTION()
	virtual void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	//UFUNCTION(BlueprintCallable, Category = Item_Icons)
	//void PlayAnimTrack(FString AniName);

	//UFUNCTION(BlueprintCallable, Category = Item_Icons)
	//void StopAnimTrack(FString AniName);

	//void SetSelectImageInHeroManagement(bool bSelect);

	void Init_TimeTreasureReward(FName IconKey, FText InTopText, FText InBottomText);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Icons)
	UImage*				Image_Disable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UTextBlock*				TopText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UTextBlock*				BottomText = nullptr;

	/*
	Equip
	*/	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Icons)
	UOverlay*			Overlay_EquipHero = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UImage*				Image_EquipHero_BG = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UTextBlock*			Text_EquipHero_Name = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Icons)
	UWidgetSwitcher*	Switcher_ItemCompare = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UTextBlock*			Text_Compare_Better_Value = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UTextBlock*			Text_Compare_Worse_Value = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UWidgetSwitcher*	Switcher_Number = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UTextBlock*			Text_ItemLevel = nullptr;


	/*
	Item icon
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UImage*			Image_Grade = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UImage*			Image_Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UTextBlock*		Text_Enchant_Level = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UTextBlock*		Text_StackNum = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UTextBlock*		Text_ItemGrade = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Icons)
	UTextBlock*		Item_Private_Text = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UImage*			Image_InventoryLock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Icons)
	UCanvasPanel*	Item_Selection_Lock_Container = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item_Icons)
	UCanvasPanel*	time_remain_text_info = nullptr;

	/*
	Gem
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UOverlay*				Overlay_EquipGem1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UOverlay*				Overlay_EquipGem2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UOverlay*				Overlay_EquipGem3 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UImage*					Image_EquipGem1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UImage*					Image_EquipGem2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UImage*					Image_EquipGem3 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UPanelWidget*			Panel_GemEquip = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UUC_Notify*				Item_Notify = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item_Icons|Ref")
	UOverlay*				Overlay_Select = nullptr;
	
	//UPROPERTY()
	FITEM_ICON_INFO			ItemIconInfo;

	UPROPERTY()
	UScaleBox*				ScaleBox_IconSizeType;

private:
	void SetRewardItemIconInfo();
	FItemTableInfo* GetItemTableInfo(FName ItemID);
	EItemTypeEnum GetItemTypeEnum(FName ItemID);

	bool IsValidItem();

private:
	UPROPERTY()
	TArray< UOverlay* >		GemOverlayArray;
	
	UPROPERTY()
	TArray< UImage* >		Item_Gems;

	UPROPERTY()
	bool		bIsVisibleEquipInfo = false;
	
	UPROPERTY()
	bool		IsLargeIcon = false;

	int32		ItemLevel = 1;

	const int32	MaxGemSlot = 3;

	bool		IsItemDetailIcon = false;
};
