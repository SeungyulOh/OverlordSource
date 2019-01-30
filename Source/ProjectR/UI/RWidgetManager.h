// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UI/UIDefs.h"
#include "SharedConstants/GlobalConstants.h"
#include "RWidgetManager.generated.h"

USTRUCT(BlueprintType)
struct FUIEventInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FUIEventInfo)
	EUIEventEnum		EventType = EUIEventEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FUIEventInfo)
	int32				IValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FUIEventInfo)
	float				FValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FUIEventInfo)
	FString				StringValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FUIEventInfo)
	FString				StringValue1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FUIEventInfo)
	FName				NameValue;

};

/**
 * 
 */
UCLASS()
class PROJECTR_API URWidgetManager : public UObject//, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rwidgetmanager")
	TArray<FUIEventInfo> UIEventQueue;

	//temp heromanager event
	UPROPERTY()
	EItemEquipPositionEnum CacheEquipPosition;

public:
	//////////////////////////////////////////////////////////////////////////
	// Widget Manager
	void			Initialize(void);
	FString			GetWBPEnumAsString(EWidgetBluePrintEnum eWBP);
	FName			GetWBPEnumAsName(EWidgetBluePrintEnum eWBP);

	UUserWidget*	CreateUserWidget(EWidgetBluePrintEnum eWBP);
	UUserWidget*	GetUserWidget(EWidgetBluePrintEnum eWBP);

	template<class T>
	T*		GetUserWidgetT(EWidgetBluePrintEnum eWBP)
	{
		UUserWidget* userWidget = GetUserWidget(eWBP);
		return Cast<T>(userWidget);
	}

	void			ShowUserWidget(EWidgetBluePrintEnum eWBP, bool bForceOrder = false, int32 order = 0);
	void			HideUserWidget(EWidgetBluePrintEnum eWBP);
	bool			IsShowUserWidget(EWidgetBluePrintEnum eWBP);
	void			RemoveAllUserWidget();

	void			SetUIVisibility(bool bVisible);

	void			ShowUserWidgetAutoCreate(EWidgetBluePrintEnum eWBP, bool bForceOrder = false, int32 order = 0);
	UUserWidget*	GetUserWidgetAutoCreate(EWidgetBluePrintEnum eWBP);

	void			AddToViewport(EWidgetBluePrintEnum InWidgetEnum, UUserWidget* InWidget, int32 ZOrder);

	void			ChangeInputModeUIOnly(bool bRes);
	FORCEINLINE bool			IsInputModeUI() { return bInputModeUI; }

	//==================================================================================================
	// UIEvent
	//==================================================================================================
	void			AddUIEvent(EUIEventEnum InEventType);
	void			AddUIEventInt(EUIEventEnum InEventType, int32 InValue);
	void			AddUIEventString(EUIEventEnum InEventType, FString InValue, int32 InIValue = 1);
	void			AddUIEventName(EUIEventEnum InEventType, FName InValue);
	void			RemoveUIEvent(EUIEventEnum InEventType);
	void			RemoveAllUIEvent();
	bool			IsInUIEvent(EUIEventEnum InEventType);
	FUIEventInfo*	GetUIEventInfo(EUIEventEnum InEventType);


	EUIPageEnum GetCurrentUIPage();
	EUIPageEnum GetPrevUIPage();
	
	// Linked UI (Prev - Current - Next, LobbyUI)
	UFUNCTION(BlueprintCallable, Category = WidgetManager)
	void ChangeUIPage(EUIPageEnum Targetpage);
	
	// Independent UI (InGameUI, PopupUI)
	UFUNCTION(BlueprintCallable, Category = WidgetManager)
	void AttachUIPage(EUIPageEnum page);
	UFUNCTION(BlueprintCallable, Category = WidgetManager)
	void DetachUIPage(EUIPageEnum page);
	
	//todo: test
	void RefreshVisibleUI();
	void RefreshLocalizingTextForVisibleUI();

	void AddLocalizingKey(FName key, EStringTableType type);
	bool GetLocalizingKeyAndType(FText Intext, FName& key, EStringTableType& type);


	void ExceptionRTSError();
	UFUNCTION()
	void RTSError();

	FORCEINLINE EUIStackEventEnum GetStackEvent()				{ return StackEvent; }
	FORCEINLINE void SetStackEvent(EUIStackEventEnum InEvent)	{ StackEvent = InEvent; }


private:
	void ShowUIPage(EUIPageEnum page);
	void HideUIPage(EUIPageEnum page);

	
private:
	void RefreshLocalizingText(UUserWidget* InUswerWidget);

	UPROPERTY()
	TMap<FName, EStringTableType> LocalizingKeyMaps;
	
	UPROPERTY()
	TMap<EWidgetBluePrintEnum, UUserWidget*> UserWidgetMap;
	// UI Page Stack
	UPROPERTY()
	TArray<EUIPageEnum> UIPageStack;

	int32				ViewPortOrder = 0;
	bool				bInputModeUI = false;
	
	UPROPERTY()
	EUIStackEventEnum	StackEvent = EUIStackEventEnum::VE_None;

	UPROPERTY()
	const UEnum*		EWidgetBluePrintEnumPtr = nullptr;
};
