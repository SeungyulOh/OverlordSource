// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_ResultEffect.h"


void UUC_ResultEffect::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_ResultEffect::SetState(EResultEffectEnum InState)
{
	switch (InState)
	{
	case EResultEffectEnum::VE_Victory:
		PlayAni(TEXT("Victory"));
		PlayAni(TEXT("NoneStar"));
		break;
	case EResultEffectEnum::VE_Defeat:
		PlayAni(TEXT("Defeated"));
		break;
	case EResultEffectEnum::VE_Draw:
		PlayAni(TEXT("Draw"));
		break;
	case EResultEffectEnum::VE_Star1:
		PlayAni(TEXT("Victory"));
		PlayAni(TEXT("Star1"));
		break;
	case EResultEffectEnum::VE_Star2:
		PlayAni(TEXT("Victory"));
		PlayAni(TEXT("Star2"));
		break;
	case EResultEffectEnum::VE_Star3:
		PlayAni(TEXT("Victory"));
		PlayAni(TEXT("Star3"));
		break;
	}
}

void UUC_ResultEffect::ShowVictoryText(bool bShow)
{
	if (IsValid(Text_Win))
		Text_Win->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}