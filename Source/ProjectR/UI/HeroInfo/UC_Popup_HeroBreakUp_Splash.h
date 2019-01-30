// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"

#include "UC_Popup_HeroBreakUp_Splash.generated.h"


UCLASS()
class PROJECTR_API UUC_Popup_HeroBreakUp_Splash : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();
	void SetReward(const TArray<FREWARD>& Inrewards);
protected:
private:

//	¸É¹ö º¯¼ö
public:
	UPROPERTY()
	TArray<URenewal_BaseHeroIcon*>		Icon;
	UPROPERTY()
	TArray<UTextBlock*>					Text;
protected:
private:
};