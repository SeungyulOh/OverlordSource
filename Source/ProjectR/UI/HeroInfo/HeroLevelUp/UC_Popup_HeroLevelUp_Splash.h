// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"

#include "UC_Popup_HeroLevelUp_Splash.generated.h"


UCLASS()
class PROJECTR_API UUC_Popup_HeroLevelUp_Splash : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();
	void SetSplashText(FText caption, FText desc);
	void SetHeroSetting(FHERO_ICON_INFO InPrevHero, FHERO_ICON_INFO InCurHero);
	void SetResult(FHERO Hero);

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
private:

//	¸É¹ö º¯¼ö
public:
	UPROPERTY()
	URenewal_BaseHeroIcon*			PrevIcon		=	nullptr;
	UPROPERTY()
	URenewal_BaseHeroIcon*			CurIcon			=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_PrevName	=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_CurName	=	nullptr;
	UPROPERTY()
		UTextBlock* Text_Caption;
	UPROPERTY()
		UTextBlock* Text_Desc;
protected:
private:
};