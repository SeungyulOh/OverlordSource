// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GlobalIntegrated.h"
#include "Global/ShopManager.h"

#include "GameInfo/UserInfo.h"

#include "Network/PacketDataStructures.h"

#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"

#include "UC_FloorShop_FloorSelect.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Utils/UIFunctionLibrary.h"

#include "UI/Common/UP_CommonPopupSplash.h"

void UUC_FloorShop_FloorSelect::NativeConstruct()
{	
	Super::NativeConstruct();

	UCanvasPanelSlot*	CanvasPanelSlot			=	Cast<UCanvasPanelSlot>(Overlay_Drag->Slot);
	ButtonSizeY		=	CanvasPanelSlot->GetSize().Y;
	PosY			=	CanvasPanelSlot->GetPosition().Y;

	Height_1Floor = ButtonSizeY / TOTAL_FLOOR;

	UCanvasPanelSlot*	FullCanvasPanelSlot		=	Cast<UCanvasPanelSlot>(this->Slot);
	LeftTopPos		=	FullCanvasPanelSlot->GetPosition().Y;
	FirstPosY		=	LeftTopPos + PosY+20;
}


void UUC_FloorShop_FloorSelect::NativeDestruct()
{
	Super::NativeDestruct();
}

FReply UUC_FloorShop_FloorSelect::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	MovePos				=	InGestureEvent.GetScreenSpacePosition();
	TargetTextFloorPos	=	FMath::Clamp<float>(MovePos.Y - FirstPosY, 0, ButtonSizeY);
	TargetFloor			=	FMath::Clamp<int32>((TOTAL_FLOOR+1) - (TargetTextFloorPos / Height_1Floor), 1, TOTAL_FLOOR);

	FString					str					=	FString::FromInt(TargetFloor) + TEXT("F");

	if (Text_CurFloor)
		Text_CurFloor->SetText(FText::FromString(str));

	if (CanvasPanel_Cursor)
	{
		UCanvasPanelSlot*	CanvasPanelSlot		=	Cast<UCanvasPanelSlot>(CanvasPanel_Cursor->Slot);
		float				CursorSizeY			=	CanvasPanelSlot->GetSize().Y;
		float				CursorSizeYHalf		=	CursorSizeY / 4;
		FVector2D			NewPos				=	CanvasPanel_Cursor->RenderTransform.Translation;
		NewPos.Y		=	TargetTextFloorPos - CursorSizeYHalf;
		CanvasPanel_Cursor->SetRenderTranslation(NewPos);
	}

	return FReply::Unhandled();
}

FReply UUC_FloorShop_FloorSelect::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (CanvasPanel_Cursor)
		CanvasPanel_Cursor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	return FReply::Unhandled();
}

FReply UUC_FloorShop_FloorSelect::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	URGameInstance*		GameInstance	=	RGAMEINSTANCE(this);
	UShopManager*		ShopManager		=	GameInstance->ShopManager;
	UUserInfo*			UserInfo		=	GameInstance->UserInfo;
	FAVATAR				Avatar			=	UserInfo->GetAvatarData();

	if (CanvasPanel_Cursor)
		CanvasPanel_Cursor->SetVisibility(ESlateVisibility::Collapsed);

	/*if (TargetFloor <= Avatar.elevatedFloor)
	{
		ShopManager->RequestFloorStoreStatus(TargetFloor, false);
		ShopManager->SetCurFloor(TargetFloor);
	}
	else
	{
		UUP_CommonPopupSplash* CommonPopup = UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000060")));

		TargetFloor = 1;
	}*/


	return FReply::Unhandled();
}

