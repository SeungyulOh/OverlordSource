// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_CampaignTower_Scrollview.h"

#include "UI/Control/RScrollView.h"

#include "GlobalIntegrated.h"


#include "GameInfo/UserInfo.h"

#include "UI/UI_Campaign/RTowerScrollView.h"
#include "UI/UI_Campaign/UP_Campaign_Tower.h"

const float ScrollY = 720.f;

void UUP_CampaignTower_Scrollview::NativeConstruct()
{
	Super::NativeConstruct();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FAVATAR Avatar = RGameInstance->UserInfo->GetAvatarData();
	FTOWER_STATUS TowerStatus = RGameInstance->CampaignManager->TowerStatus;
	UMissionManager* MissionManager = RGameInstance->MissionManager;

	/*TowerScrollView->Custom_Init(Avatar.elevatedFloor);
	
	for (size_t i = 0; i < TowerScrollView->ScrollItemList.Num(); ++i)
	{
		URScrollItemData* ScrollItemData = Cast<URScrollItemData>(TowerScrollView->AddScrollItemData(URScrollItemData::StaticClass()));

		if (!IsValid(ScrollItemData))
			continue;

		URTowerScrollItem* ScrollItem = Cast<URTowerScrollItem>(TowerScrollView->ScrollItemList[i]);
		if (IsValid(ScrollItem))
		{
			UUP_Campaign_Tower* UP_CampaignTower = ScrollItem->UP_CampaignTower;
			if (UP_CampaignTower)
			{
				int32 Floor = Avatar.elevatedFloor - i + 1;

				if (Floor <= 0)
					Floor = 1;

				UP_CampaignTower->SetCurrentFloor(Floor);
				UP_CampaignTower->Invalidate();
			}
		}
		
	}*/

	TowerScrollView->InvalidateData();
	
	int32 TargetFloor = TowerStatus.currentFloor;

// 	if(IsValid(MissionManager) && MissionManager->StageTargetFloor != INVALID_TARGET_FLOOR)
// 		TargetFloor = MissionManager->StageTargetFloor;

	MoveScroll_ToTargetFloor(TargetFloor);

// 	if (IsValid(MissionManager))
// 		MissionManager->StageTargetFloor = INVALID_TARGET_FLOOR;
}

void UUP_CampaignTower_Scrollview::Invalidate_CurrentFloor()
{
	FTOWER_STATUS TowerStatus = RGAMEINSTANCE(this)->CampaignManager->TowerStatus;

	for (size_t i = 0; TowerScrollView->ScrollItemList.Num(); ++i)
	{
		URTowerScrollItem* TowerScroll = Cast<URTowerScrollItem>(TowerScrollView->ScrollItemList[i]);
		if (!TowerScroll)
			continue;
		UUP_Campaign_Tower* campaignTower = TowerScroll->UP_CampaignTower;
		if (!campaignTower)
			continue;

		if (campaignTower->GetCurrentFloor() == TowerStatus.currentFloor)
		{
			campaignTower->Invalidate();
			break;
		}
	}
}

void UUP_CampaignTower_Scrollview::MoveScroll_ToTargetFloor(int32 InTargetFloor)
{
	/*TowerScrollView->ResetCurScrollOffSet();
	FAVATAR Avatar = RGAMEINSTANCE(this)->UserInfo->GetAvatarData();

	FVector2D Offset = FVector2D(0.f, (Avatar.elevatedFloor - InTargetFloor) * -ScrollY);
	TowerScrollView->SetScrollUpdate(nullptr, Offset);*/
}
