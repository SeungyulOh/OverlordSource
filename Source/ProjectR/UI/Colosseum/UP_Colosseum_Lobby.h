// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Colosseum/UC_Colosseum_Ranking.h"
#include "UI/Colosseum/UC_Colosseum_Class.h"
#include "UI/Colosseum/UC_Colosseum_Reward.h"

#include "UP_Colosseum_Lobby.generated.h"

UENUM(BlueprintType)
enum class ECollseumTap : uint8
{
	VE_Info				=	0,
	VE_BattleRecord		=	1,
	VE_Ranking			=	2,
};

UCLASS()
class PROJECTR_API UUP_Colosseum_Lobby : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void			NativeConstruct()		override;
	void			NativeDestruct()		override;

	UFUNCTION()
	void			Init();
	UFUNCTION()
	void			OnMatch();
	UFUNCTION()
	void			OnRewardInfo();
	UFUNCTION()
	void			OnSeasonReward();
	UFUNCTION()
	void			OnRadioCheckColloseumInfo(bool InIsCheck);
	UFUNCTION()
	void			OnRadioCheckBattleRecord(bool InIsCheck);
	UFUNCTION()
	void			OnRadioCheckRanking(bool InIsCheck);
	UFUNCTION()
	void			SetColosseumInfo();
	UFUNCTION()
	void			SetColosseumSeason();
	UFUNCTION()
	void			SetSeasonTime();
protected:
private:
//	¸É¹ö º¯¼ö
public:
	UPROPERTY()
	UWidgetSwitcher*					WidgetSwitcher_ColloseumInfo	=	nullptr;
	UPROPERTY()
	UCheckBox_Radio*					Radio_ColloseumInfo				=	nullptr;
	UPROPERTY()
	UCheckBox_Radio*					Radio_BattleRecord				=	nullptr;
	UPROPERTY()
	UCheckBox_Radio*					Radio_Ranking					=	nullptr;
	UPROPERTY()
	UUC_Colosseum_Ranking*				RankingList						=	nullptr;
	
	UPROPERTY()
	UUC_Colosseum_Class*				ColosseumTier					=	nullptr;
	UPROPERTY()
	UTextBlock*							Text_Ranking					=	nullptr;
	UPROPERTY()
	UTextBlock*							Text_Point						=	nullptr;
	UPROPERTY()
	UTextBlock*							Text_UITIer						=	nullptr;

	UPROPERTY()
	UButton*							Button_RewardInfo				=	nullptr;

	UPROPERTY()
	UWidgetSwitcher*					WidgetSwitcher_SeasonState		=	nullptr;

	UPROPERTY()
	UTextBlock*							Text_SeasonRemainDay			=	nullptr;
	UPROPERTY()
	UTextBlock*							Text_SeasonRemainHour			=	nullptr;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Colosseum_Lobby")
	UTextBlock*							Text_SeasonRemain = nullptr;
	UPROPERTY()
	UProgressBar*						ProgressBar_Season				=	nullptr;

	UPROPERTY()
	UButton*							Button_SeasonReward				=	nullptr;
	
	UPROPERTY()
	UTextBlock*							Text_WiningStreak				=	nullptr;

	UPROPERTY()
	UTextBlock*							Text_Win						=	nullptr;
	UPROPERTY()
	UTextBlock*							Text_Lose						=	nullptr;
	
	UPROPERTY()
	UButton*							Button_Match					=	nullptr;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Colosseum_Lobby")
	URScrollView*						Scrollview					=	nullptr;
	UPROPERTY()
	UUC_Colosseum_Reward*				RewardInfoPopup					=	nullptr;
	UPROPERTY()
	UTextBlock*							TextBlock_UseStamia				=	nullptr;

	UPROPERTY()
	TArray<class URColosseum_BattleRecord_Data*> CachedItemDataList;
	
private:
	FCOLOSSEUM_STATUS					ColosseumStatus;

	const int32 MAXSCROLLITEM = 10;
	const int32 MAXSCROLLITEMDATA = 30;

};
