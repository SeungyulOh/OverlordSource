// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroGrade.h"

void UUC_HeroGrade::Refresh(int32 Grade)
{
	Image_0->SetVisibility(Grade >= 1 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	Image_1->SetVisibility(Grade >= 2 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	Image_2->SetVisibility(Grade >= 3 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	Image_3->SetVisibility(Grade >= 4 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	Image_4->SetVisibility(Grade >= 5 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}
