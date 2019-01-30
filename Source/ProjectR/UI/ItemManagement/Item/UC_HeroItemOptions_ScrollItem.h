// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItem.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_HeroItemOptions_ScrollItem.generated.h"

class UPanelWidget;
class UTextBlock;

/**
 * used as single item ScrollItem in UC_HeroItemInfo
 * display additional options of an item
 * 오버로드_장비아이템_변경안.docx.3
 *
 * 표시 항목:
 *
 * * 추가옵션
 * * 특수옵션1
 * ** 룬 옵션
 * ** 타워 옵션 활성화 조건
 * * 특수옵션2
 * ** 룬워드 옵션
 * ** 타워옵션의 옵션 내용
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroItemOptions_ScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:

	//// Interface

	void NativeConstruct() override;
	void NativeDestruct() override;

	void InvalidateData() override;

	//// BP Widgets

	// Additional options

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UPanelWidget* Panel_ItemOption1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UPanelWidget* Panel_ItemOption2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UPanelWidget* Panel_ItemOption3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UPanelWidget* Panel_ItemOption4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UPanelWidget* Panel_ItemOption5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UPanelWidget* Panel_ItemOption6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UPanelWidget* Panel_ItemOption7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UPanelWidget* Panel_ItemOption8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionLabel1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionLabel2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionLabel3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionLabel4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionLabel5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionLabel6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionLabel7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionLabel8;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	//UTextBlock* Text_ItemOptionPlus1;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	//UTextBlock* Text_ItemOptionPlus2;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	//UTextBlock* Text_ItemOptionPlus3;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	//UTextBlock* Text_ItemOptionPlus4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionValue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionValue2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionValue3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionValue4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionValue5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionValue6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionValue7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItemOptions_ScrollItem|Ref")
	UTextBlock* Text_ItemOptionValue8;

	// Special option 1

	// Special option 2

private:

	void RefreshRandomOptionUI(EItemStatOption ItemStatOption, int32 Value, int32 WidgetIndex);

	// BP widgets collected into arrays
	UPROPERTY()
	TArray<UPanelWidget*> ItemOptionPanels;
	
	UPROPERTY()
	TArray<UTextBlock*> ItemOptionLabels;

	//UPROPERTY()
	//TArray<UTextBlock*> ItemOptionPluses;

	UPROPERTY()
	TArray<UTextBlock*> ItemOptionValues;
};
