// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Result/UC_ResultEffect.h"
#include "UI/Common/UC_ItemIcon.h"
#include "UP_Result_ForRTS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Result_ForRTS : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	//	성공 출력
	void SetStageNameText();
	void SetTimeText();
	void SetAvatarDataText();
	void SetReward();
	void SetStar();

	//	실패
	void SetStageFailText();

	//	버튼 
	UFUNCTION()
		void ClickHeroManager();
	UFUNCTION()
		void ClickMainLobby();
	UFUNCTION()
		void ClickStageSelect();
	UFUNCTION()
		void ClickStageDifficulty();
	UFUNCTION()
		void ClickRetry();
	UFUNCTION()
		void ClickNextStage();
	//
	UFUNCTION()
		bool IsEnoughStamina();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UWidgetSwitcher*	WidgetSwitcher_Type = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UTextBlock*			Text_CampaignName = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UTextBlock*			Text_Time = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UTextBlock*			Text_Level = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UProgressBar*		ProgressBar_LevelMax = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UProgressBar*		ProgressBar_LevelMin = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UTextBlock*			Text_LevelPercent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UUC_ResultEffect*	UC_Result_Display = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	ULocalizingTextBlock* Text_TimeInFail = nullptr;

	//	Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UButton*			Button_HeroManager = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UButton*			Button_MainLobby_01 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UWidgetSwitcher*	WidgetSwitcher_Button1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UButton*			Button_StageSelect = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UButton*			Button_StageDifficulty = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UWidgetSwitcher*	WidgetSwitcher_Button2 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UButton*			Button_Retry = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UButton*			Button_MainLobby_02 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UWidgetSwitcher*	WidgetSwitcher_Button3 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UButton*			Button_Next_01 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UButton*			Button_OK = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UUC_MaterialIcon*	UC_Item_Icons_Money = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UUC_MaterialIcon*	UC_Item_Icons_Hero = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		URScrollView*		BP_ScrollView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UTextBlock*			Text_Gold = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
		UTextBlock*			Text_HeroRune = nullptr;

	UPROPERTY()
		TArray<UWidgetSwitcher*>	WidgetSwitcher_Star;

	UPROPERTY()
		TArray<UTextBlock*>	Text_Star;
	UPROPERTY()
		TArray<UTextBlock*>	Text_UnStar;

	UPROPERTY()
		TArray<UWidgetSwitcher*>	WidgetSwitcher_Fail;

	UPROPERTY()
		TArray<UTextBlock*>	Text_Fail;
	UPROPERTY()
		TArray<UTextBlock*>	Text_UnFail;
	UPROPERTY()
		TArray<class UUC_Item_Icons*> Reward_Icons;

	int32				GoldAmt = 0;
	int32				HeroRuneAmt = 0;
	
};
