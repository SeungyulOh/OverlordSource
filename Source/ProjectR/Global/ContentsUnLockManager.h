// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataEnum.h"
#include "UI/UIDefs.h"
#include "ContentsUnLockManager.generated.h"


//class URGameInstance;
class UUP_ContentsUnlock;
class UUP_ContentsLockNotify;

/**
 * 
 */
UCLASS()
class PROJECTR_API UContentsUnLockManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize();
	

public:
	
	UFUNCTION()
	void SetContentsUnlockNotification();

	UFUNCTION(BlueprintCallable, Category = ContentsUnLockManager)
	void ShowContentsUnlockNotificationPopup();

	UFUNCTION(BlueprintCallable, Category = ContentsUnLockManager)
	void CloseContentsUnlockNotification();

	
	UFUNCTION(BlueprintCallable, Category = ContentsUnLockManager)
	bool IsContentsUnlock(EGamePlayModeTypeEnum InContentType, int32 InSubKey = 0);

	UFUNCTION(BlueprintCallable, Category = ContentsUnLockManager)
	void ShowLockNotificationPopup(EOVERLORD ContentsType);

	UFUNCTION(BlueprintCallable, Category = ContentsUnLockManager)
	void CloseLockNoticePopup();

	UFUNCTION(BlueprintCallable, Category = ContentsUnLockManager)
	bool IsHaveContentsUnlockNoticeQueue();

	void GetUnlockReasonText(EGamePlayModeTypeEnum InContentType, FText& InTitleText, FText& InDescText, int32 InSubKey = 0);

public:
	UFUNCTION()
	void ProcessConsoleCommand(FString Command);

	// 로그인시 한번만 받음
	void SetUnlockList(FUNLOCK_LIST InList);

	// 언락시점에 해당컨텐츠 목록(UnlockList에 추가 해줘야함)
	void SetUnlockNotifyList(FUNLOCK_NOTIFY InList);
public :
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ContentsUnLockManager)
	void RequestContentsUnlock();

	void SetFirstEnterArr(TArray<int32>& InArr);
	bool IsFirstEnter(EFirstCheckContentsType InType);
	int32 GetSavedForcetutorialStep();
	void StartAndSaveUnlockTutorial(EFirstCheckContentsType InContents);

	bool IsUnlockRaidDifficulty(bool InbSingleRaid, int32 InDifficulty);
	bool IsUnlockFloor(int32 InFloor);

	void SetIsFirstEnterArr(int32 nType, int32 flag);
	void AddUnlockList(int32 InContent) { UnlockNoticeQueue.Emplace(InContent); }

	void OnShowUnlockNotification();
	void ShowUnlockNotificationDelay(float InDelay = 0.5f);

	void ShowStageFailPopup();

private:

	bool CreateContentsUnlockWidget();
	bool CreateLockNotifyWidget();

	bool CheckInViewport();
	bool CheckGamePlayByGameModeState();

	void ShowImmediatlyUnlockNotification();

	void OnResult();

	void UpdateUnlockList(int32 InContent);
	void RemoveWidget();

	void SetAllUnlockListForTest();

private:
	UPROPERTY()
	TArray<int32>										UnlockNoticeQueue;
	UPROPERTY()
	FUNLOCK_LIST										UnlockList;
	UPROPERTY()
	FUNLOCK_NOTIFY										UnlockNotifyList;

	UPROPERTY()
	UUP_ContentsUnlock*									UP_ContentsUnlock;
	UPROPERTY()
	UUP_ContentsLockNotify*								UP_ContentsLockNotify;
	UPROPERTY()
	FTimerHandle										TimeHandler_UnlockPopup;
	UPROPERTY()
	FTimerHandle										TimeHandler_LockNotify;

	FTimerHandle										TimeHandler_Result;



// AVATAR_SELECTIVE_TUTORIAL_UPDATE: [ // 선택적 튜토리얼 값 업데이트 요청
// {
//	idx: { type: "int32" }, // Flag index 번호. 0~30 까지만 허용
// 	value : { type: "int32"}, // Flag value. 1~100 까지만 허용
// },
// 고상문 : 첫진입 체크용 array, 0이면 첫진입, 진입이후 서버에 저장해야함.
	UPROPERTY()
	TArray<int32>										IsFirstEnterArr;

	bool												IsCheat = false;
	bool												IsEnterLobby = false;
	bool												IsLevelupWidgetOpen = false;


};
