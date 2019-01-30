// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_Result_Colosseum.generated.h"

class UUC_Item_Icons;
class UUC_Coloseum_RankUp_Splash;
class UUC_ResultEffect;
class UUC_MaterialIcon;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Result_Colosseum : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void Init();
	UFUNCTION()
	void ShowUnlockContentsPopup();

	void SetInfoText();
	void SetInfoSwitcher();
	void SetProgressBar();
	void SetRewards();
	void OpenRankingChanginSplash();
	//	성공 출력
	void SetStageNameText();
	void SetTimeText();

	void SetStageResultFail();
	void SetStageResultTimeOut();

	//	버튼 
	UFUNCTION()
		void ClickHeroManager();
	UFUNCTION()
		void ClickTowerLobby();
	UFUNCTION()
		void ClickColosseumLobby();

public:
	UPROPERTY()
		ULocalizingTextBlock* Text_LeagueName = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_Ranking = nullptr;
	//
	UPROPERTY()
		UWidgetSwitcher* RankingChangingSwitcher = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_RankingRaise = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_RankingFall = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_RankingStay = nullptr;
	//
	UPROPERTY()
		ULocalizingTextBlock* Text_WinningRecord = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_LosingRecord = nullptr;
	//
	UPROPERTY()
		UWidgetSwitcher* RecordChangingSwitcher = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_WinningStraight = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_LosingStraight = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_RecordStay = nullptr;
	//
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "UUP_Result_Colosseum")
	UTextBlock* Text_LevelNew = nullptr;
	UPROPERTY()
		UProgressBar* ProgressBar_LevelMax = nullptr;
	UPROPERTY()
		UProgressBar* ProgressBar_LevelMin = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_LevelPercent = nullptr;
	//
	UPROPERTY()
		ULocalizingTextBlock* Text_ColosseumPoint = nullptr;
	UPROPERTY()
		UWidgetSwitcher* ColosseumPointSwitcher = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_PointRaise = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_PointFall = nullptr;
	UPROPERTY()
		ULocalizingTextBlock* Text_PointStay = nullptr;
	//
	UPROPERTY()
		UUC_Item_Icons* UC_Item_Icons_C_1;
	UPROPERTY()
		UUC_Item_Icons* UC_Item_Icons_C_2;
	UPROPERTY()
		UUC_Item_Icons* UC_Item_Icons_C_3;
	UPROPERTY()
		UUC_MaterialIcon* UC_MaterialIcon = nullptr;
	UPROPERTY()
		UButton*			Button_HeroManager = nullptr;
	UPROPERTY()
		UButton*			Button_TowerLobby = nullptr;
	UPROPERTY()
		UButton*			Button_ColosseumLobby = nullptr;
	UPROPERTY()
		UUC_ResultEffect* UC_Result_Display = nullptr;
	UPROPERTY()
		UUC_Coloseum_RankUp_Splash* UC_Coloseum_RankUp_Splash = nullptr;
	UPROPERTY()
	FCOLOSSEUM_STATUS CurrentStatus;
	UPROPERTY()
	FCOLOSSEUM_STATUS PrevStatus;
	UPROPERTY()
	TArray<UUC_Item_Icons*> ItemIcons;

	UPROPERTY()
	class UUC_Colosseum_Class*			ColosseumTier = nullptr;


	int32	currentGrade = 0;
	FText		GradeText;
	int32				GoldAmt = 0;
	int32				HeroRuneAmt = 0;
	
};
