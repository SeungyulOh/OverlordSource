// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NotifyDefs.h"
#include "NotifyEvent.h"
#include "NotifyEventUnlockContents.generated.h"

UCLASS()
class PROJECTR_API UNotifyEventUnlockContents : public UNotifyEvent
{
	GENERATED_BODY()

public:
	void	SetUnlockKey(FName key);
	bool	GoShortCuts() override;

protected:
	UPROPERTY()
	FName				UnlockTableKey;
};