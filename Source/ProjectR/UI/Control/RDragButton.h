// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "RDragButton.generated.h"

class URScrollPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDragButtonDelegate_Down, FVector2D, CurPos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDragButtonDelegate_Up, FVector2D, CurPos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDragButtonDelegate_Move, FVector2D, CurPos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDragButtonDelegate_Clicked, FVector2D, CurPos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDragButtonDelegate_Drag, FVector2D, CurPos);
/**
 * 
 */
UCLASS()
class PROJECTR_API URDragButton : public URBaseWidget
{
	GENERATED_BODY()

	friend class URScrollPanel;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragButton)
	FVector2D			Size;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = DragButton)
	FDragButtonDelegate_Down		OnButtonDown_Delegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = DragButton)
	FDragButtonDelegate_Up			OnButtonUp_Delegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = DragButton)
	FDragButtonDelegate_Move		OnButtonMove_Delegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = DragButton)
	FDragButtonDelegate_Clicked		OnButtonClicked_Delegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = DragButton)
	FDragButtonDelegate_Drag		OnButtonClicked_Drag;

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
	UPROPERTY()
	FVector2D			DownPos;
	UPROPERTY()
	bool				IsScrollEnable = false;
	UPROPERTY()
	URScrollPanel*		ScrollPanel = nullptr;
	UPROPERTY()
	UCanvasPanel*		ParentCanvasPanel = nullptr;
};
