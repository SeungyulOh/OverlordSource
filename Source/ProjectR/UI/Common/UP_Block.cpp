// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Block.h"

#include "GlobalIntegrated.h"

#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"

void UUP_Block::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Border_Background))
	{
		Border_Background->SetVisibility(ESlateVisibility::Collapsed);
	}

	//not use
	/*if (Button_Click) 
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UUP_Block::OnClick_Block);
		Button_Click->OnClicked.AddDynamic(this, &UUP_Block::OnClick_Block);
	}*/
}

void UUP_Block::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(Border_Background))
	{
		if (Border_Background->GetVisibility() == ESlateVisibility::Visible)
		{
			if (bStartFadeIn)
			{
				CurTime += InDeltaTime;

				if (CurTime >= FadeInTime)
				{
					CurTime = FadeInTime;

					Border_Background->SetVisibility(ESlateVisibility::Collapsed);
					URGameInstance* RGameInstance = RGAMEINSTANCE(this);
					if ( IsValid(RGameInstance->RWidgetManager))
					{
						//RGameInstance->RWidgetManager->HideBlockUI();
					}

					bStartFadeIn = false;
				}

				float fAlpha = 1.0f - CurTime / FadeInTime;

				Border_Background->BrushColor.A = fAlpha;
				Border_Background->SetBrushColor(Border_Background->BrushColor);
			}
		}
	}
}

void UUP_Block::OnClick_Block()
{
// #if SHOW_TOOLTIP == 1
// 	if (CurBlockType == EUIBlockTypeEnum::VE_Tooltip)
// 	{
// 		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 		if (IsValid(RGameInstance)  && IsValid(RGameInstance->TooltipEventList))
// 		{
// 			if (RGameInstance->TooltipEventList->IsShowTooltip())
// 			{
// 				RGameInstance->TooltipEventList->SetHide();
// 			}
// 		}
// 	}
// #endif
}

void UUP_Block::ShowBackground()
{
	if (IsValid(Border_Background))
	{
		if (Border_Background->GetVisibility() == ESlateVisibility::Collapsed)
		{
			Border_Background->SetVisibility(ESlateVisibility::Visible);

			Border_Background->BrushColor.A = 1.0f;
			Border_Background->SetBrushColor(Border_Background->BrushColor);

			bStartFadeIn = false;
		}
	}
}

void UUP_Block::FadeIn(float InTime)
{
	bStartFadeIn = true;
	FadeInTime = InTime;
	CurTime = 0.0f;
}