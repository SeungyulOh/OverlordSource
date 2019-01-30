// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UIDefs.h"
#include "Blueprint/UserWidget.h"
#include "ToolTipList/ToolTipDefs.h"
#include "RBaseWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_UIOneParamEvent, int32, value);

#define UIMOVE_DIFF		10.0f
/**
 * 
 */
UCLASS()
class PROJECTR_API URBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
//	static float DpiScale;
	static float GetDPIScale();
public:
	URBaseWidget(const class FObjectInitializer &PCIP);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
//	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	static void GatherSWidgetAllChildren(TSharedRef<SWidget> Root, TSet<TSharedRef<SWidget>>& Children);

	static void SetCanvasPanelZOrderOffset(UWidget* widget, int32 OffsetZOrder);

	//static void SetZOrderOffset(UUserWidget* userwidget, int32 OffsetZOrder);
	//static int32 GetZOrder(UUserWidget* userwidget);
	//static void SetZOrder(UUserWidget* userwidget, int32 ZOrder);

	static void SetCanvasPanelSize(UWidget* widget, FVector2D size);
	static void SetCanvasPanelPos(UWidget* widget, FVector2D pos);
	static void SetCanvasPanelXPos(UWidget* widget, float xPos);
	static void SetCanvasPanelYPos(UWidget* widget, float yPos);
	static void SetCanvasPanelXOffset(UWidget* widget, float xOffset);
	static void SetCanvasPanelYOffset(UWidget* widget, float yOffset);
	static void SetCanvasPanelOffset(UWidget* widget, FVector2D Offset);
	static void GetHaveWidget(UWidget* widget, TArray<UWidget*>& outlist);

	static void SetFocus(UWidget* widget, UWorld* world);

	void GetHaveWidget(TArray<UWidget*>& outlist);
	virtual void RefreshWidgetSize();
	void UpdateWidgetSize(const FGeometry& MyGeometry, bool bCalSize = false);
	virtual void ChangeWidgetSize() {}
	virtual void SetActiveSubPage() {}

	bool UpdateFindWidget(const FGeometry& MyGeometry, UWidget* InParentWidget, UWidget* InWidget, FVector2D& location, FVector2D& size);
	bool GetWidgetPosSize(const FGeometry& MyGeometry, UWidget* InWidget, FVector2D& location, FVector2D& size);
	bool GetAbsoluteRWidgetPos(URBaseWidget* InRWidget, FVector2D& location);
	UWidget* GetParentWidget(UWidget* InChildWidget, FString name);

	//Get UWIdgetAnimation from name
	UWidgetAnimation* GetWidgetAnimFromName(FString _name);
	
	virtual void SetParentWidget(URBaseWidget* widget);
	FORCEINLINE URBaseWidget* GetParentWidget()				{ return ParentWidget; }
	FORCEINLINE void SetIsUpdateWidgetSize(bool InIsUpdate) { IsUpdateWidgetSize = InIsUpdate; }

	UFUNCTION(BlueprintCallable, Category = RBaseWidget)
	bool PlayAni(FString name, bool bReverse = false, int32 num = 1);
	UFUNCTION(BlueprintCallable, Category = RBaseWidget)
	bool PlayAniFromSpecificStartTimeToEndTime(FString name, float StartTime, float EndTime, bool bReverse = false);
	UFUNCTION(BlueprintCallable, Category = RBaseWidget)
	void StopAni(FString name);
	UFUNCTION(BlueprintCallable, Category = RBaseWidget)
	void ResetAni(FString name);
	UWidgetAnimation* GetAni(FString name);
	bool IsPlayAni(FString name);

	void EventClear(TArray<FString>& exceptList, TArray<UWidget*>& exceptWidgetList);

	virtual void SetDisable(bool bDisable);

//	FORCEINLINE bool				IsTickEnter()	{ return IsTick; }
	FORCEINLINE const FGeometry& 	GetGeometry()	{ return CustomGeometry; }
	//FORCEINLINE void SetInitActive(bool InOn)		{ IsInitActive = InOn; }

	UFUNCTION(BlueprintCallable, Category = RBaseWidget)
	virtual void InvalidateData();

	UFUNCTION(BlueprintCallable, Category = RBaseWidget)
	virtual void RefreshLocalizingText();

//***************************************************************
// Template
//***************************************************************
	template<class T>
	T* FindParentRBaseWidget(URBaseWidget* InBaseWidget)
	{
		if (IsValid(InBaseWidget))
		{
			T* castBaseWidget = Cast<T>(InBaseWidget);
			if (IsValid(castBaseWidget))
			{
				return castBaseWidget;
			}

			if (IsValid(InBaseWidget->GetParentWidget()))
			{
				return FindParentRBaseWidget<T>(InBaseWidget->GetParentWidget());
			}
		}

		return nullptr;
	}

	template<class T>
	static T* FindChildNameWidget(UWidget* widget, FString name = TEXT(""))
	{
		if (IsValid(widget))
		{
			T* castWidget = Cast<T>(widget);
			if (IsValid(castWidget))
			{
				if (name.Len() > 0)
				{
					if (castWidget->GetFName().ToString().Contains(name))
					{
						return castWidget;
					}
				}
				else
				{
					return castWidget;
				}
			}

			UUserWidget* userWidget = Cast<UUserWidget>(widget);
			if (IsValid(userWidget) && IsValid(userWidget->WidgetTree))
			{
				TArray<UWidget*> widgetlist;
				userWidget->WidgetTree->GetAllWidgets(widgetlist);
				for (UWidget* widgetChild : widgetlist)
				{
					T* castWidgetChild = Cast<T>(widgetChild);
					if (IsValid(castWidgetChild))
					{
						if (name.Len() > 0)
						{
							if (castWidgetChild->GetFName().ToString().Contains(name))
							{
								return castWidgetChild;
							}
						}
						else
						{
							return castWidgetChild;
						}
					}

					UUserWidget* userWidgetChild = Cast<UUserWidget>(widgetChild);
					if (IsValid(userWidgetChild))
					{
						T* castWidgetRt = FindChildNameWidget<T>(userWidgetChild, name);

						if (IsValid(castWidgetRt))
							return castWidgetRt;
					}
				}
			}
		}

		return nullptr;
	}

	template<class T>
	static T* FindChildWidgetWithCorrectName(UWidget* widget, FString name = TEXT(""))
	{
		if (IsValid(widget))
		{
			T* castWidget = Cast<T>(widget);
			if (IsValid(castWidget))
			{
				if (name.Len() > 0)
				{
					if (castWidget->GetFName().ToString() == name)
					{
						return castWidget;
					}
				}
				else
				{
					return castWidget;
				}
			}

			UUserWidget* userWidget = Cast<UUserWidget>(widget);
			if (IsValid(userWidget) && IsValid(userWidget->WidgetTree))
			{
				TArray<UWidget*> widgetlist;
				userWidget->WidgetTree->GetAllWidgets(widgetlist);
				for (UWidget* widgetChild : widgetlist)
				{
					T* castWidgetChild = Cast<T>(widgetChild);
					if (IsValid(castWidgetChild))
					{
						if (name.Len() > 0)
						{
							if (castWidgetChild->GetFName().ToString() == name)
							{
								return castWidgetChild;
							}
						}
						else
						{
							return castWidgetChild;
						}
					}

					UUserWidget* userWidgetChild = Cast<UUserWidget>(widgetChild);
					if (IsValid(userWidgetChild))
					{
						T* castWidgetRt = FindChildWidgetWithCorrectName<T>(userWidgetChild, name);

						if (IsValid(castWidgetRt))
							return castWidgetRt;
					}
				}
			}
		}

		return nullptr;
	}

	template<class T>
	static T* FindChildWidget(UWidget* widget, FString name = TEXT(""))
	{
		if (IsValid(widget))
		{
			T* castWidget = Cast<T>(widget);
			if (IsValid(castWidget))
			{
				if (name.Len() > 0)
				{
					if (castWidget->GetFName().ToString().Contains(name))
					{
						return castWidget;
					}
				}
				else
				{
					return castWidget;
				}
			}

			UPanelWidget* panelWidget = Cast<UPanelWidget>(widget);
			if (IsValid(panelWidget))
			{
				for (int32 i = 0; i < panelWidget->GetChildrenCount(); ++i)
				{
					UWidget* child = panelWidget->GetChildAt(i);
					if (IsValid(child))
					{
						castWidget = FindChildWidget<T>(child, name);

						if (IsValid(castWidget))
							return castWidget;
					}
				}
			}
		}

		return nullptr;
	}

	template<class T>
	static void FindChildWidgets(TArray<T*>& outList, UWidget* widget, FString name = TEXT(""))
	{
		if (IsValid(widget))
		{
			T* castWidget = Cast<T>(widget);
			if (IsValid(castWidget))
			{
				if (name.Len() > 0)
				{
					if (castWidget->GetFName().ToString().Contains(name))
					{
						outList.Emplace(castWidget);
					}
				}
				else
				{
					outList.Emplace(castWidget);
				}
			}

			UPanelWidget* panelWidget = Cast<UPanelWidget>(widget);
			if (IsValid(panelWidget))
			{
				for (int32 i = 0; i < panelWidget->GetChildrenCount(); ++i)
				{
					UWidget* child = panelWidget->GetChildAt(i);
					if (IsValid(child))
					{
						FindChildWidgets<T>(outList, child, name);
					}
				}
			}
		}
	}

	template<class T>
	static void FindChildWidgets(UUserWidget* parent, TArray<T*>& outList)
	{
		if (outList.Num() != 0)
			return;

		if (IsValid(parent) && IsValid(parent->WidgetTree))
		{
			TArray<UWidget*> widgetlist;
			parent->WidgetTree->GetAllWidgets(widgetlist);
			for (UWidget* widget : widgetlist)
			{
				T* castWidget = Cast<T>(widget);
				if (IsValid(castWidget))
				{
					outList.Emplace(castWidget);
				}

// 				UUserWidget* userWidget = Cast<UUserWidget>(widget);
// 				if (IsValid(userWidget))
// 				{
// 					FindChildWidgets<T>(userWidget, outList);
// 				}
			}
		}
	}

	template<class T>
	static void FindChildWidgetsWithCertainName(UUserWidget* parent, TArray<T*>& outList, FString name)
	{
		if (outList.Num() != 0)
			return;

		if (IsValid(parent) && IsValid(parent->WidgetTree))
		{
			TArray<UWidget*> widgetlist;
			parent->WidgetTree->GetAllWidgets(widgetlist);
			for (UWidget* widget : widgetlist)
			{
				T* castWidget = Cast<T>(widget);
				if (IsValid(castWidget))
				{
					if(castWidget->GetName().Contains(name))
						outList.Emplace(castWidget);
				}

// 				UUserWidget* userWidget = Cast<UUserWidget>(widget);
// 				if (IsValid(userWidget))
// 				{
// 					FindChildWidgetsWithCertainName<T>(userWidget, outList, name);
// 				}
			}
		}
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBaseWidget)
	FMargin			WidgetSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBaseWidget)
	FVector2D		WidgetRealSize;

	UPROPERTY()
	FVector2D		WidgetRealPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBaseWidget)
	TArray<UWidgetAnimation*>		WidgetAnimationList;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBaseWidget)
// 	TArray<URBaseWidget*>			ChildBaseWidgetList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBaseWidget)
	EToolTipEventTypeEnum			ToolTipEvent = EToolTipEventTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBaseWidget)
	FName							ToolTipID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RBaseWidget)
	bool							IsDisabled = false;

	//Delegate
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = RBaseWidget)
	FDelegate_UIOneParamEvent		OnSendOneParamEvent;

protected:
	bool				IsInWidget;
	bool				IsDown;
	bool				IsMoved;
	bool				IsUpdateWidgetSize = false;
	bool				IsToolTipShow = false;
	
	
	FVector2D			DownPos;
	FVector2D			MovePos;

	UPROPERTY()
	URBaseWidget*		ParentWidget = nullptr;
	
	UPROPERTY()
	FGeometry			CustomGeometry;

	UPROPERTY()
	float				ElapsedTimeOfTooltip = 0.0f;

	bool				IsTick = false;
	bool				IsInitActive = false;
};