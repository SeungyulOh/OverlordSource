// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UP_Result.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 1	영웅 보상 팝업
// 2	퀘스트 보상 팝업
// 3	계정 레벨업 팝업
// 4	Unlock 팝업
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UENUM()
enum class EResultPopupType : uint8
{
	None = 0,

	// 영웅 보상 팝업
	HeroRewardPopup = 1,
	// 퀘스트 보상 팝업
	QuestRewardPopup = 2,
	// 계정 레벨업 팝업
	AvatarLevelupPopup = 3,
	// 언락 팝업
	UnlockPopup = 4,
};

UCLASS()
class UResultPopupOrderChecker : public UObject
{
 	GENERATED_BODY()

public:
	void Initialize();

	EResultPopupType	DequeuePopupOrder();
	bool				IsEmptyQueue();
private:
	void				EnqueuePopupOrder(EResultPopupType InType);
	// TQueue::Enqueue(), TQueue::Dequeue(), TQueue::IsEmpty()
	UPROPERTY()
	TArray<EResultPopupType> PopupOrderQue;
// 	TQueue<EResultPopupType> PopupOrderQue;

};

class UUC_ResultEffect;
class UUC_ResultHeroIcon;
class UUC_Item_Icons;
class URScrollView;
struct FFloorStageDetail;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Result : public URBaseWidget
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
	void SetHeroIcon();

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
	UFUNCTION()
	void GoNextStage();
	//
	UFUNCTION()
	void ClickTutoOK();
	UFUNCTION()
	void ClickTutoCancel();
	
	void SetImportantMissionInfo();


	//// result popup order function start
	UFUNCTION()
	void OnResultPopupClose();

private:
	void ShowResultPopup(EResultPopupType InType);

	void ShowHeroRewardPopup();
	void ShowQuestRewardPopup();
	void ShowAvatarLevelupPopup();
	void ShowUnlockPopup();
	//// end
	
	FFloorStageDetail* FindNextStageInfo();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UWidgetSwitcher*	WidgetSwitcher_Type = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UTextBlock*			Text_CampaignName		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UTextBlock*			Text_Time				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UTextBlock*			Text_Level				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UTextBlock*			LocalizingTextBlock_NextStage = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UProgressBar*		ProgressBar_LevelMax	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UProgressBar*		ProgressBar_LevelMin	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UTextBlock*			Text_LevelPercent		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UUC_ResultEffect*	UC_Result_Display		=	nullptr;
	
	//	Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UButton*			Button_HeroManager		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UButton*			Button_MainLobby_01		=	nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UWidgetSwitcher*	WidgetSwitcher_Button1	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UButton*			Button_StageSelect		=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UButton*			Button_StageDifficulty	=	nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UWidgetSwitcher*	WidgetSwitcher_Button2	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UButton*			Button_Retry			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UButton*			Button_MainLobby_02		=	nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UWidgetSwitcher*	WidgetSwitcher_Button3	=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UButton*			Button_Next_01			=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UButton*			Button_OK				=	nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	URScrollView*		BP_ScrollView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UTextBlock*			Text_Point = nullptr;

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
	TArray<UUC_ResultHeroIcon*> ResultHeroIcon;

	TArray<UUC_Item_Icons*> ItemIcon;


	int32				GoldAmt = 0;

	UPROPERTY()
	FTimerHandle					TimeHandler_BattleRepeat;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UCanvasPanel*				ImportantMissionPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	URRichTextBlock*			ImportantMissionTargetText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	ULocalizingTextBlock*		ImportantCurrentText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	ULocalizingTextBlock*		ImportantMaxText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Result)
	UButton*					GotoMissionContentsButton = nullptr;

	bool						bCheckQuestPass = false;

	UPROPERTY()
	UResultPopupOrderChecker*	ResultPopupOrderChecker;

	UPROPERTY()
	EResultPopupType			CurrentShowingPopup = EResultPopupType::None;

	UPROPERTY()
	FTimerHandle				TimeHandler_HeroRewardDelay;

};