// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "UC_Popup_HeroInfo_Strong_Splash.h"

void UUC_Popup_HeroInfo_Strong_Splash::NativeConstruct()
{
	Super::NativeConstruct();
	Init();
}

void UUC_Popup_HeroInfo_Strong_Splash::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Popup_HeroInfo_Strong_Splash::Init()
{
	
}

void UUC_Popup_HeroInfo_Strong_Splash::SetEnchant(TArray<int32>& InPrev, TArray<int32>& InEnchant)
{
	int32 prevCount = 0;
	for (int32 Num : InPrev)
		prevCount += Num;
	int32 EnchantCount = 0;
	for (int32 Num : InEnchant)
		EnchantCount += Num;

	if(IsValid(Text_PrevEhchant))
		Text_PrevEhchant->SetText(FText::AsNumber(prevCount));
	if (IsValid(Text_NextEhchant))
		Text_NextEhchant->SetText(FText::AsNumber(EnchantCount));
}

FReply UUC_Popup_HeroInfo_Strong_Splash::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	SetVisibility(ESlateVisibility::Collapsed);

	UUIFunctionLibrary::GetGenericScrollview()->SetState(EScrollviewType::VE_HERO_ENCHANT);
	UUIFunctionLibrary::GetGenericScrollview()->Appear(true);

	return FReply::Unhandled();
}
