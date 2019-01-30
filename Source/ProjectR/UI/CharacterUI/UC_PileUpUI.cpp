

#include "ProjectR.h"
#include "UC_PileUpUI.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"
void UUC_PileUpUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_PileUpUI::SetStackProgress(int32 InCurrentCount, int32 InMaxCount)
{
	float fPercent = (float)InCurrentCount / (float)InMaxCount;

	PileUpProgressBar->SetPercent(fPercent);
	if (InCurrentCount == InMaxCount)
	{
		PlayStackFullAni(true);
	}
	else
	{
		PlayStackFullAni(false);
	}
}

void UUC_PileUpUI::PlayStackFullAni(bool bPlay)
{
	if (bPlay)
	{
		const int32 nIndefinitelyNum = 0;
		PlayAni("StackFullAni", false, nIndefinitelyNum);
		OutLine->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		StopAni("StackFullAni");
		OutLine->SetVisibility(ESlateVisibility::Collapsed);
	}
}

