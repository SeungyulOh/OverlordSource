#pragma once

#include "UI/RBaseWidget.h"
#include "UC_CombineBenefitNoti.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API UUC_CombineBenefitNoti : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
// 	virtual void NativeDestruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void SetData(FText InDescText, FText InValueText, FText InEndText);


	UPROPERTY()
	ULocalizingTextBlock*		TextDesc = nullptr;
	UPROPERTY()
	ULocalizingTextBlock*		TextValue = nullptr;
	UPROPERTY()
	ULocalizingTextBlock*		TextEnd = nullptr;

};