// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_DamageMeter.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"
#include "Network/RTSManager.h"

#define MAX_MemberCount		5

void UUC_HUD_DamageMeter::NativeConstruct()
{
	Super::NativeConstruct();
	PrepareWidget();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR)) 
		return;
// 	RGameInstance->EventManager->OnModRoomDamageMeter.RemoveDynamic(this, &UUC_HUD_DamageMeter::OnModRoomDamageMeter);
// 	RGameInstance->EventManager->OnModRoomDamageMeter.AddDynamic(this, &UUC_HUD_DamageMeter::OnModRoomDamageMeter);
	RGameInstance->TimerManager->ClearTimer(RefreshUI_TimeHandler);

	MyUserKID = RGameInstance->HttpClient->GetKID();
}

void UUC_HUD_DamageMeter::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
//		RGameInstance->EventManager->OnModRoomDamageMeter.RemoveDynamic(this, &UUC_HUD_DamageMeter::OnModRoomDamageMeter);
		RGameInstance->TimerManager->ClearTimer(RefreshUI_TimeHandler);
	}

	Super::NativeDestruct();
}

bool UUC_HUD_DamageMeter::Initialize()
{
	Super::Initialize();
	return true;
}

void UUC_HUD_DamageMeter::PrepareWidget()
{
	FindChildWidgetsWithCertainName<UCanvasPanel>(this, Panel_BlueTeamList, TEXT("CanvasPanel_Ranker_"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_BlueTeamNameList, TEXT("TextBlock_RankerName_"));
	FindChildWidgetsWithCertainName<UWidgetSwitcher>(this, Swicher_BlueTeamMyDataList, TEXT("WidgetSwitcher_Ranker_"));
	FindChildWidgetsWithCertainName<UTextBlock>(this, Text_BlueTeamDamageList, TEXT("Damage_"));

}

void UUC_HUD_DamageMeter::OnModRoomDamageMeter(bool bEnable)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	RGameInstance->TimerManager->ClearTimer(RefreshUI_TimeHandler);

	if (bEnable)
	{
		RGameInstance->TimerManager->SetTimer(RefreshUI_TimeHandler, this, &UUC_HUD_DamageMeter::RefreshDamageMeter, RefreshUITime, true);
	}
}

void UUC_HUD_DamageMeter::RefreshDamageMeter()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	TArray<FBattleRecord*> DamageRankList;
	RGameInstance->BattleRecordManager->GetDamageMeterRank(DamageRankList);

	for (int32 CheckIndex = 0; CheckIndex <= MAX_MemberCount; ++CheckIndex)
	{
		ESlateVisibility DataVisible = ESlateVisibility::Collapsed;
		if (DamageRankList.IsValidIndex(CheckIndex))
		{
			DataVisible = ESlateVisibility::SelfHitTestInvisible;

			auto BattleRecord = DamageRankList[CheckIndex];

			if (Swicher_BlueTeamMyDataList.IsValidIndex(CheckIndex))
			{
				Swicher_BlueTeamMyDataList[CheckIndex]->SetActiveWidgetIndex((MyUserKID == BattleRecord->UserKID));
			}
			if (Text_BlueTeamNameList.IsValidIndex(CheckIndex))
			{
				Text_BlueTeamNameList[CheckIndex]->SetText(FText::FromString(*(BattleRecord->NickName)));
			}
			if (Text_BlueTeamDamageList.IsValidIndex(CheckIndex))
			{
				Text_BlueTeamDamageList[CheckIndex]->SetText(FText::AsNumber(BattleRecord->GetAmountOfAttack()));
			}
		}
		
		if (Panel_BlueTeamList.IsValidIndex(CheckIndex))
		{
			Panel_BlueTeamList[CheckIndex]->SetVisibility(DataVisible);
		}
	}
}