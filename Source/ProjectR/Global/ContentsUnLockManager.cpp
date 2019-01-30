// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ContentsUnLockManager.h"

#include "GlobalIntegrated.h"


//
#include "NotifyList/NotifyEventList.h"
#include "NotifyList/NotifyEvent.h"
#include "NotifyList/NotifyEventUnlockContents.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"
#include "UI/RWidgetManager.h"
#include "UI/UP_ContentsUnlock.h"
#include "UI/UP_ContentsLockNotify.h"


#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"
#include "ProjectRGameMode.h"
#include "UtilFunctionIntegrated.h"




void UContentsUnLockManager::Initialize()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	RGameInstance->EventManager->OnUpdateUserInfo.AddUniqueDynamic(this, &UContentsUnLockManager::SetContentsUnlockNotification);	
	RGameInstance->GetGameViewportClient()->ExecCommandDelegate.AddUniqueDynamic(this, &UContentsUnLockManager::ProcessConsoleCommand);
}


void UContentsUnLockManager::SetContentsUnlockNotification()
{	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	UnlockNoticeQueue.Empty();

	if(UnlockNotifyList.notify.Num() == 0)
		return;

	const TArray<int32>& Unlocked = UnlockNotifyList.notify;

	for (int32 Val : Unlocked)
	{	
		FName IndexName = FName(*FString::FromInt(Val));
		FContentsUnlockTableInfo* UnlockTable = UTableManager::GetInstancePtr()->GetContentsUnlockRow(IndexName);

		if(UnlockTable == nullptr || UnlockTable->Level == 1)
			continue;

		UnlockNoticeQueue.Emplace(Val);
	}

	UnlockNotifyList.notify.Empty();

// 	if (IsEnterLobby && !IsLevelupWidgetOpen)
// 	{
// 		if(RGameInstance->CurState == EGameStateEnum::VE_Lobby)
// 			ShowContentsUnlockNotificationPopup();
// 	}
	
}

void UContentsUnLockManager::SetAllUnlockListForTest()
{	 
 	TArray<FName> TableKeys = UTableManager::GetInstancePtr()->GetContentsUnlockRowNames();
 	int32 KeyValue = 0;
 	for (FName TableKey : TableKeys)
 	{
 		FContentsUnlockTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetContentsUnlockRow(TableKey);
		if (TableInfo)
		{
			KeyValue = FCString::Atoi(*TableKey.ToString());
			UnlockList.list.Emplace(KeyValue);
		}

//		checkf(TableInfo != nullptr, TEXT("TableInfo is Empty"));
 	}
}

void UContentsUnLockManager::SetFirstEnterArr(TArray<int32>& InArr)
{
	IsFirstEnterArr.Reset(InArr.Num());
	
	for(int32 nValue : InArr)
	{
		IsFirstEnterArr.Emplace(nValue);
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->ForceTutorialManager->nCurrentTutorialStep = GetSavedForcetutorialStep();
}

bool UContentsUnLockManager::IsFirstEnter(EFirstCheckContentsType InType)
{
	int32 nType = (int32)InType;
	
	if(IsFirstEnterArr.IsValidIndex(nType))
	{
		if (0 < IsFirstEnterArr[nType])
			return false;
	}

	return true;
}

int32 UContentsUnLockManager::GetSavedForcetutorialStep()
{
	if (IsFirstEnterArr.IsValidIndex((int32)EFirstCheckContentsType::FirstForceTutorial))
		return IsFirstEnterArr[(int32)EFirstCheckContentsType::FirstForceTutorial];
	else
		return 0;
}

#if WITH_EDITOR
void UContentsUnLockManager::SetIsFirstEnterArr(int32 nType, int32 flag)
{
	if (IsFirstEnterArr.IsValidIndex(nType))
	{
		IsFirstEnterArr[nType] = flag;
	}
}
#endif // WITH_EDITOR

void UContentsUnLockManager::StartAndSaveUnlockTutorial(EFirstCheckContentsType InContents)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (RGameInstance->ForceTutorialManager)
	{
		//start
		RGameInstance->ForceTutorialManager->UnlockTutorialTriggerSetting(InContents);
		RGameInstance->ForceTutorialManager->OnStartUnlockTutorial();
	}

	//save
	UPacketFunctionLibrary::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ((int32)InContents, 1);
}

bool UContentsUnLockManager::IsUnlockRaidDifficulty(bool InbSingleRaid, int32 InDifficulty)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
#if WITH_EDITOR
	if (!RGameInstance->GameEnvironmentVar.bCheckContentsUnlock)
		return true;
#endif // WITH_EDITOR
	TArray<FName> Rows = UTableManager::GetInstancePtr()->GetContentsUnlockRowNames();
	EGamePlayModeTypeEnum eRaidType = InbSingleRaid ? EGamePlayModeTypeEnum::VE_SingleRaid : EGamePlayModeTypeEnum::VE_MultiRaid;
	for (FName row : Rows)
	{
		FContentsUnlockTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetContentsUnlockRow(row);
		if (TableInfo)
		{
			if (TableInfo->ContentType == eRaidType)
			{
				if (TableInfo->SubKey != InDifficulty)
					continue;

				if (TableInfo->Trigger == FName(TEXT("FLOOR")))
				{
					UCampaignManager* CM = RGameInstance->CampaignManager;
					CM->CalculateFloorStageIndex();
					int32 nFloor = CM->GetCurrentNormalFloor();
					if (TableInfo->Level <= nFloor)
						return true;
					else
						return false;

				}
				else if (TableInfo->Trigger == FName(TEXT("AVATAR")))
				{
					if (TableInfo->Level <= RGameInstance->UserInfo->Avatar.level)
						return true;
					else
						return false;
				}
				else if (TableInfo->Trigger == FName(TEXT("FLOORSTAGE")))
				{
					UCampaignManager* CM = RGameInstance->CampaignManager;
					if (CM->TowerStatus.clearGrades.IsValidIndex(TableInfo->Level))
					{
						if (0 < CM->TowerStatus.clearGrades[TableInfo->Level])
							return true;
						else
							return false;
					}
				}
			}
		}
	}
	return true;
}

bool UContentsUnLockManager::IsUnlockFloor(int32 InFloor)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

#if WITH_EDITOR
	if (!RGameInstance->GameEnvironmentVar.bCheckContentsUnlock)
		return true;
#endif // WITH_EDITOR

	TArray<FName> Rows = UTableManager::GetInstancePtr()->GetContentsUnlockRowNames();
	for (FName row : Rows)
	{
		FContentsUnlockTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetContentsUnlockRow(row);
		if (TableInfo)
		{
			if (TableInfo->ContentType == EGamePlayModeTypeEnum::VE_Campaign)
			{
				if (TableInfo->SubKey != InFloor)
					continue;

				if (TableInfo->Level <= RGameInstance->UserInfo->Avatar.level)
					return true;
				else
					return false;
			}
		}
	}
	return true;
}

bool UContentsUnLockManager::IsContentsUnlock(EGamePlayModeTypeEnum InContentType, int32 InSubKey)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

#if WITH_EDITOR
	if (!RGameInstance->GameEnvironmentVar.bCheckContentsUnlock)
		return true;
#endif // WITH_EDITOR

	bool bResult = true;

	TArray<FName> Rows = UTableManager::GetInstancePtr()->GetContentsUnlockRowNames();
	for(FName& row : Rows)
	{
		FContentsUnlockTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetContentsUnlockRow(row);
		if (TableInfo == nullptr)
			continue;
		if (TableInfo->ContentType != InContentType)
			continue;
		if(InSubKey != TableInfo->SubKey)
			continue;

		if (TableInfo->Trigger == FName(TEXT("FLOOR")))
		{
			UCampaignManager* CM = RGameInstance->CampaignManager;
			CM->CalculateFloorStageIndex();
			int32 nFloor = CM->GetCurrentNormalFloor();
			bResult = (TableInfo->Level <= nFloor);
			break;
		}
		else if (TableInfo->Trigger == FName(TEXT("AVATAR")))
		{
			bResult = (TableInfo->Level <= RGameInstance->UserInfo->Avatar.level);
			break;
		}
		else if (TableInfo->Trigger == FName(TEXT("FLOORSTAGE")))
		{
			UCampaignManager* CM = RGameInstance->CampaignManager;
			
			if (CM->TowerStatus.clearGrades.IsValidIndex(TableInfo->Level))
				bResult = (0 < CM->TowerStatus.clearGrades[TableInfo->Level]);
			else
				bResult = false;
			
			break;
		}
	}

	return bResult;
}

void UContentsUnLockManager::GetUnlockReasonText(EGamePlayModeTypeEnum InContentType, FText& InTitleText, FText& InDescText, int32 InSubKey)
{
	UTableManager* TableManager = UTableManager::GetInstancePtr();
	TArray<FName> RowNames = TableManager->GetContentsUnlockRowNames();
	for (FName& row : RowNames)
	{
		FContentsUnlockTableInfo* TableInfo = TableManager->GetContentsUnlockRow(row);
		if (TableInfo->ContentType == InContentType)
		{
			if(InSubKey != TableInfo->SubKey)
				continue;

			InTitleText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TableInfo->StrKey3);
			InDescText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TableInfo->StrKey4);

			InTitleText = FText::Format(InTitleText, FText::AsNumber(TableInfo->Level));
			InDescText = FText::Format(InDescText, FText::AsNumber(TableInfo->Level));

			break;
		}
	}
}

void UContentsUnLockManager::ShowContentsUnlockNotificationPopup()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("RGameInstance->RInventory is Empty"));
	checkf(RGameInstance->TimerManager != nullptr, TEXT("RGameInstance->TimerManager is Empty"));

	if(UnlockNoticeQueue.Num() == 0)
	{
		if (IsValid(RGameInstance->RInventory))
		{
			if (RGameInstance->RInventory->RuneIDArray.Num() > 0)
			{
				RGameInstance->RInventory->RuneIDArray.Empty();

				RGameInstance->TimerManager->SetTimer(TimeHandler_Result, this, &UContentsUnLockManager::OnResult, 2.0f, false);
			}
			else
			{
				if (UUIFunctionLibrary::ShowHero2D())
				{
					RGameInstance->TimerManager->SetTimer(TimeHandler_Result, this, &UContentsUnLockManager::OnResult, 2.0f, false);
				}
				else
				{
					OnResult();
				}
			}
		}

		return;
	}		

// 	if (RGameInstance && RGameInstance->TimerManager)
// 	{
// 		RGameInstance->TimerManager->SetTimer(TimeHandler_UnlockPopup, this, &UContentsUnLockManager::OnShowUnlockNotification, 0.5f, false);
// 	}		
}

bool UContentsUnLockManager::IsHaveContentsUnlockNoticeQueue()
{
	if (UnlockNoticeQueue.Num() == 0)
		return false;

	return true;
}

void UContentsUnLockManager::ShowLockNotificationPopup(EOVERLORD ContentsType)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));
	checkf(RGameInstance->TimerManager != nullptr, TEXT("RGameInstance->TimerManager is Empty"));

	if (UP_ContentsLockNotify == nullptr)
	{
		if (!CreateLockNotifyWidget())
		{
			return;
		}
	}
	UP_ContentsLockNotify->RefreshUI(ContentsType);
	RGameInstance->TimerManager->SetTimer(TimeHandler_LockNotify, this, &UContentsUnLockManager::CloseLockNoticePopup, 2.f, false);	
}

void UContentsUnLockManager::CloseLockNoticePopup()
{
	UE_LOG(LogUI, Log, TEXT("Unlock popup close!!!!!!!!!!!!!!!!!!!!!!!"));
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));
	checkf(RGameInstance->TimerManager != nullptr, TEXT("RGameInstance->TimerManager is Empty"));

	if( RGameInstance && RGameInstance->TimerManager)
		RGameInstance->TimerManager->ClearTimer(TimeHandler_LockNotify);

	if(UP_ContentsLockNotify)
	{
		UP_ContentsLockNotify->RemoveFromViewport();
		UP_ContentsLockNotify = nullptr;
	}
	
}

void UContentsUnLockManager::ProcessConsoleCommand(FString Command)
{
	const TCHAR* cmd = const_cast<TCHAR*>(*Command);
	if (FParse::Command(&cmd, TEXT("ALLUNLOCK")))
	{
		IsCheat = !IsCheat;
		if (IsCheat)
		{
			SetAllUnlockListForTest();
		}
		else
		{
			RequestContentsUnlock();
		}
	}
}

void UContentsUnLockManager::SetUnlockList(FUNLOCK_LIST InList)
{
	UnlockList = InList;
}

void UContentsUnLockManager::SetUnlockNotifyList(FUNLOCK_NOTIFY InList)
{
	UnlockNotifyList = InList;
}

void UContentsUnLockManager::RequestContentsUnlock()
{
}



bool UContentsUnLockManager::CreateContentsUnlockWidget()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("RGameInstance->RWidgetManager is Empty"));

	if (UP_ContentsUnlock == nullptr)
	{
		UUserWidget* Widget = RGameInstance->RWidgetManager->CreateUserWidget(EWidgetBluePrintEnum::WBP_UnlockPopup);

		if (IsValid(Widget))
		{			
			UP_ContentsUnlock = Cast<UUP_ContentsUnlock>(Widget);			
		}
	}

	if (UP_ContentsUnlock == nullptr)
		return false;
		
	UP_ContentsUnlock->SetVisibility(ESlateVisibility::Hidden);

	return true;
}

bool UContentsUnLockManager::CreateLockNotifyWidget()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("RGameInstance->RWidgetManager is Empty"));

	if (UP_ContentsLockNotify == nullptr)
	{
		UUserWidget* Widget = RGameInstance->RWidgetManager->CreateUserWidget(EWidgetBluePrintEnum::WBP_LockNotify);

		if (IsValid(Widget))
		{
			UP_ContentsLockNotify = Cast<UUP_ContentsLockNotify>(Widget);
			RGameInstance->RWidgetManager->AddToViewport(EWidgetBluePrintEnum::WBP_LockNotify, UP_ContentsLockNotify, 19000);
		}	
	}

	if(UP_ContentsLockNotify)
		return true;

	return false;
}

bool UContentsUnLockManager::CheckInViewport()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("RGameInstance->RWidgetManager is Empty"));

	if (UP_ContentsUnlock == nullptr)
	{
		if (!CreateContentsUnlockWidget())
		{
			return false;
		}
	}

	if(UP_ContentsUnlock == nullptr)
		return false;

	if (!UP_ContentsUnlock->IsInViewport())
	{
		if ( IsValid(RGameInstance->RWidgetManager))
		{
			RGameInstance->RWidgetManager->AddToViewport(EWidgetBluePrintEnum::WBP_UnlockPopup, UP_ContentsUnlock, 20000);
		}
	}	

	return true;

}

bool UContentsUnLockManager::CheckGamePlayByGameModeState()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));

	AProjectRGameMode* GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!IsValid(GameMode))
		return false;

	if(GameMode->GameModeState != EGameModeStateEnum::VE_Start )
		return false;

	return true;

}

void UContentsUnLockManager::CloseContentsUnlockNotification()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));
	checkf(RGameInstance->RInventory != nullptr, TEXT("RGameInstance->RInventory is Empty"));
	checkf(RGameInstance->TimerManager != nullptr, TEXT("RGameInstance->TimerManager is Empty"));

	RGameInstance->TimerManager->ClearTimer(TimeHandler_UnlockPopup);


	if(UP_ContentsUnlock)		
		UP_ContentsUnlock->SetVisibility(ESlateVisibility::Hidden);

	RemoveWidget();

	if (RGameInstance->RInventory->RuneIDArray.Num() > 0)
	{
		RGameInstance->RInventory->RuneIDArray.Empty();

		RGameInstance->TimerManager->SetTimer(TimeHandler_Result, this, &UContentsUnLockManager::OnResult, 0.1f, false);
	}
	else
	{
		if (UUIFunctionLibrary::ShowHero2D())
		{
			RGameInstance->TimerManager->SetTimer(TimeHandler_Result, this, &UContentsUnLockManager::OnResult, 2.0f, false);
		}
		else
		{
			OnResult();
		}
	}
}

void UContentsUnLockManager::OnResult()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));
	checkf(RGameInstance->EventManager != nullptr, TEXT("RGameInstance->EventManager is Empty"));

//	RGameInstance->EventManager->OnCampaignResultEndEvent.Broadcast();
}

void UContentsUnLockManager::ShowStageFailPopup()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	CheckInViewport();
	UP_ContentsUnlock->SetVisibility(ESlateVisibility::Visible);
	UP_ContentsUnlock->InvalidateUI();

	FText Content = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Unlock_Tutorial_Msg_Title")));
	FText Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Unlock_Tutorial_Msg")));

	UP_ContentsUnlock->SetInfo(EGamePlayModeTypeEnum::VE_HeroManagement, Content, Desc, 0);

	UP_ContentsUnlock->RefreshUI();
}

void UContentsUnLockManager::OnShowUnlockNotification()
{	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));
	checkf(RGameInstance->TimerManager != nullptr, TEXT("RGameInstance->TimerManager is Empty"));
	checkf(RGameInstance->NotifyEventList != nullptr, TEXT("RGameInstance->NotifyEventList is Empty"));

	RGameInstance->TimerManager->ClearTimer(TimeHandler_UnlockPopup);

	if (UnlockNoticeQueue.Num() == 0)
		return;

	if(IsLevelupWidgetOpen)
		return;

	if (!CheckInViewport())
		return;
		
	UP_ContentsUnlock->SetVisibility(ESlateVisibility::Visible);
	UP_ContentsUnlock->InvalidateUI();

	for(int32 i = 0; i < UnlockNoticeQueue.Num(); ++i)
	{
		FName IndexName = FName(*FString::FromInt(UnlockNoticeQueue[i]));
				

		UpdateUnlockList(UnlockNoticeQueue[i]);

		FContentsUnlockTableInfo* UnlockTable = UTableManager::GetInstancePtr()->GetContentsUnlockRow(IndexName);

		checkf(UnlockTable != nullptr, TEXT("UnlockTable is Empty"));
		if(!UnlockTable)
			continue;

		FText Content = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, UnlockTable->StrKey1);
		FText Desc = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, UnlockTable->StrKey2);

		if (Content.IsEmpty() || Content.ToString().Equals(TEXT("None")) || Desc.IsEmpty() || Desc.ToString().Equals(TEXT("None")))
		{
			continue;
		}

		Content = FText::Format(Content, FText::AsNumber(UnlockTable->SubKey));
		Desc = FText::Format(Desc, FText::AsNumber(UnlockTable->SubKey));

		UP_ContentsUnlock->SetInfo(UnlockTable->ContentType, Content, Desc, i);

		UNotifyEvent* notifyEvent = RGameInstance->NotifyEventList->AddNotifyEvent(ENotifyEventTypeEnum::VE_UnlockContents);
		if (IsValid(notifyEvent))
		{
			UNotifyEventUnlockContents* unlockNotiyEvent = Cast<UNotifyEventUnlockContents>(notifyEvent);
			if (IsValid(unlockNotiyEvent))
			{
				unlockNotiyEvent->SetUnlockKey(IndexName);
			}
		}
	}

	UP_ContentsUnlock->RefreshUI();
	UnlockNoticeQueue.Reset();

}

void UContentsUnLockManager::ShowUnlockNotificationDelay(float InDelay)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->TimerManager)
	{
		RGameInstance->TimerManager->SetTimer(TimeHandler_UnlockPopup, this, &UContentsUnLockManager::OnShowUnlockNotification, InDelay, false);
	}
}

void UContentsUnLockManager::UpdateUnlockList(int32 InContent)
{
	if (!UnlockList.list.Contains(InContent))
		UnlockList.list.Emplace(InContent);
}

void UContentsUnLockManager::RemoveWidget()
{
	if(UP_ContentsUnlock)
	{
		UP_ContentsUnlock->RemoveFromViewport();
		UP_ContentsUnlock = nullptr;
	}
}

void UContentsUnLockManager::ShowImmediatlyUnlockNotification()
{
	if (UnlockNoticeQueue.Num() == 0)
		return;

	OnShowUnlockNotification();
}

