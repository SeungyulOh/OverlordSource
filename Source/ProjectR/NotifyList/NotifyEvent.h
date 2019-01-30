// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NotifyDefs.h"
#include "NotifyEvent.generated.h"

class URGameInstance;
class UNotifyEventList;

UCLASS()
class PROJECTR_API UNotifyEvent : public UObject
{
	GENERATED_BODY()

	friend class UNotifyEventList;

public:
	UFUNCTION(BlueprintCallable, Category = UNotifyEvent)
	virtual bool GoShortCuts();

	FORCEINLINE ENotifyEventTypeEnum			GetNotifyEventType() { return EventType; }

protected:
	UPROPERTY()
	URGameInstance*					RGameInstance = nullptr;

	UPROPERTY()
	ENotifyEventTypeEnum			EventType = ENotifyEventTypeEnum::VE_None;
};