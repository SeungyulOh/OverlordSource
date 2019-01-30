// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Popup_Splash_AttackPower.h"
// 
// #include "UI/RWidgetManager.h"

// #include "GameInfo/UserInfo.h"
// #include "Table/LevelGrowth.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"

void UUP_Popup_Splash_AttackPower::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bPlayStart)
	{

		if (!IsPlayAni("Splash_Ani"))
		{
			this->SetVisibility(ESlateVisibility::Collapsed);
			bPlayStart = false;
		}
	}
	if(bTextAni)
	{
		ChangeValueTick(InDeltaTime);
	}
}

void UUP_Popup_Splash_AttackPower::RefreshUI(FName InHeroID, int32 InCurrentPower, int32 InPrevPower)
{
	bool bIncrease = (InCurrentPower - InPrevPower) > 0;
	ResetValue();

	if (IsValid(UC_Icon_Hero))
	{
		UUIFunctionLibrary::ClearHeroIconIfo(HeroIconInfo);

//		UUIFunctionLibrary::GetHeroIconInfoWithID(HeroIconInfo, InHeroID, EHeroOwnershipType::VE_Me);
		if (IsValid(UC_Icon_Hero))
		{
			UC_Icon_Hero->SetHeroIconInfoCopy(HeroIconInfo);
			UC_Icon_Hero->SetHeroIconInfoWithID(InHeroID, EHeroOwnershipType::VE_Me);
		}
	}

	if (IsValid(Text_AttackPow))
	{
		Text_AttackPow->SetText(FText::AsNumber(InCurrentPower));
	}

	if (IsValid(ChangeTextSwitcher))
	{
		ChangeTextSwitcher->SetActiveWidgetIndex(bIncrease ? 1 : 0);
	}
	if (IsValid(BG_WidgetSwitcher))
	{
		BG_WidgetSwitcher->SetActiveWidgetIndex(bIncrease ? 1 : 0);
	}

	nStart = InPrevPower;
	nEnd = InCurrentPower;
	nCurrent = InPrevPower;
	if(bIncrease)
	{
		if (IsValid(TextIncrease))
		{
			int32 nChangedValue = FMath::Abs<int32>(InCurrentPower - InPrevPower);
			TextIncrease->SetText(FText::AsNumber(nChangedValue));
		}
	}
	else
	{
		if (IsValid(TextDecrease))
		{
			int32 nChangedValue = FMath::Abs<int32>(InCurrentPower - InPrevPower);
			TextDecrease->SetText(FText::AsNumber(nChangedValue));
		}
	}

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlayAni("Splash_Ani", !bIncrease);
	bPlayStart = true;
	bTextAni = true;
}

void UUP_Popup_Splash_AttackPower::ChangeValueTick(float InDeltaTime)
{
	fCurrentTime += InDeltaTime;

	if (fCurrentTime >= 1.0f)
	{
		fCurrentTime = 1.0f;
	}

	nCurrent = FMath::Lerp(nStart, nEnd, fCurrentTime);
	
	if (CheckEnd())
	{
		bTextAni = false;
	}

	if (IsValid(Text_AttackPow))
	{
		Text_AttackPow->SetText(FText::AsNumber(nCurrent));
	}

}

bool UUP_Popup_Splash_AttackPower::CheckEnd()
{
	if (nStart < nEnd)
	{
		return nCurrent >= nEnd; // Increase case
	}
	else
	{
		return nCurrent <= nEnd; // decrease case 
	}
}

void UUP_Popup_Splash_AttackPower::ResetValue()
{
	bTextAni = false;
	nStart = 0;
	nEnd = 0;
	nCurrent = 0;
	fCurrentTime = 0.0f;
}
