// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
 #include "RCampaignHUD_OneControl.h"

#include "RCampaignHUD_RaidOneControl.generated.h"


UCLASS()
class PROJECTR_API URCampaignHUD_RaidOneControl : public URCampaignHUD_OneControl
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	NativeDestruct() override;
	void	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void	UpdateHeroIconSlot();
	virtual void	HideUnusedCharacterUI();

	void			RenderTime();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	class URHeroUIforLeader*					MemberUI_01 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	class URHeroUIforLeader*					MemberUI_02 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	class URHeroUIforLeader*					MemberUI_03 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	class URHeroUIforLeader*					MemberUI_04 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UTextBlock*				Text_Min = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RCampaignHUD|Ref")
	UTextBlock*				Text_Sec = nullptr;
	
	UPROPERTY()
	TArray<class URHeroUIforBase*>	HeroIconArray;
};
