// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Shop/UC_HeroProduct.h"
#include "Network/PacketDataStructures.h"
#include "UC_HeroBoxsProduct.generated.h"

class UUC_HeroBox;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroBoxsProduct : public UUC_HeroProduct
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void RefreshUI() override;
	void OnRefresh() override;
	void SetRefScrollView(URScrollView* InScrollView) override;

	void SetHeroShopPanelList(TArray<FHERO_SHOP_PANEL>& InPanelList);

	//UFUNCTION(BlueprintCallable, Category = UUP_Shop)
	//void ChangePackage(int32 InDisplayIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	TArray<UUC_HeroBox*>		BoxList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UUC_HeroBox*				UC_HeroBox_01 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UUC_HeroBox*				UC_HeroBox_02 = nullptr;

private:
	TArray<FHERO_SHOP_PANEL>	ShopPanelList;

};