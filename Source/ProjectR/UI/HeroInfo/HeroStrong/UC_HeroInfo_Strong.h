// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/Common/UC_StatHeroLevelUp.h"
#include "UI/HeroInfo/HeroStrong/UC_HeroInfo_Strong_Stat.h"
#include "UC_HeroInfo_Strong.generated.h"

class UUC_HeroStar;

USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_HeroInfoStrong
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) URenewal_BaseHeroIcon*				HeroIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) UTextBlock*							Text_HeroName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) UTextBlock*							Text_HeroNickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) UImage*								Image_Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) ULocalizingTextBlock*					Text_CombatPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) UUC_HeroStar*							HeroStar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) ULocalizingTextBlock*					Text_HeroLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) ULocalizingTextBlock*					Text_EnchantLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) ULocalizingTextBlock*					Text_EL_Before;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) ULocalizingTextBlock*					Text_EL_After;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) ULocalizingTextBlock*					Text_EnchantPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) UButton*								Button_EnchantPointPlus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) UWidgetSwitcher*						Switcher_EnchantPointPlusImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) ULocalizingTextBlock*					Text_EP_After;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) TArray<UUC_HeroInfo_Strong_Stat*>		HeroStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) UButton*								Button_Apply;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) UImage*								Image_Currency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) ULocalizingTextBlock*					Text_Currency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) UButton*								Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_HeroInfoStrong) UButton*								Button_Close;

};

class HeroStrongSharedStat {
public:
	void	InitStat(int32 StatInt, int32 enchantLv, int32 maxP, UTextBlock* text_ep, UTextBlock* text_ep_after, UTextBlock* text_el_after);
	void	AddWidget(URBaseWidget* InWidget);
	int32	Write();
	void	Erase();
	bool	EnableStat() const;
	void	InvalidateData();
	bool	LessThanMax(int32 newStat);

private:
	int32 sharedInt;
	int32 m_enchantLv;
	int32 sharedIntBackup = 0;
	int32 statMaxPoint = 0;
	int32 UsePoint = 0;
	TArray<URBaseWidget*>	WidgetList;
	UTextBlock*				Text_EnchantPoint;
	UTextBlock*				Text_EP_After;
	UTextBlock*				Text_EL_After;
};

UCLASS()
class PROJECTR_API UUC_HeroInfo_Strong : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetHero(FString HeroUD);

	void SetMaterial(TArray<FString>& InHeroUD, TArray<int32>& InPer);
	void InvalidateDataStat();

	UFUNCTION()
		void OnClick_EnchanPointPlus();
	UFUNCTION()
		void OnPress_EnchantPointPlus();
	UFUNCTION()
		void OnRelease_EnchantPointPlus();

	UFUNCTION()
	void OnClick_Apply();

	UFUNCTION()
		void OnClick_Close();

//	¸É¹ö º¯¼ö
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInfo_Strong)
	FVariables_HeroInfoStrong variables;
	//
	int32 CurrentUsePoint;
	TArray<int32>							m_PrevStat;
private:
	FString									m_CurrentHeroUD;
	HeroStrongSharedStat					sharedStat;
	FHERO*									m_Hero;
};