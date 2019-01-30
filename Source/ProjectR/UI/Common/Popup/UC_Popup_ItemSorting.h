// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/HeroManagement/UC_HeroFilterButton.h"
#include "UC_Popup_ItemSorting.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Popup_ItemSorting : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	void SetTypeNDescending(EFilterSortTypeEnum currentType, bool isDescending);
	void Reset();
	// BP Widget Evnet Handlers
	UFUNCTION()		void OnClick_Button_Grade();
	UFUNCTION()		void OnClick_Button_AttackPower();
	UFUNCTION()		void OnClick_Button_GettingOrder();
	

	//	UMG Widget Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_ItemSorting)
		UUC_HeroFilterButton*						Button_AttackPower = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_ItemSorting)
		UUC_HeroFilterButton*						Button_GettingOrder = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_ItemSorting)
		UUC_HeroFilterButton*						Button_Grade = nullptr;
};
