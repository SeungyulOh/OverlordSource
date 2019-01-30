// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"

#include "UI/Shop/FloorShop/UC_Popup_FloorShop_Default.h"
#include "UI/Shop/FloorShop/UC_Popup_FloorShop_Item.h"
#include "UI/Shop/FloorShop/UC_FloorShop_FloorSelect.h"
#include "UI/Shop/FloorShop/UC_Splash_FloorShop_Item.h"
#include "UI/Shop/FloorShop/UC_FloorShop_ScrollItem.h"

#include "UI/Component/CheckBox_Radio.h"
#include "UI/Component/CanvasPanel_RadioGroup.h"

#include "Network/PacketDataEnum.h"

#include "UP_FloorShop.generated.h"



#define		Normal_Max_Item				10
#define		Sells_PerLevel_StartValue	4
#define		Ruler_Max_Item				5

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_FloorShop : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Init();

	void SetUI();
	void SetGroupList();
	void SetButton();
	void SetFloorText();
	void SetResetText();
// 	void SetItem(ESHOP_FLOORSTORE_TYPE InCurStore);
	void SetTime();
	void ShopOpenPopupActive();
	void ShopOpenPopupDisable();
	void SetRewardItem(TArray<FREWARD>& InReward);
// 	void SetItemList(UWidgetSwitcher* InWidgetSwitch, ESHOP_FLOORSTORE_TYPE InType, bool InButtonEnable, int32 InShopSwitch);

	UFUNCTION()
	void OnNormalCheckBoxClick(bool IsCheck);
	UFUNCTION()
	void OnCoinageCheckBoxClick(bool IsCheck);
	UFUNCTION()
	void OnRulerCheckBoxClick(bool IsCheck);

	UFUNCTION()
	void OnButtonClickRest();
	UFUNCTION()
	void OnTimeOut();
	UFUNCTION()
	void OnItemResetTimeOut();
	UFUNCTION()
	void TimeVisibleState();
public:
	UPROPERTY()
	UCheckBox_Radio*					CheckBox_Normal			=	nullptr;
	UPROPERTY()
	UCheckBox_Radio*					CheckBox_Coinage		=	nullptr;
	UPROPERTY()
	UCheckBox_Radio*					CheckBox_Ruler			=	nullptr;
	
	UPROPERTY()
	UWidgetSwitcher*					WidgetSwitcher_Normal	=	nullptr;
	UPROPERTY()
	UWidgetSwitcher*					WidgetSwitcher_Coinage	=	nullptr;
	UPROPERTY()
	UWidgetSwitcher*					WidgetSwitcher_Ruler	=	nullptr;

	UPROPERTY()
	UTextBlock*							Text_Floor				=	nullptr;
	UPROPERTY()
	UTextBlock*							Text_ShopTime			=	nullptr;
	
	UPROPERTY()
	UButton*							Button_Reset			=	nullptr;

	UPROPERTY()
	UTextBlock*							Text_ResetCountMin		=	nullptr;
	UPROPERTY()
	UTextBlock*							Text_ResetCountMax		=	nullptr;
	UPROPERTY()
	UTextBlock*							Text_ResetTime			=	nullptr;
	
	UPROPERTY()
	UWidgetSwitcher*					WidgetSwitcher_Shop		=	nullptr;
	
	UPROPERTY()
	TArray<UUC_FloorShop_ScrollItem*>	ItemInfo;
	UPROPERTY()
	TArray<UUC_FloorShop_ScrollItem*>	RulerItemInfo;

	UPROPERTY()
	UOverlay*							Overlay_RulerText		=	nullptr;

	UPROPERTY()
	UUC_Popup_FloorShop_Default*		Popup_OpenFloorShop		=	nullptr;
	UPROPERTY()
	UUC_Popup_FloorShop_Item*			Popup_SellItem			=	nullptr;
	UPROPERTY()
	UUC_FloorShop_FloorSelect*			FloorScroll				=	nullptr;
	UPROPERTY()
	UUC_Splash_FloorShop_Item*			Splash_AddItem			=	nullptr;

private:
	UPROPERTY()
	FTimerHandle						TimeHandler_ShopTime;
	UPROPERTY()
	FTimerHandle						TimeHandler_ResetTime;
	UPROPERTY()
	UWorld*								OwnerWorld;
}; 