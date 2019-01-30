// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NotifyDefs.h"
#include "NotifyEvent.h"
#include "NotifyEventList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnLobbyNotify, UNotifyEvent*, notify);

class URGameInstance;

UCLASS()
class PROJECTR_API UNotifyEventList : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE void			Initialize(/*URGameInstance* GameInstance*/) { /*RGameInstance = GameInstance;*/ }
	UNotifyEvent*	AddNotifyEvent(ENotifyEventTypeEnum eventtype);
	void			RemoveNotifyEvent(UNotifyEvent* InNotifyEvent);
	
	void			ClearList();

	void			SetNotifyStateFlag(ENotifyEventTypeEnum InEventType);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UNotifyEventList)
	FDelegate_OnLobbyNotify		OnAddNotify;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UNotifyEventList)
	FDelegate_OnLobbyNotify		OnRemoveNotify;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UNotifyEventList)
	TArray< UNotifyEvent* >			NotifyList;

protected:
// 	UPROPERTY()
// 	URGameInstance*					RGameInstance = nullptr;
	
};