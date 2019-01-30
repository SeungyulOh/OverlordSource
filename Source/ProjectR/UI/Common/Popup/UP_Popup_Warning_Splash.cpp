// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "UP_Popup_Warning_Splash.h"


void UP_Popup_Warning_Splash::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP_Popup_Warning_Splash::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bPlayStart)
	{
		if (!IsPlayAni(CurAniName))
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if ( IsValid(RGameInstance->RWidgetManager))
			{
				this->SetVisibility(ESlateVisibility::Collapsed);
			}

			bPlayStart = false;
		}
	}
}

void UP_Popup_Warning_Splash::RefreshUI(FText InDesc)
{
	TextBlock_Comments->SetText(InDesc);
	CurAniName = TEXT("Splash_Text");
	bPlayStart = true;
	this->SetVisibility(ESlateVisibility::Visible);
	PlayAni(CurAniName);
}
