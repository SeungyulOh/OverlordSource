#pragma once

#include "UI/RBaseWidget.h"
#include "UC_MaterialSpinBox.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaterialSpinBoxValueChanged, int32 /* New Value */);

class UButton;
class UTextBlock;
class USlider;

/**
* LEGACY COMMENT - because of the UMG bug in 4.18, cannot implement the spinbox as intended by Design (https://issues.unrealengine.com/issue/UE-50329)
*/
UCLASS()
class PROJECTR_API UUC_MaterialSpinBox : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	// invalidates itself from events
	void InvalidateData() override;

	//// Interface

	int32 ValueMin = 0;
	int32 ValueMax = 0;
	int32 CountMaterialOwned = 0; // for displaying Text_Count "2900 / 19000"

	int32 CurrentValue = 0;

	FOnMaterialSpinBoxValueChanged OnValueChanged;

	//// BP Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_MaterialSpinBox)
	UTextBlock* Text_Count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_MaterialSpinBox)
	USlider* Slider_Count;

	////

	UFUNCTION()
	void OnSliderValueChanged(float Value);
};
