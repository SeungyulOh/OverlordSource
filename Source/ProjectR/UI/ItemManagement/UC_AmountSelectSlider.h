#pragma once

#include "EngineMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_AmountSelectSlider.generated.h"


/**
*
*/
UCLASS()
class PROJECTR_API UUC_AmountSelectSlider : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	int32 CurrentCount = 0;
	int32 MaxCount = 0;

	void InvalidateData() override;

	DECLARE_DELEGATE_OneParam(FOnCountChanged, int32 /* NewCount */);
	FOnCountChanged OnCountChangedDelegate;
	
	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	USlider* Slider_SelectedAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UButton* Button_Decrease;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UButton* Button_Increase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UButton* Button_Max;

	// delegate functions

	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnButtonSliderMinusClicked();
	UFUNCTION()
	void OnButtonSliderPlusClicked();
	UFUNCTION()
	void OnButtonMaxClicked();

private:
	void InvalidateOnCountChanged();
};
