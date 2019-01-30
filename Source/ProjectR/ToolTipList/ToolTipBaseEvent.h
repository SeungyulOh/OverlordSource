// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ToolTipDefs.h"
#include "ToolTipBaseEvent.generated.h"

class URGameInstance;
class UTooltipEventList;
class UUC_Tooltip;

UCLASS()
class PROJECTR_API UTooltipBaseEvent : public UObject
{
	GENERATED_BODY()

	friend class UTooltipEventList;

public:
	UFUNCTION(BlueprintCallable, Category = UTooltipBaseEvent)
	virtual bool IsShowUI();

	UFUNCTION(BlueprintCallable, Category = UTooltipBaseEvent)
	virtual void ShowUI(FVector2D pos);

	UFUNCTION(BlueprintCallable, Category = UTooltipBaseEvent)
	virtual void HideUI();

	UFUNCTION(BlueprintImplementableEvent, Category = Battle)
	void								OnHideTooltip();

	EToolTipEventTypeEnum			GetTooltipEventType() { return EventType; }
	FText							GetNameText();
	FText							GetDescriptionText();
	FText							GetToolTipTypeText();
	FName							GetEventID() { return EventID; }

protected:
	URGameInstance*					RGameInstance = nullptr;
	EToolTipEventTypeEnum			EventType = EToolTipEventTypeEnum::VE_None;
	FName							EventID;
	FTimerHandle					HideTimer;
	bool							bShowUI = false;

	UPROPERTY()
	TWeakObjectPtr< UUC_Tooltip >	ToolTipUI;

private:
	UFUNCTION()
	void HideTooltip();
};