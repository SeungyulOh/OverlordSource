#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UP_BattleRepeatStatePopup.generated.h"


class UUC_Item_Icons;
class UCheckBox_Radio;
/**
* BP: UP_AutoPlay
* WBP_AutoPlayPopup
*/
UCLASS()
class PROJECTR_API UUP_BattleRepeatStatePopup : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = UUP_BattleRepeatStatePopup)
	void OnClickStateButton();
	
	UFUNCTION(BlueprintCallable, Category = UUP_BattleRepeatStatePopup)
	void OnCheckRepeatCountSelect(int32 InCount);
	
	UFUNCTION(BlueprintCallable, Category = UUP_BattleRepeatStatePopup)
	void OnClickOK();

	UFUNCTION(BlueprintCallable, Category = UUP_BattleRepeatStatePopup)
	void OnClickCancel();

	UFUNCTION(BlueprintCallable, Category = UUP_BattleRepeatStatePopup)
	void ShowAutoPlayPopup(EBattleRepeatPopupState InState);

	UFUNCTION()
	void AfterCheckStateChanged(bool isChecked);

	void InvalidateRewardData();
	void AutoPlayCountStart();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UWidgetSwitcher*			PopupTypeSwitcher = nullptr;	// 0 : gain item state, 1 : repeat count select
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	USizeBox*					CancelButton_Area = nullptr;


	//////////////////////////////////////////////////////////////////////////
	// info popup - item
	//////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UCanvasPanel*				InfoPopup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UTextBlock*					Exp_Text = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UTextBlock*					Gold_Text = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	TArray<UUC_Item_Icons*>		RuneRewardList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	TArray<USizeBox*>			RuneRewardSizeBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	TArray<UTextBlock*>			CollectItemCountList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UTextBlock*					Text_Caption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UTextBlock*					DescriptionText;

	//////////////////////////////////////////////////////////////////////////
	// info popup - count select
	//////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	TArray<UCheckBox_Radio*>	CountCheckList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	TArray<UWidgetSwitcher*>	CheckedSwitcherList;

	//////////////////////////////////////////////////////////////////////////
	// repeat state button
	//////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UButton*					RepeatStateButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UButton*					Button_Cancel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UButton*					Button_OK = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UImage*						NowPlayImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UWidgetSwitcher*			RepeatStateSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_BattleRepeatStatePopup)
	UTextBlock*					CountDownText = nullptr;

private:
	EBattleRepeatPopupState		PopupState;
	int32						nSelectedRepeatCount = 0;

	bool								bAutoPlayCount = false;
	int64								nCurrentTime = 0;
	int32								nRemainTime = 0;
};
