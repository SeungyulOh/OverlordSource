// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RScrollItemData.h"
#include "Blueprint/UserWidget.h"
#include "UI/RBaseWidget.h"
#include "RScrollItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScrollItemDelegate_SelectedItemEvent, URScrollItem*, SelectScrollItem);

class URScrollView;

UCLASS()
class PROJECTR_API URScrollItem : public URBaseWidget
{
	GENERATED_BODY()

	friend class URScrollView;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollItem)
	int32				Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollItem)
	FVector2D			Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollItem)
	URScrollItemData*	ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollItem)
	bool				IsSizeUpdate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollItem)
	bool				IsSelectEnable = true;

	UPROPERTY(BlueprintReadOnly, Category = ScrollItem)
	bool				IsSelected;

	UPROPERTY(BlueprintReadOnly, Category = ScrollItem)
	URScrollView*		ParentScrollView = nullptr;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ScrollItem)
	FScrollItemDelegate_SelectedItemEvent		OnSelectedItem;

public:
	UFUNCTION(BlueprintCallable, Category = ScrollItem)
	void				SetSizeChange(FVector2D ChangeSize, bool bParentUpdate = true);

	UFUNCTION(BlueprintCallable, Category = ScrollItem)
	virtual void		SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView);

	UFUNCTION()
	virtual void		SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView);

	UFUNCTION()
	virtual void		SetSelected(bool bSelected);

	UFUNCTION(BlueprintImplementableEvent, Category = ScrollItem)
	void				UpdateItemDataState();
	UFUNCTION(BlueprintImplementableEvent, Category = ScrollItem)
	void				Editor_SetSelected(bool bSelected);
	UFUNCTION(BlueprintImplementableEvent, Category = ScrollItem)
	void				Editor_SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView);

	URScrollItem(const class FObjectInitializer &PCIP);

	void		NativeConstruct() override;
	void		NativeDestruct() override;
	void		NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual	void SetParentScrollView(URScrollView* InScrollView);

	void ForceSelect();
	int32 GetItemIndex() { return ItemIndex; }

protected:
	void				ScrollUpdate(const FPointerEvent& InEvent);
	virtual void		Selected();

protected:
	bool				IsTimeEvent = false;
	bool				IsExeTimeEvent = false;
	float				TimeEventTime;
	float				CurTime;
	UPROPERTY()
	FVector2D			DownPos;
	UPROPERTY()
	FVector2D			MovePos;

	int32				ItemIndex = -1;
};
