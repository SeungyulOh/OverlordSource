// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_Popup_Splash_AttackPower.generated.h"

class URenewal_BaseHeroIcon;
/**
*
*/
UCLASS()
class PROJECTR_API UUP_Popup_Splash_AttackPower : public URBaseWidget
{
	GENERATED_BODY()


public:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UFUNCTION(BlueprintCallable, Category = Popup_Splash_AttackPower)
	void		RefreshUI(FName InHeroID, int32 InCurrentPower, int32 InPrevPower);
	
	void		ChangeValueTick(float InDeltaTime);
	void		ResetValue();
	bool		CheckEnd();

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Popup_Splash_AttackPower)
	URenewal_BaseHeroIcon*			UC_Icon_Hero = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Popup_Splash_AttackPower)
	UTextBlock*						Text_AttackPow = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Popup_Splash_AttackPower)
	UWidgetSwitcher*				ChangeTextSwitcher = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Popup_Splash_AttackPower)
	UWidgetSwitcher*				BG_WidgetSwitcher = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Popup_Splash_AttackPower)
	UTextBlock*						TextIncrease = nullptr;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Popup_Splash_AttackPower)
	UTextBlock*						TextDecrease = nullptr;

private:
	FHERO_ICON_INFO			HeroIconInfo;
	bool					bPlayStart = false;
	bool					bTextAni = false;

	int32					nStart = 0;
	int32					nEnd = 0;
	int32					nCurrent = 0;

	
	float					fCurrentPercent = 0.0f;
	float					fCurrentTime = 0.0f;
};
