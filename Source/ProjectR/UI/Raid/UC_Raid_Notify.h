// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Raid_Notify.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Raid_Notify : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void Notify(FName key);
	UFUNCTION()
	void OnRealTimeModNotify(FText msg);

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Raid_Notify)
	UPanelWidget*			Panel_Canvas = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Raid_Notify)
	UTextBlock*				TextMsg = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Raid_Notify)
	UWidgetAnimation*		Notify_Ref = nullptr;
};