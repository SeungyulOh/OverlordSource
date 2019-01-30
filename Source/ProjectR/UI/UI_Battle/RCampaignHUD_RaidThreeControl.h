// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "RCampaignHUD_ThreeControl.h"

#include "RCampaignHUD_RaidThreeControl.generated.h"




UCLASS()
class PROJECTR_API UHUD_RaidThreeControl : public URCampaignHUD_ThreeControl
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;
	void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void	UpdateHeroIconSlot();
	virtual void	HideUnusedCharacterUI();
	virtual void	RenderTime();

	void UpdateMyTotalHP(int32 curr_hp, int32 max_hp);
	void UpdateEnemyTotalHP(int32 curr_hp, int32 max_hp);


	void	ColosseumInit(int32& InMyTeamTotalHP, int32& InOtherTeamTotalHP);
	void	ColosseumPointUpdate(int32 InMyPoint, int32 InOtherPoint);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UTextBlock*				Text_Min = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UTextBlock*				Text_Sec = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UTextBlock*				Text_CountDown = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ColosseumHPUI)
	class UUC_HUD_PVP_TotalHP*	UC_TotalHP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ColosseumHPUI)
	class UUC_CS_Topbar*	UC_CS_Topbar = nullptr;

	DECLARE_DELEGATE_TwoParams(FOnColosseum_PointChanged , int32 , int32)
	FOnColosseum_PointChanged OnColosseum_PointChanged;

	UPROPERTY()
	UCanvasPanel*				TimerPanel = nullptr;

private:
	int32						nPrevRemainTime = 0;
};
