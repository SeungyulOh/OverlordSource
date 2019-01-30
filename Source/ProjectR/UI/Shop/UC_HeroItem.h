// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "Network/PacketDataStructures.h"
#include "UC_HeroItem.generated.h"

class UUC_CurrencyDisplay;
class UUC_HeroSynergyProduct;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EUIHeroItemTypeEnum : uint8
{
	VE_Shop					UMETA(DisplayName = "Shop"),
	VE_Display				UMETA(DisplayName = "Display"),
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	void SetCharacterID(FName InCharacterID);
	void SetProductID(FHERO_SHOP_PRODUCT& InProduct);
	void SendBuyPacket(FString InProductID, int32 InBuytype);
	void SetHeroSynergyProduct(UUC_HeroSynergyProduct* InProduct);

	UFUNCTION()
	void OnBuy();

	UFUNCTION()
	void OnClickSelected(URScrollItem* InSelectScrollItem);

private:
	void Refresh();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	EUIHeroItemTypeEnum		CurHeroItemType = EUIHeroItemTypeEnum::VE_Shop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UUC_CurrencyDisplay*	CurrencyValue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UImage*					JobImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UImage*					BackImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UTextBlock*				FrontHeroText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UTextBlock*				BackHeroText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UTextBlock*				FrontText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UTextBlock*				BackText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UCanvasPanel*			DisablePanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UButton*				ClickBtn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	TArray<UPanelWidget*>	TypePanelWidgetList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UPanelWidget*			CurrencyPanelWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UPanelWidget*			DownPanelWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroItem)
	UPanelWidget*			HorizontalBox_2 = nullptr;

private:
	UUC_HeroSynergyProduct*	SynergyProduct = nullptr;
	FHERO_SHOP_PRODUCT		CurProduct;
	bool					bBuyEnable = true;
};