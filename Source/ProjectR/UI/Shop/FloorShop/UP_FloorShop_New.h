// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UP_FloorShop_New.generated.h"

class UUC_FloorShopScrollListItemData;
class URScrollView;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_FloorShop_New : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void OpenBuyConfirmPopup(FSHOP_FLOORSHOP_SLOT slotInfo);
	void SetItemList(TArray<FSHOP_FLOORSHOP_SLOT> dataList);
	void SetRemainTime(int32 remainSec);
	void RefreshTimerUI();

	//property
	UPROPERTY()
		UTextBlock* Text_Refresh_Desc;
	UPROPERTY()
		UTextBlock* Text_ShopTime_Hour;
	UPROPERTY()
		UTextBlock* Text_ShopTime_Min;
	UPROPERTY()
		UTextBlock* Text_ShopTime_Sec;
	UPROPERTY()
		URScrollView* ScrollView_ItemList;
	UPROPERTY()
		class UUC_Popup_FloorShop_Item* Popup_SellItem;
	UPROPERTY()
		class UUC_Popup_HeroBreakUp_Splash* UC_Popup_FloorShopSuccess_Splash;
	//
	FTimerHandle shopResetTime;
	int32 m_RemainSec = 0;
	UPROPERTY()
	TArray<UUC_FloorShopScrollListItemData*> cachedListData;
};
