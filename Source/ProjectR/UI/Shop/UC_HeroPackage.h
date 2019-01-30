// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Shop/UC_HeroProduct.h"
#include "Network/PacketDataStructures.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UC_HeroPackage.generated.h"


class URScrollView;
class UUC_CurrencyDisplay;
class UCanvasPanel_RadioGroup;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroPackage : public UUC_HeroProduct
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void RefreshUI() override;

	UFUNCTION(BlueprintCallable, Category = UUC_HeroPackage)
	void ChangePackage(bool isCheck);

	UFUNCTION(BlueprintCallable, Category = UUC_HeroPackage)
	void Buy();

	void RefreshPackage();

	UFUNCTION()
	void OnResetRefresh(FHERO_SHOP_PANEL InPanel) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UButton*				ClickBtn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UButton*				Button_Refresh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UImage*					BackImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UCanvasPanel_RadioGroup*	RadioGroup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UCanvasPanel*			BuyComplatePanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UUC_CurrencyDisplay*	CurrencyDisplay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	TArray<UWidgetSwitcher*>	PackageTabSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UWidgetSwitcher*			 WidgetSwitcher_0 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UWidgetSwitcher*			 WidgetSwitcher_1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UWidgetSwitcher*			 WidgetSwitcher_2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UCheckBox_Radio*			 CheckBox_Radio1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UCheckBox_Radio*			 CheckBox_Radio2 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	UCheckBox_Radio*			 CheckBox_Radio3 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroPackage)
	TArray<UCheckBox_Radio*>	RadioButtonList;

	int32						SelectMenuNumber;



private:
	int32						CurSelectIdx = 0;
	int32						TotalTime = 0;
};