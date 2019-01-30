// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"


#include "UC_HeroInfo_Strong_Stat.generated.h"

class HeroStrongSharedStat;
struct FHeroEnchant;

UCLASS()
class PROJECTR_API UUC_HeroInfo_Strong_Stat : public URBaseWidget
{
	GENERATED_BODY()
public:
	virtual void	NativeConstruct() override;
	virtual void	NativeDestruct() override;
	virtual void	InvalidateData();
	void			InvalidateNewStat();

	void SetCaption();
	void SetEnchantPoint();
	void SetCurrentValue();
	void SetNextValue();
	void SetStat(const FString& InCurrentUD, EHERO_ENCHANT_STAT_SLOT slotEnum, HeroStrongSharedStat* InSharedStat);
	int32 GetNewStat() const;
	EHERO_ENCHANT_STAT_SLOT GetEnchantSlot() const;

	UFUNCTION()
	void	OnClick_Plus();
	UFUNCTION()
	void	OnClick_Minus();

	UPROPERTY()	ULocalizingTextBlock*		TextBlock_StatName;
	UPROPERTY() ULocalizingTextBlock*		Text_EnchantPoint;
	UPROPERTY()	ULocalizingTextBlock*		TextBlock_CurrentValule;
	UPROPERTY()	ULocalizingTextBlock*		TextBlock_NextValule;
	UPROPERTY()	ULocalizingTextBlock*		TextBlock_ChangeValue;
	UPROPERTY()	UButton*					Button_Plus;
	UPROPERTY()	UButton*					Button_Minus;

private:
	int32						ExistStat = 0;
	int32						NewStat = 0;
	int32						iProperty;
	int32						iNextProperty;
	HeroStrongSharedStat*		sharedStat;
	FString						CurrentUD;
	//TArray<FHeroEnchant*>		HeroEnchantInfos;
	EHERO_ENCHANT_STAT_SLOT		eEnchantStateSlot = EHERO_ENCHANT_STAT_SLOT::PhysicalAttackPower;
	EClassTypeEnum				CurrentClass = EClassTypeEnum::VE_ClassTypeMax;
};