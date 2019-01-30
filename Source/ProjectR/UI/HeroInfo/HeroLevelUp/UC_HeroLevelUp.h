// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/HeroManagement/UC_HeroInfoStat.h"
#include "UI/HeroManagement/UC_Hero_Skill_Icon.h"

#include "UI/Common/UC_StatHeroLevelUp.h"

#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"

#include "Table/CharacterTableInfo.h"

#include "UC_HeroLevelUp.generated.h"

#define HERO_STAT_MaxHP	0
#define HERO_STAT_AttackPower 1
#define HERO_STAT_Armor 2
#define HERO_STAT_MagicResistance 3


UCLASS()
class PROJECTR_API UUC_HeroLevelUp : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();
	void SetHero(FString InHeroUD);
	void SetHeroInfo();
	void InvalidateDataStat(int32 currentLevel, int32 nextLevel);
	void RefreshLevelUpUI();
	void HeroLevelUp_SliderReset();
	bool isLackOfMaterial();
	void Refresh(FString InHeroUD);

	UFUNCTION()
	void OnClick_Close();
	UFUNCTION()
	void OnClick_HeroLevelUp_SliderPlus();
	UFUNCTION()
	void OnClick_HeroLevelUp_SliderMinus();
	UFUNCTION()
	void OnClick_HeroLevelUp_SliderMax();
	UFUNCTION()
	void OnClick_HeroLevelUp_SliderValueChange(float InChangeValue);
protected:
private:

//	¸É¹ö º¯¼ö
public:
	UPROPERTY()
	class URenewal_BaseHeroIcon*			IconLevelUpHero			=	nullptr;
	UPROPERTY()
	UTextBlock*						LHeroName				=	nullptr;
	UPROPERTY()
	UTextBlock*						LHeroNickName			=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_LAttackPower		=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_LLevel				=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_LReinForce			=	nullptr;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "UUC_HeroLevelUp")
	UTextBlock*						Text_CurrentLevel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroLevelUp")
	UTextBlock*						Text_NextLevel = nullptr;

	UPROPERTY()
	UImage*							Image_LJob				=	nullptr;

	UPROPERTY()
	UWidgetSwitcher*				WidgetSwitcher_Star		=	nullptr;
	UPROPERTY()
	TArray<UImage*>					HeroStar;
	UPROPERTY()
	UButton*						Button_Minus			=	nullptr;
	UPROPERTY()
	UButton*						Button_Plus				=	nullptr;
	UPROPERTY()
	UButton*						Button_Max				=	nullptr;
	UPROPERTY()
	UButton*						Button_LevelUp			=	nullptr;
	UPROPERTY()
	UButton*						Button_Close			=	nullptr;

	UPROPERTY()
	UTextBlock*						Text_HeroRune			=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_Gold				=	nullptr;

	UPROPERTY()
	USlider*						Slider_Level			=	nullptr;

	UPROPERTY()
	UTextBlock*						Text_SliderMin			=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_SliderMax			=	nullptr;

	UPROPERTY()
	UImage*							Image_Progress = nullptr;

	UPROPERTY()
	TArray<UUC_StatHeroLevelUp*>	HeroStats;
	
	
protected:
private:
	FString							m_CurHeroUD;
	FHERO*							m_Hero;
	UCharacterBattleInfo*			m_UIBattleInfo;
	FCharacterTableInfo*			m_UITableInfo;

	int32							m_MinLevel				=	0;
	int32							m_MaxLevel				=	0;
	int32							m_NeedGold				=	0;
	int32							m_NeedHeroRune			=	0;
};