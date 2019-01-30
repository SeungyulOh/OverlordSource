// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UCW_SpeechBubble.h"

#include "SharedConstants/GlobalConstants.h"
//#include "GlobalIntegrated.h"
//

//#include "Utils/WidgetObjectPool.h"
#include "UtilFunctionIntegrated.h"

void UUCW_SpeechBubble::NativeConstruct()
{
	Super::NativeConstruct();
}

void  UUCW_SpeechBubble::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bShowUI)
	{
		ShowTime -= InDeltaTime;
		if (ShowTime <= 0.0f)
		{
			bShowUI = false;
			//OnShow(false);
			if (Overlay_All)
			{
				Overlay_All->SetVisibility(ESlateVisibility::Collapsed);
				//this->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUCW_SpeechBubble::Show(FName key, float showTime)
{
	bShowUI = true;
	ShowTime = showTime;
	//this->GetWorld()->
	if (IsValid(TextBlock))
	{
		TextBlock->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, key));
	}

	
	//OnShow(bShowUI);
	if (Overlay_All)
	{
		Overlay_All->SetVisibility(ESlateVisibility::HitTestInvisible);
		//this->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}