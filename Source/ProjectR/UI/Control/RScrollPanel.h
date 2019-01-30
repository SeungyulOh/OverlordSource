// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "RScrollPanel.generated.h"

namespace ScrollInfo
{
	USTRUCT()
	struct PROJECTR_API FScrollInfo
	{
		UPROPERTY()
		UCanvasPanel*		Panel;

		UPROPERTY()
		float				ScrollPos;
	};
}

/**
 * 
 */
UCLASS()
class PROJECTR_API URScrollPanel : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollPanel)
	TArray<UCanvasPanel*>			ScrollPanelList;

	UFUNCTION(BlueprintCallable, Category = ScrollPanel)
	void RefreshPanel();
	
	void NativeConstruct() override;
	void NativeDestruct() override;

	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	void SetScrollUpdate(UCanvasPanel* canvaspanel, FVector2D diff);

protected:
	void SetScrollUpdate(ScrollInfo::FScrollInfo& info, FVector2D diff, bool bHor);

	FMargin GetComposePanelSlot(UCanvasPanel* panel);
	void SeekDragButton(UCanvasPanel* panel);

protected:
	TArray<ScrollInfo::FScrollInfo>			ScrollHorUsePanelList;
	TArray<ScrollInfo::FScrollInfo>			ScrollVerUsePanelList;

	//UPROPERTY()
	//bool				IsInWidget = false;
	//UPROPERTY()
	//bool				IsDown = true;

	UPROPERTY()
	FVector2D			DownPos;
};
