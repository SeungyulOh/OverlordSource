// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_Tooltip.generated.h"

class UTooltipBaseEvent;
class UUC_Item_Icons;
class URenewal_BaseHeroIcon;
class UBaseMonsterIcon;
class URBuffIcon;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Tooltip : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetTooltipEvent(UTooltipBaseEvent* tooltip, FVector2D InPos);
	void HideTooltip();
	bool GetShow();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UTextBlock*				LocalizingTextBlock_Name = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UTextBlock*				LocalizingTextBlock_Description = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UTextBlock*				TextBlock_Num = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UPanelWidget*			NumPanel_Widget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UImage*					BG_Image = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UWidgetSwitcher*		WidgetSwitcher_Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UTextBlock*				LocalizingTextBlock_ItemText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UUC_Item_Icons*			UC_Item_Icons = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	URenewal_BaseHeroIcon*	UC_Icon_Hero = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UBaseMonsterIcon*		UC_Icon_Monster = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	float					ToolTipBaseWidth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	FVector2D				OffsetPos = FVector2D::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	UWidgetSwitcher*		Info_WidgetSwitcher = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	TArray<URBuffIcon*>		WeakIconList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Tooltip)
	TArray<URBuffIcon*>		ImmuneIconList;
private:
	UPROPERTY()
	UTooltipBaseEvent*		TooltipBaseEvent = nullptr;
	
	bool					bShow = false;
	UPROPERTY()
	FVector2D				PrevPos = FVector2D::ZeroVector;
	UPROPERTY()
	FVector2D				LastPos = FVector2D::ZeroVector;
	UPROPERTY()
	FTimerHandle			TooltipHandler;

	UFUNCTION()
	void Show();

	void SetPosition(const FGeometry& MyGeometry);
};
