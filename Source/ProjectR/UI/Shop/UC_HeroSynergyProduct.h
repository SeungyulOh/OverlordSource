// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Shop/UC_HeroProduct.h"
#include "Network/PacketDataStructures.h"
#include "UC_HeroSynergyProduct.generated.h"


class UUC_HeroItem;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroSynergyProduct : public UUC_HeroProduct
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void RefreshUI() override;
	void SetHeroShopPanel(FHERO_SHOP_PANEL& InPanel, bool bRefresh = true) override;
	void SetRefScrollView(URScrollView* InScrollView) override;


	//UFUNCTION(BlueprintCallable, Category = UUP_Shop)
	//void ChangePackage(int32 InDisplayIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroSynergyProduct)
	TArray<UUC_HeroItem*>			HeroItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroSynergyProduct)
	UUC_HeroItem*					UC_HeroItem_01 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroSynergyProduct)
	UUC_HeroItem*					UC_HeroItem_02 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroSynergyProduct)
	UUC_HeroItem*					UC_HeroItem_03 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroSynergyProduct)
	UButton*						Button_Refresh = nullptr;

private:
	int32				TotalTime = 0;
};