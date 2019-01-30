// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "Network/PacketDataStructures.h"
#include "UC_HeroBox.generated.h"


class UUC_CurrencyDisplay;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroBox : public URScrollItem
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SendBuyPacket(FString InProductID, int32 InBuytype);
	void SendRefreshPacket();

	void SetHeroShopPanel(FHERO_SHOP_PANEL& InPanel);

	UFUNCTION(BlueprintCallable, Category = UUC_HeroBox)
	void OnClick_Buy();

	UFUNCTION()
	void OnBuy();

	UFUNCTION()
	void OnClickSelected(URScrollItem* InSelectScrollItem);

	UFUNCTION()
	void OnRefresh();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroBox)
	UUC_CurrencyDisplay*	UC_CurrencyDisplay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroBox)
	UTextBlock*				TitleText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroBox)
	UTextBlock*				TimeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroBox)
	UImage*					BackImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroBox)
	UWidgetSwitcher*		NormalSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroBox)
	UWidgetSwitcher*		FreeSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroBox)
	UWidgetSwitcher*		PurchaseSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroBox)
	UProgressBar*			RestTimeProgressBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroBox)
	UButton*				ClickBtn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroBox)
	UButton*				Button_Value = nullptr;

private:
	FHERO_SHOP_PANEL		CurPanel;
	FDateTime				CurDateTime;
	int32					RestTime = 0;
	int32					TotalTime = 0;
	bool					bStart = false;
	bool					bSendRefreshPacket = false;
};