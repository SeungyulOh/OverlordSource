#pragma once

#include "UI/RBaseWidget.h"



#include "UP_MissionSystemMain.generated.h"


/**
*
*/
#define MAX_MISSION_COUNT 10

UCLASS()
class PROJECTR_API UUP_MissionSystemMain : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetMissionStateText();
	void InvalidateMission();
	void SetBotPanel();

	UFUNCTION()
	void OnMissionStatusRp();

	UFUNCTION()
	void OnMissionRewardGetRp(TArray<FREWARD>& InRewards, TArray<FREWARD>& InExtraRewards);

	UFUNCTION()
	void OnClickDayTotalRewardButton();

	UFUNCTION()
	void OnCloseRewardPopup();

// 	void SortMissionListbyDescendingOrder(TArray<FDAILYMISSION_QUEST>& InList);

	UFUNCTION()
	void OnNewMissionButtonClick();
	UFUNCTION()
	void OnRefreshSpecialButtonClick();
	UFUNCTION()
	void OnRefreshNormalButtonClick();
	UFUNCTION()
	void OnReceiveMissionButtonClick();

	UFUNCTION()
	void ReceiveMissionOK();
	UFUNCTION()
	void NewMissionOK();
	UFUNCTION()
	void RefreshSpecialOK();
	UFUNCTION()
	void RefreshNormalOK();

private:
	void ResetTimeTick(float InDeltaTime);
	void SetTimeText(int32 InRemainTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MissionSystemMain)
	ULocalizingTextBlock*			CurrentFloorText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MissionSystemMain)
	ULocalizingTextBlock*			RemainTimeText = nullptr;

	UPROPERTY()
		TArray<class UUC_MissionScrollItem*> ScrollItemList;
	UPROPERTY()
		UProgressBar* MissionProgressBar;
	UPROPERTY()
		UTextBlock* CurrentProgressText;
	UPROPERTY()
		UTextBlock* MaxProgressText;
	UPROPERTY()
		UButton* RequestRewardButton;
	UPROPERTY()
		ULocalizingTextBlock* RewardButtonText;
	UPROPERTY()
		class UUC_FloorMissionReward_Popup* UC_Mission_popup;

	UPROPERTY()
		TArray<FREWARD>	ExtraRewards;
private:
	bool							bActivate = false;
	int32							nCurrentFloor = 1;
	int32							nRemainTime = 0;

	int64							nCurrentCheckTime = 0;

	FText							CountText;
};