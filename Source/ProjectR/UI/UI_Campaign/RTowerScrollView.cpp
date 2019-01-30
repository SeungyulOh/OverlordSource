// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RTowerScrollView.h"



#include "GameInfo/UserInfo.h"

#include "UI/UI_Campaign/UP_Campaign_Tower.h"

#include "Network/PacketFunctionLibrary.h"
#include "Global/RGameInstance.h"
#include "Global/CampaignManager.h"


void URTowerScrollView::Custom_Init(int InCreateCount)
{
	ScrollItemList_RemoveAllElement();
	
	for (int32 i = 0; i < InCreateCount + 2 ; ++i)
	{
		ScrollItemList_AddElement();
	}
	ResetScrollAnimationState();
	ClearScrollItemData();
}

void URTowerScrollView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bUserScrolling && bIsScrollingActiveTimerRegistered)
	{
		UE_LOG(LogUI, Log, TEXT("NativeTick: [ERROR] bInInteraction and bIsScrollingActiveTimerRegistered at the same time"));
	}


	bool bKeepScrolling = false;

	if (bScrolling)
	{
		if (bUserScrolling)
		{
			InertialScrollManager.AddScrollSample(TickScrollDelta, FPlatformTime::Seconds());
			bKeepScrolling = true;
		}
		else if (bIsScrollingActiveTimerRegistered)
		{
			// UpdateInertialScroll
			float ScrollVelocity = InertialScrollManager.GetScrollVelocity();

			if (ScrollVelocity != 0.f)
			{
				if (InitialVelocity == 0.f)
				{
					InitialVelocity = ScrollVelocity;
					float expectedStopTime = FMath::Abs<float>(InitialVelocity / InertialScrollManager.ScrollDecceleration);
					float RemainingDistance = 0.5 * expectedStopTime * (InitialVelocity);
					TargetOffSet = RemainingDistance + CurScrollOffset;

					int32 iScrollCnt = FMath::Abs<float>(TargetOffSet) / ItemInitSize.Y;
					int32 iRemainder = FMath::Abs<int32>(TargetOffSet) % int32(ItemInitSize.Y);
					
					
					int32 DeltaDist = -(iRemainder > 0.5 * ItemInitSize.Y ? ItemInitSize.Y - iRemainder : -iRemainder);

					float TempDist = (RemainingDistance + DeltaDist);
					float NewVelocity = FMath::Sqrt(2 * (TempDist < 0 ? -TempDist : TempDist) * InertialScrollManager.ScrollDecceleration) * (InitialVelocity < 0 ? -1 : 1);
					InertialScrollManager.SetScrollVelocity(NewVelocity);
					ScrollVelocity = InertialScrollManager.GetScrollVelocity();
				
					//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::SanitizeFloat(TargetOffSet + DeltaDist));
					//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::SanitizeFloat(ScrollVelocity));
				}
					
				
				TickScrollDelta += ScrollVelocity * InDeltaTime;
				bKeepScrolling = true;
				InertialScrollManager.UpdateScrollVelocity(InDeltaTime);
			}
			else
			{
				//Double check
				int32 iRemainder = FMath::Abs<int32>(CurScrollOffset) % int32(ItemInitSize.Y);
				if (iRemainder != 0 && (iRemainder < ItemInitSize.Y * 0.04f || iRemainder > ItemInitSize.Y * 0.96f))
				{
					int32 DeltaDist = -(iRemainder > 0.5 * ItemInitSize.Y ? ItemInitSize.Y - iRemainder : -iRemainder);
					TickScrollDelta = DeltaDist;
				}
				else if (iRemainder != 0)
				{
					int32 DeltaDist = -(iRemainder > 0.5 * ItemInitSize.Y ? ItemInitSize.Y - iRemainder : -iRemainder);
					float NewVelocity = FMath::Sqrt(2 * InertialScrollManager.ScrollDecceleration * FMath::Abs<float>(DeltaDist)) * (DeltaDist < 0 ? -1 : 1);
					InertialScrollManager.SetScrollVelocity(NewVelocity);
					InitialVelocity = -1.f;
					return;
				}
				else
				{
					// Stop gliding
					//InertialScrollManager.ClearScrollVelocity();
					//bIsScrollingActiveTimerRegistered = false;
					//InitialVelocity = 0.f;
					//TargetOffSet = 0.f;

					//// Check if possible to change floor
					//FAVATAR Avatar = RGAMEINSTANCE(this)->UserInfo->GetAvatarData();
					//FTOWER_STATUS TowerStatus = RGAMEINSTANCE(this)->CampaignManager->TowerStatus;

					//int32 CalculatedFloor = Avatar.elevatedFloor - FMath::Abs<int32>((int32)CurScrollOffset) / ItemInitSize.Y;
				
					//if ((TowerStatus.currentFloor != CalculatedFloor) && (CalculatedFloor > 0) && (CalculatedFloor <= 25))
					//	UPacketFunctionLibrary::TOWER_CHANGE_FLOOR_RQ(CalculatedFloor);
				}
			}
		}
		
		
		// Execute
		if (TickScrollDelta != 0.f)
		{
			bKeepScrolling = true;
		}

		FVector2D diffVector(0.f, 0.f);
		diffVector.Y = TickScrollDelta;
		SetScrollUpdate(nullptr, diffVector);

		TickScrollDelta = 0.f;

		// Relieve overscroll
		if (Overscroll.GetOverscroll() != 0.f)
		{
			bKeepScrolling = true;

			// Clear inertia if overscrolling
			InertialScrollManager.ClearScrollVelocity();
			bIsScrollingActiveTimerRegistered = false;

			if (!bUserScrolling)
			{
				Overscroll.UpdateOverscroll(InDeltaTime);
			}
		}
	}

	bScrolling = bKeepScrolling;
}

void URTowerScrollView::SetScrollUpdate(URScrollItem* CurItem, FVector2D diff)
{
	if (!IsScrollAble)
		return;

	URScrollItem* Item = nullptr;
	int32 ItemListLength = ScrollItemList.Num();
	int32 DataListLength = ItemDataList.Num();

	if (ItemListLength == 0)
		return;

	if (DataListLength == 0)
		return;

	// Handle overscroll
	if (!bIsScrollingActiveTimerRegistered) {
		float fMoveDist = CalculateMoveDist();
		//UE_LOG(LogUI, Log, TEXT("-fMoveDist=%f CurScrollOffset=%f"), -fMoveDist, CurScrollOffset);

		float ScrollAmount = -GetScrollComponentFromVector(diff);

		if (Overscroll.ShouldApplyOverscroll(CurScrollOffset == 0, CurScrollOffset == -fMoveDist, ScrollAmount))
		{
			//UE_LOG(LogUI, Log, TEXT("Exerting overscroll: %f"), ScrollAmount);
			bIsScrollingActiveTimerRegistered = false;
			InertialScrollManager.ClearScrollVelocity();
			Overscroll.ScrollBy(ScrollAmount);
		}
	}
	
	float fMoveDist = 0.0f;
	float fMoveValue = 0.0f;

	fMoveDist = (float)DataListLength * ItemInitSize.Y - ScrollViewSize.Y;
	CurScrollOffset += diff.Y;

	// min max condition
	if (fMoveDist < 0.0f)					// ItemTotalLength < ScrollViewLength
	{
		CurScrollOffset = 0.0f;
	}
	else if (CurScrollOffset <= -fMoveDist)	// ScrollIng Tail
	{
		CurScrollOffset = -fMoveDist;
	}
	else if (CurScrollOffset >= 0.0f)		// ScrollIng Head
	{
		CurScrollOffset = 0.0f;
	}

	float PhysicalScrollOffset = CurScrollOffset - Overscroll.GetOverscroll();

	fMoveValue = PhysicalScrollOffset;

	int CurIdx = 0;
	if (PhysicalScrollOffset > -ItemInitSize.Y)
	{
		CurIdx = 0;
		fMoveValue = PhysicalScrollOffset;
	}
	else if ((DataListLength >= ItemListLength) && (PhysicalScrollOffset <= ((float)(DataListLength - ItemListLength) * ItemInitSize.Y) * -1.0f))
	{
		CurIdx = DataListLength - ItemListLength;
		fMoveValue = PhysicalScrollOffset + (float)(DataListLength - ItemListLength) * ItemInitSize.Y;
	}

	if (CurIdx != StartIdx)
	{
		ShiftScrollItemList(CurIdx);
		ScrollItemList_ChangeItemDataList(CurIdx);
		StartIdx = CurIdx;
	}
	

	Super::SetScrollUpdate(fMoveValue);
}

void URTowerScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (!UP_CampaignTower)
		UP_CampaignTower= Cast<UUP_Campaign_Tower>(GetWidgetFromName(TEXT("UP_CampaignTower")));
}
