// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/HeroInfo/HeroUpgrade/UC_HeroInfo_Upgrade.h"
#include "UI/HeroInfo/HeroUpgrade/UC_Popup_HeroInfo_Upgrade_Splash.h"

#include "UP_HeroInfo_Upgrade.generated.h"

UCLASS()
class PROJECTR_API UUP_HeroInfo_Upgrade : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();
	void SetHero(FString InHeroUD);

	UFUNCTION()
	void OnClick_HeroIcon(FString InHeroUD);
	UFUNCTION()
	void OnHeroUpgrade();
	UFUNCTION()
	void OnClickBack();
protected:
private:

//	¸É¹ö º¯¼ö
public:
	UPROPERTY()
	UUC_HeroInfo_Upgrade*					UpgradeHeroInfo					=	nullptr;
	UPROPERTY()
	UUC_Popup_HeroInfo_Upgrade_Splash*		UpgradeResult					=	nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	UButton* ButtonBack;

protected:
private:
	FString									m_HeroUD;
	FHERO*									m_Hero;
};