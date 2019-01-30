// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RChatScrollView.h"
#include "UC_ChatItem.h"
#include "UC_Chat_Outgame.h"




void URChatScrollView::SetSelected(URScrollItem* SelecteItem)
{
	UUC_ChatItem* CurrItem = Cast<UUC_ChatItem>(SelecteItem);
	UUC_ChatItemData* CurrItemData = Cast<UUC_ChatItemData>(CurrItem->ItemData);
	if (CurrItemData->itemData.UIType == EChatItemUIType::VE_INVITE)
	{
		UUC_Chat_Outgame* chatWidget = Cast<UUC_Chat_Outgame>(ParentWidget);
		chatWidget->BP_CanvasPanel_RadioGroup_Tabs->SetActiveRadioIndex(2);
		chatWidget->OnCheckRadioCheck_Guild(true);
	}
}

bool URChatScrollView::IsTailOfScroll()
{
	float fMoveDist = CalculateMoveDist();

	if (CurScrollOffset <= -fMoveDist)
		return  true;
	else
		return false;
}

void URChatScrollView::SetScrollToTail()
{
	FVector2D diff = FVector2D::ZeroVector;

	for (int32 i = 0; i < ItemDataList.Num(); i++)
		diff.Y -= ItemDataList[i]->WidgetSize.Y;

	if (ScrollItemList.Num() > 0)
	{
		CurScrollOffset = 0; 
		SetScrollPos(diff);
		//SetScrollUpdate(nullptr, diff);
	}
}

void URChatScrollView::SetScrollPos(FVector2D diff)
{
	//only for limit enable type
	if (!IsScrollAble)
		return;

	URScrollItem* Item = nullptr;
	int32 ItemListLength = ScrollItemList.Num();
	int32 DataListLength = ItemDataList.Num();

	if (ItemListLength == 0)
		return;

	if (DataListLength == 0)
		return;

	float fMoveDist = 0;

	if (!bIsScrollingActiveTimerRegistered) {
		if (Horizontal)
			fMoveDist = -diff.X - ScrollViewSize.Y;
		else
			fMoveDist = -diff.Y - ScrollViewSize.Y;
		float ScrollAmount = -GetScrollComponentFromVector(diff);
		if (Overscroll.ShouldApplyOverscroll(CurScrollOffset == 0, CurScrollOffset == -fMoveDist, ScrollAmount))
		{
			bIsScrollingActiveTimerRegistered = false;
			InertialScrollManager.ClearScrollVelocity();
			Overscroll.ScrollBy(ScrollAmount);
		}
	}

	float fMoveValue = 0.0f;

	if (Horizontal)
		CurScrollOffset += diff.X;
	else
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

	// if (DataListLength > ItemListLength)
	{
		int CurIdx = 0;

		if (Horizontal)
		{
			if (PhysicalScrollOffset > -ItemInitSize.X)
			{
				CurIdx = 0;
				fMoveValue = PhysicalScrollOffset;
			}
			else if ((DataListLength >= ItemListLength) && (PhysicalScrollOffset <= ((float)(DataListLength - ItemListLength) * ItemInitSize.X) * -1.0f))
			{
				CurIdx = DataListLength - ItemListLength;
				fMoveValue = PhysicalScrollOffset + (float)(DataListLength - ItemListLength) * ItemInitSize.X;
			}
			else
			{
				CurIdx = (int)(PhysicalScrollOffset / ItemInitSize.X * -1.0f) - 1;
				fMoveValue = FMath::Fmod(PhysicalScrollOffset, ItemInitSize.X) - ItemInitSize.X;
			}
		}
		else
		{
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
			else
			{
				CurIdx = (int)(PhysicalScrollOffset / ItemInitSize.Y * -1.0f) - 1;
				fMoveValue = FMath::Fmod(PhysicalScrollOffset, ItemInitSize.Y) - ItemInitSize.Y;
			}
		}
		 
		if (CurIdx != StartIdx)
		{
			ShiftScrollItemList(CurIdx);
			ScrollItemList_ChangeItemDataList(CurIdx);
			StartIdx = CurIdx;
		}
	}
}
