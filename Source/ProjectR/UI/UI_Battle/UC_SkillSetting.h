// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UC_SkillSetting.generated.h"

class UUC_SkillHeroIcon;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_SkillSetting : public URBaseWidget
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	InvalidateData() override;

	UFUNCTION(BlueprintCallable, Category = UUC_SkillSetting)
	void OkBtnClick();

	UFUNCTION(BlueprintCallable, Category = UUC_SkillSetting)
	void AllSelectBtnClick();

	UFUNCTION(BlueprintCallable, Category = UUC_SkillSetting)
	void AllDeSelectBtnClick();

	UFUNCTION(BlueprintCallable, Category = UUC_SkillSetting)
	void OnOffBtnClick(int32 idx, bool bOn);

private:
	void SkillCastingOnOff(bool bOn);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillSetting)
	TArray<UCanvasPanel*>			HeroCanvasPanelList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillSetting)
	TArray<UWidgetSwitcher*>		HeroBtnSwitcherList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillSetting)
	TArray<UUC_SkillHeroIcon*>			HeroIconList;

protected:
};
