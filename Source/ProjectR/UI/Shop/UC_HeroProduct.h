// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UC_HeroProduct.generated.h"


class URScrollView;
class UUC_CurrencyDisplay;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroProduct : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SendRefreshPacket();
	void SendBuyPacket(FString InProductID, int32 InBuytype);	
	void SetDisStart() { bStart = false; }
	virtual void SetHeroShopPanel(FHERO_SHOP_PANEL& InPanel, bool bRefresh = true);
	virtual void RefreshUI();
	virtual void SetRefScrollView(URScrollView* InScrollView);

	UFUNCTION(BlueprintCallable, Category = UUC_HeroProduct)
	void ResetBtnClick();

	UFUNCTION()
	virtual void OnResetOK();

	UFUNCTION()
	virtual void OnRefresh();

	UFUNCTION()
	virtual void OnBuy();

	UFUNCTION()
	virtual void OnResetRefresh(FHERO_SHOP_PANEL InPanel);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroProduct)
	URScrollView*		RefScrollView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroProduct)
	UTextBlock*			TitleText1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroProduct)
	UTextBlock*			DescText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroProduct)
	UTextBlock*			RestTimeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroProduct)
	UProgressBar*		RestTimeProgress = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroProduct)
	UPanelWidget*		ResetPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroProduct)
	UUC_CurrencyDisplay*	ResetValue = nullptr;

protected:
	FHERO_SHOP_PANEL			ShopPanel;
	FDateTime					CurDateTime;
	int32						RestTime = 0;
	bool						bStart = false;
	bool						bSendRefreshPacket = false;

};