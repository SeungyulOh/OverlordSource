// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "RCampaignHUD_OneControl.h"

#include "RCampaignHUD_ThreeControl.generated.h"

class URHeroUIforCrew;

/**
 * 
 */
UCLASS()
class PROJECTR_API URCampaignHUD_ThreeControl : public URBaseIngameHUD
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;
	void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void	UpdateHeroIconSlot() override;
	virtual void	HideUnusedCharacterUI() override;
	
	UFUNCTION(BlueprintCallable, Category = "IngameHUD")
	virtual void ChangePortrait1();

	UFUNCTION(BlueprintCallable, Category = "IngameHUD")
	virtual void ChangePortrait2();
	
	//tuto
	UFUNCTION()
		void OnClickFirstTutoSkill();
	UFUNCTION()
		void OnReleaseFirstTutoSkill();
	void ShowCrewFaceUI(bool bShow);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	class UUC_CombineBenefitNoti*	UC_HUD_Noti = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UImage*					ChangingImage1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UImage*					ChangingImage2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UImage*					Image_RT = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UHorizontalBox*			Crew1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UHorizontalBox*			Crew2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	URHeroUIforCrew*		Crew1BattleIconUI = nullptr;		// crew1 actor 정보 ui (전투중)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	URHeroUIforCrew*		Crew1RestIconUI = nullptr;			// crew1 acotr 정보 ui (휴식중)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	URHeroUIforCrew*		Crew2BattleIconUI = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	URHeroUIforCrew*		Crew2RestIconUI = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UOverlay*				CampaignStageInfoPanel = nullptr;

	UPROPERTY()
	TArray<URHeroUIforBase*>	HeroIconArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Debug")
	bool						isDebugRenderTarget = false;

	//tuto
	UPROPERTY()
		UButton* Button_ForTuto_1;
	FVector skillPos;
};
