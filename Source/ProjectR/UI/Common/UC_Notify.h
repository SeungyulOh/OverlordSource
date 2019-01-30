// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_Notify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Notify : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void CheckNotify();

	UFUNCTION(BlueprintCallable, Category = UUC_Notify)
	void SetVisibleByNotify(ENotifyStateFlagEnum InNotifyState, bool bState);

	void SetNotify();

	void SetNotifyState(ENotifyStateFlagEnum InNotifyState) { NotifyState = InNotifyState; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Notify)
	ENotifyStateFlagEnum	NotifyState;
};
