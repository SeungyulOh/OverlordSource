// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Common/UC_ItemIcon.h"
#include "Network/PacketDataStructures.h"
#include "UP_ResultPVP.generated.h"


class UUC_ResultStatistics;
class UUC_ResultEffect;
class UUC_CurrencyDisplay;
class UUC_Item_Icons;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_ResultPVP : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
		void		GotoHeroManager();
	UFUNCTION()
	void			GotoTowerOutSide();
	UFUNCTION()
	void			GotoArenaLobby();
	UFUNCTION()
	void			GotoFastMatch();
	UFUNCTION()
	void			SetRankText();
	UFUNCTION()
	void			SetPointText();
	UFUNCTION()
	void			SetResultDisplay();
	UFUNCTION()
	void			SetRankImage();
	UFUNCTION()
	void			SetReward();
	UFUNCTION()
	void			SetTimeText();
	UFUNCTION()
	void			SetAvatar();
	UFUNCTION()
	void			SetRewardItem();
public:
	UPROPERTY()
		UButton*					Button_HeroManager = nullptr;
	UPROPERTY()
	UButton*					Button_GotoTowerOutSide		=	nullptr;
	UPROPERTY()
	UButton*					Button_GotoArenaLobby		=	nullptr;
	UPROPERTY()
	UButton*					Button_GotoFastMatch		=	nullptr;
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_Ranking		=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_RankUp					=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_RankDown				=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_Ranking				=	nullptr;
	UPROPERTY()
	UWidgetSwitcher*			WidgetSwitcher_Point		=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_PointUp				=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_PointDown				=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_Point					=	nullptr;
	UPROPERTY()
	UUC_ResultEffect*			UC_Result_Display			=	nullptr;
	UPROPERTY()
	UImage*						Image_Rank					=	nullptr;
	UPROPERTY()
	UUC_MaterialIcon*			UC_MaterialIcon				=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_ResultTime				=	nullptr;
	UPROPERTY()
	UVerticalBox*				honor_ui					=	nullptr;
	UPROPERTY()
	UVerticalBox*				item_reward					=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_Level					=	nullptr;
	UPROPERTY()
	UProgressBar*				ProgressBar_LevelMax		=	nullptr;
	UPROPERTY()
	UProgressBar*				ProgressBar_LevelMin		=	nullptr;
	UPROPERTY()
	UTextBlock*					Text_LevelPercent			=	nullptr;

	TArray<UUC_Item_Icons*>		ItemIcon;
	
};