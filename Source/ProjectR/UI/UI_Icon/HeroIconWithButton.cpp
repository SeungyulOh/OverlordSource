// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "HeroIconWithButton.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"

void UHeroIconWithButton::NativeConstruct()
{
	OnClickHeroIcon.Clear();

	Super::NativeConstruct();

	Init();
}

void UHeroIconWithButton::NativeDestruct()
{
	OnClickHeroIcon.Clear();

	Super::NativeDestruct();
}

void UHeroIconWithButton::Init()
{
	if (nullptr == UC_HeroIcon)
	{
		if (IsValid(WidgetTree))
		{
			UWidget* ChildWidget = WidgetTree->FindWidget("UC_HeroIcon");
			if (IsValid(ChildWidget))
			{
				UC_HeroIcon = Cast<URenewal_BaseHeroIcon>(ChildWidget);
				UC_HeroIcon->SetIsEnabled(true);
			}
		}
	}

	if (IsValid(Button_HeroIcon))
	{
		Button_HeroIcon->OnClicked.RemoveDynamic(this, &UHeroIconWithButton::OnClick_HeroIcon);
		Button_HeroIcon->OnClicked.AddDynamic(this, &UHeroIconWithButton::OnClick_HeroIcon);
	}
}

void UHeroIconWithButton::OnClick_HeroIcon()
{
	OnClickHeroIcon.Broadcast();
	OnClickHeroIconOneParam.Broadcast(this);

	if (UC_HeroIcon)
	{
		auto bIsSelectedHeroIcon = UC_HeroIcon->GetIsSelected();
		UC_HeroIcon->SetIsSelected(!bIsSelectedHeroIcon);
	}
}

// void UHeroIconWithButton::OnSetHeroIconInfo(FHERO_ICON_INFO InHeroInfo)
// {
// 	if (UC_HeroIcon == nullptr)
// 		return;
// 
// 	UC_HeroIcon->SetHeroIconInfo(InHeroInfo);
// }

bool UHeroIconWithButton::IsValidHeroData()
{
	if (UC_HeroIcon == nullptr)
		return false;

	return true;
}

void UHeroIconWithButton::BP_HeroIconHideCombat()
{
	UC_HeroIcon->HidePowerPanel(true);
}

bool UHeroIconWithButton::GetHeroIconInfo(FHERO_ICON_INFO& HeroIconInfo)
{
	if (UC_HeroIcon == nullptr)
		return false;

	UC_HeroIcon->GetHeroIconInfo(HeroIconInfo);
	return true;
}