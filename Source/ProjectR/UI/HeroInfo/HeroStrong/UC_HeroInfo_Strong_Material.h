// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/HeroManagement/UP_HeroInventory.h"
#include "UC_HeroInfo_Strong_Material.generated.h"

class UUC_Hero_Filter_Check;
class UUP_HeroInventory_LightFilter;

UCLASS()
class PROJECTR_API UUC_HeroInfo_Strong_Material : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();

	void SetHero(FString InHeroUD);
	void SetCost(int32 InCost);

	UFUNCTION()
		void OnClick_SortBtn();
	UFUNCTION()
		void OnClick_FilterBtn();
	UFUNCTION()
	void OnClick_Enchant();
	UFUNCTION()
	void OnClick_AutoSelect();
	UFUNCTION()
		void OnReceive_LightFilter_Result(int32 eClassTypeEnum);
	UFUNCTION()
		void OnReceive_DetailFilter_Result(TArray<FString> HeroList);
	UFUNCTION()
		void RefreshScollData(TArray<FString> list);

protected:
private:

//	¸É¹ö º¯¼ö
public:
	UPROPERTY()
	URScrollView*	UC_HeroScrollView		=	nullptr;
	UPROPERTY()
		UButton*				Button_Sort_Descending = nullptr;
	UPROPERTY()
		UButton*				Button_Sort_Ascending = nullptr;
	UPROPERTY()
		UButton*				Button_Filter_Show = nullptr;
	UPROPERTY()
	UButton*				Button_Enchant			=	nullptr;
	UPROPERTY()
	UButton*				Button_AutoSelect		=	nullptr;
	UPROPERTY()
	UTextBlock*				Text_ActiveCurrency		=	nullptr;
	UPROPERTY()
		UWidgetSwitcher* Switcher_Sort = nullptr;
	UPROPERTY()
		UUC_Hero_Filter_Check* UC_CheckFilter = nullptr;
	UPROPERTY()
		UUC_HeroInventory_SoulFilter* UC_SortingFilter = nullptr;
	TArray<FString> m_FilteringList;
	bool bIsDescending = true;

private:
	FString					m_HeroUD;
	int32					m_Cost;
};