


#include "ProjectR.h"
#include "UC_CombineBenefitNoti.h"


void UUC_CombineBenefitNoti::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_CombineBenefitNoti::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUC_CombineBenefitNoti::SetData(FText InDescText, FText InValueText, FText InEndText)
{
	TextDesc->SetText(InDescText);
	TextValue->SetText(InValueText);
	TextEnd->SetText(InEndText);
}

