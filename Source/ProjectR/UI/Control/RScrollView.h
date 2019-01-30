// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RScrollItemData.h"
#include "UI/RBaseWidget.h"
#include "UI/Chat/ChatInertialScrollManager.h"
#include "ROverscroll.h"
#include "RScrollView.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScrollViewDelegate_SelectedItemEvent, URScrollItem*, SelectScrollItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScrollViewDelegate_SelectItemDataEvent, URScrollItemData*, SelectItem);
DECLARE_DYNAMIC_DELEGATE_OneParam(FScrollViewDelegate_GetSelectedScrollItemDataEvent, const TArray<URScrollItemData*>&, SelectedScrollItemData);

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EScrollViewTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Grid					UMETA(DisplayName = "Grid"),
	VE_LimitEnable			UMETA(DisplayName = "LimitEnable"),	// variable size element
};

class URScrollBar;
class UControlEventDelegator;
/**
 * 
 */
UCLASS()
class PROJECTR_API URScrollView : public URBaseWidget
{
	GENERATED_BODY()

	friend class URScrollItem;
	
public:
	// Show Details Panel UPROPERTY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	bool					TimeEvent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	float					TimeEventTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	bool					InitFirstSelect = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	bool					Horizontal = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	bool					AutoSizeEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	bool					MultiSelect = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	bool					CheckSelect = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	bool					IsSelectEnable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	bool					ForceScrollAble = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	FVector2D				InitScale = FVector2D::UnitVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	EScrollViewTypeEnum		SpecialtyType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	int32					GridCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	FVector2D				ScrollViewSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	TArray<URScrollItem*>	ScrollItemList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	TArray<URScrollItemData*>	ItemDataList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ScrollView)
	TSoftClassPtr<class URScrollItem>		ScrollItemBpClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ScrollView)
	UControlEventDelegator*		EventDelegator;

	/** Style of the scrollbar */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ScrollBar", meta=( DisplayName="ScrollBarStyle" ))
	FScrollBarStyle ScrollWidgetStyle;

	UPROPERTY()
	USlateWidgetStyleAsset* ScrollStyle_DEPRECATED;

	/**  */
	UPROPERTY(EditAnywhere, Category="ScrollBar")
	bool bAlwaysShowScrollbar;

	/**  */
	UPROPERTY(EditAnywhere, Category="ScrollBar")
	TEnumAsByte<EOrientation> ScrollOrientation;

	/** The thickness of the scrollbar thumb */
	UPROPERTY(EditAnywhere, Category="ScrollBar")
	FVector2D ScrollThickness;
public:

	UPROPERTY(BlueprintReadOnly, Category = ScrollView)
	UCanvasPanel*			BGPanel = nullptr;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ScrollView)
	FScrollViewDelegate_SelectItemDataEvent		OnScrollViewSelectedItemData;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ScrollView)
	FScrollViewDelegate_SelectedItemEvent		OnScrollViewSelectedItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ScrollView)
	FScrollViewDelegate_SelectItemDataEvent		OnScrollViewTimeEventItemData;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = ScrollView)
	FScrollViewDelegate_SelectedItemEvent		OnScrollViewTimeEventItem;


	UPROPERTY()
	FScrollViewDelegate_GetSelectedScrollItemDataEvent	OnGetSelectedScrollItemDataEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scrollview)
	bool					bShowTooltip = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	bool					IsSpawnAni = false;

public:
	URScrollView(const class FObjectInitializer &PCIP);

	///////////////////////////////////////////////////////////////////////////////////////////
	// ItemDataList Element
	UFUNCTION(BlueprintCallable, Category = ScrollView)
	int32					GetScrollItemDataCount();
	UFUNCTION(BlueprintCallable, Category = ScrollView)
	void					ClearScrollItemData(int32 InSize = 0);
	FORCEINLINE TArray<URScrollItemData*>&	GetScrollItemAll() { return ItemDataList; }

	UFUNCTION(BlueprintCallable, Category = ScrollView)
	URScrollItemData*		AddScrollItemData(TSubclassOf<class URScrollItemData> dataClass);
	void					RemoveScrollItemData(URScrollItemData* dataClass);
	void					UpdateScrollItemData(URScrollItemData* dataClass);

	URScrollItemData*		CreateScrollItemData(TSubclassOf<class URScrollItemData> dataClass);
	UFUNCTION(BlueprintCallable, Category = ScrollView)
	void					RefreshScrollItemData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = ScrollView)
	void					SetData(TArray<URScrollItemData*>& list);
	void					SetData_Implementation(TArray<URScrollItemData*>& list);
	void					AddItemDataList(URScrollItemData* InScrollItemData);

	UFUNCTION(BlueprintCallable, Category = ScrollView)
	virtual void			Init();

	/*
	this function is almost the same with Init function except for removing array of ScrollItemList and ItemDataList 
	we use this function when we know how many scrollItem we want to add and reuse them for optimization.
	*/
	void					Init_NoClear(int32 InCreateCount);

	UFUNCTION(BlueprintCallable, Category = ScrollView)
	void					InvalidateData();
	
	void					InvalidateDataWithoutResetSelect();

	void					UpdateTargetScrollItem(URScrollItem* InTargetScrollItem);
	void					ScrollItemList_RemoveElement(URScrollItem* InElement);
	void					SetSizeChangeUpdate();
	void					SetDisable(bool bDisable) override;
	void					Refresh();

	UFUNCTION(BlueprintCallable, Category = ScrollView)
	void					ScrollToItemAnimation(int32 ScrollItemDataIndex);
	void					ComputeSpawnAni(int32 InIdx);

	void					SetIsCustomItemSize(bool InValue) { IsCustomItemSize = InValue; }

	FORCEINLINE				bool IsScrolling() { return bScrolling; }
	FORCEINLINE				FVector2D GetItemInitSize() { return ItemInitSize; }
	void					Force_DeSelect(URScrollItem* SelecteItem);
	
protected:
	void					NativeConstruct() override;
	void					NativeDestruct() override;
	void					ChangeWidgetSize() override;

	void					GetListPosByIndex(int32 Index, FIntPoint& GridPos) const;

	///////////////////////////////////////////////////////////////////////////////////////////
	// ScrollItemList Element
	void					ScrollItemList_AddElement(int32 InElementNumber = 1);
	void					ScrollItemList_RemoveAllElement(int32 InSize = 0);
	void					ScrollItemList_SetItemDataList(bool bResetSelectList = true);
	virtual void			ScrollItemList_ChangeItemDataList(int32 dataIndex);

public:
	///////////////////////////////////////////////////////////////////////////////////////////
	// ScrollItemList Scroll
	void					SetScrollUpdate(URScrollItem* CurItem, const FPointerEvent& InEvent);
	virtual void			SetScrollUpdate(URScrollItem* CurItem, FVector2D diff);
	void					SetScrollUpdate(float fMoveValue);

	virtual void			ClearAllSelected();
protected:
	///////////////////////////////////////////////////////////////////////////////////////////
	// ScrollItemList Select

	// Called on Selected event on a scroll item
	// *toggle* the Selected status of an item and its data
	virtual void			SetSelected(URScrollItem* SelecteItem);
	void					SelectInvalidateData();

	virtual void			SetTimeEvent(URScrollItem* SelecteItem);

	///////////////////////////////////////////////////////////////////////////////////////////
	// Tooltip
	void					SetTooltipInfo(URScrollItem* InItem);

	///////////////////////////////////////////////////////////////////////////////////////////
	// Scroll Animation

	bool bScrolling;

	float TickScrollDelta;

	// Scroll flags
	bool bUserScrolling;
	float AmountScrolledByUser;

	float DragTriggerDistance;

	// Inertia state
	bool bIsScrollingActiveTimerRegistered;
	FChatInertialScrollManager InertialScrollManager;

	// ScrollToItemAnimation state
	bool bAnimating;
	float DesiredScrollOffset;

	// Overscroll
	FROverscroll Overscroll;
	float CalculateMoveDist() const;

	void ResetScrollAnimationState();

	void ScrollItemOnTouchStarted();
	void ScrollItemOnTouchMoved(const FPointerEvent& InEvent);
	void ScrollItemOnTouchEnded();

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	float GetScrollComponentFromVector(FVector2D Vector) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScrollView)
	float					InertialScrollDecceleration = 2048.f;

	// To be called before ScrollItemList_ChangeItemDataList in order to minimize number of invalidate
	void ShiftScrollItemList(int32 dataIndex);
	void ScrollItemList_SwapRow(int32 a, int32 b);

	FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	// DEBUG
	void PrintScrollItemList();

public:
	URScrollItem*			GetScrollItem(int32 ListIndex);
	void					SetSelectScrollListIndex(int32 ListIndex);
	void					SetSelectItemDataListIndex(int32 ListIndex);
	void					SetSelectScrollListData(URScrollItemData* SelectItemData);
	void					SetScrollPos(URScrollItemData* SelectItemData);


	UFUNCTION(BlueprintCallable, Category = ScrollView)
	TArray<int32>&				GetSelectedScrollItem();
	UFUNCTION(BlueprintCallable, Category = ScrollView)
	bool						IsSelectedScrollItem();
	UFUNCTION(BlueprintCallable, Category = ScrollView)
	TArray<URScrollItemData*>&	GetSelectedScrollItemData();
	UFUNCTION(BlueprintCallable, Category = ScrollView)
	URScrollItemData*			GetSelectedScrollItemDataOnly();


	UClass*						GetScrollItemBpClass();

protected:
	UPROPERTY()
	TArray<int32>				SelectedScrollItemList;
	UPROPERTY()
	TArray<URScrollItemData*>	SelectedScrollItemDataList;	
	//temp from base scroll view
	
	float					CurScrollOffset;
	bool					IsScrollAble;
	int32					CreateCount;
	int32					VisibleMaxCount;
	int32					VisibleCreateCount;
	int32					StartIdx;
	UPROPERTY()
	FVector2D				ItemInitSize;

	UPROPERTY()
	URScrollBar*			ScrollBar = nullptr;

	bool					IsFirstInit = true;
	bool					IsCustomItemSize = false;
	bool					bInitNoClear = false;
};