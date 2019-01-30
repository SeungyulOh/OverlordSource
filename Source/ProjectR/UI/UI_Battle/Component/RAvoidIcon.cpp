// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"

#include "RAvoidIcon.h"

void URAvoidIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Click)
	{
#ifdef RULE_TWOTOUCHBUTTON
// 		Button_Click->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		this->SetVisibility(ESlateVisibility::HitTestInvisible);
#else
		Button_Click->OnClicked.RemoveDynamic(this, &URAvoidIcon::BtnEventOnClicked);
		Button_Click->OnClicked.AddDynamic(this, &URAvoidIcon::BtnEventOnClicked);
// 		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
#endif
	}

	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());

	if (IsValid(Text_CoolTime))
	{
		Text_CoolTime->SetVisibility(ESlateVisibility::Collapsed);
	}
	IsCheckTick = true;
}

void URAvoidIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsCheckTick == false)
		return;

	if (IsValid(BasePlayerController))
	{
		float fPercent = 0.0f;
		FName param_name = "Percent";
		if (BasePlayerController->GetIsAvoidState())
		{
			fPercent = BasePlayerController->GetAvoidTimePercent();

			if (IsValid(Text_CoolTime))
			{
				if (Text_CoolTime->GetVisibility() != ESlateVisibility::HitTestInvisible)
				{
					Text_CoolTime->SetVisibility(ESlateVisibility::HitTestInvisible);
				}

				int32 cool_seconds = (int32)BasePlayerController->GetAvoidRestTime() + 1;
				FText cool_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(cool_seconds));
				Text_CoolTime->SetText(cool_text);
			}
			if (Switcher_IconImage && Switcher_IconImage->GetActiveWidgetIndex() == 0)
			{
				Switcher_IconImage->SetActiveWidgetIndex(1);
			}
		}
		else
		{
			fPercent = 0.0f;

			if (IsValid(Text_CoolTime))
			{
				if (Text_CoolTime->GetVisibility() != ESlateVisibility::Collapsed)
				{
					Text_CoolTime->SetVisibility(ESlateVisibility::Collapsed);
				}
			}

			if (Switcher_IconImage && Switcher_IconImage->GetActiveWidgetIndex() == 1)
			{
				Switcher_IconImage->SetActiveWidgetIndex(0);
			}
		}

		if (IsValid(Image_CoolGauge))
		{
			UMaterialInstanceDynamic* gauge_material = Image_CoolGauge->GetDynamicMaterial();
			if (gauge_material)
			{
				gauge_material->SetScalarParameterValue(param_name, fPercent);
			}
		}
		if (fPercent == 0)
		{
			IsCheckTick = false;
		}
	}
}

void URAvoidIcon::BtnEventOnPressed()
{
	if (IsTouchEnable == false)
		return;

	PlayAni("Click_Pressed");
}

void URAvoidIcon::BtnEventOnClicked()
{
	if (IsTouchEnable == false)
		return;

	PlayAni("Click_Pressed", true);

	if (BasePlayerController)
	{
		if (BasePlayerController->GetIsAvoidState())
			return;

		BasePlayerController->SetAvoidState(true);
	}
	IsCheckTick = true;
}

void URAvoidIcon::SetClickedButton(bool InEnableClick)
{
	IsTouchEnable = InEnableClick;

#ifdef RULE_TWOTOUCHBUTTON
	return;
#endif

	if (InEnableClick)
	{
		Button_Click->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Button_Click->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}