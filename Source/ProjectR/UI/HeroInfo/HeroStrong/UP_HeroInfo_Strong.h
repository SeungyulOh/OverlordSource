// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/HeroInfo/HeroStrong/UC_HeroInfo_Strong.h"
#include "UI/HeroInfo/HeroStrong/UC_HeroInfo_Strong_Material.h"
#include "UI/HeroInfo/HeroStrong/UC_Popup_HeroInfo_Strong_Splash.h"
#include "UI/HeroInfo/HeroStrong/UC_Popup_HeroInfo_StrongResult_Splash.h"

#include "UP_HeroInfo_Strong.generated.h"

UCLASS()
class PROJECTR_API UUP_HeroInfo_Strong : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();

	void SetHero(FString HeroUD);
	
	void Do_Consume();

	UFUNCTION()
	void OnClick_HeroIcon(FString InHeroUD);
	UFUNCTION()
	void OnSelectedItem_HeroEnchant_ScrollView(URScrollItem* SelectedItem);
	UFUNCTION()
	void OnHeroEnchantResult(TArray<bool>& Result, int32 InCurrentPoint);
	UFUNCTION()
	void OnReceiveStatApplyResult();
	UFUNCTION()
		void OnClick_BackButton();
		

//	¸É¹ö º¯¼ö
public:
	
	UPROPERTY()
	UUC_HeroInfo_Strong*						EnchantInfo				=	nullptr;
	UPROPERTY()
	UUC_HeroInfo_Strong_Material*				EnchantMaterialList		=	nullptr;
	UPROPERTY()
	UUC_Popup_HeroInfo_Strong_Splash*			EnchantPoint_Apply		=	nullptr;
	UPROPERTY()
	UUC_Popup_HeroInfo_StrongResult_Splash*		Result_Popup			=	nullptr;
	UPROPERTY()
		UButton* Button_Back;

	FString									m_HeroUD;
	FHERO*									m_Hero;
	TArray<FName>							m_HeroID;
	TArray<int32>							m_SpiritPercentList;
	
	int32									m_nPrevPoint = 0;
};