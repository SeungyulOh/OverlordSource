// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"

#include "GlobalIntegrated.h"
#include "ProjectRGameMode.h"

#include "UtilFunctionIntegrated.h"


#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"

#include "UI/Component/LocalizingTextBlock.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "RBattleTimer.h"

void URBattleTimer::NativeConstruct()
{
	Super::NativeConstruct();
	Text_TimeCount->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));

	bool bExe = false;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		if (!(UUtilFunctionLibrary::GetGamePlayMode()))
		{
			bExe = true;
		}
	}
	else
	{
		bExe = true;
	}

	if (bExe)
	{
		//bCanEverTick = false;
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URBattleTimer::NativeDestruct()
{
	Super::NativeDestruct();
}

void URBattleTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateTime();
}

bool URBattleTimer::IsValidData()
{
	if (Text_TimeCount == nullptr)
		return false;

	return true;
}

void URBattleTimer::UpdateTime()
{
	auto World = GetWorld();
	if (IsValid(World) == false)
		return;

	if (!(UUtilFunctionLibrary::GetGamePlayMode()))
		return;

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	auto TimeHandler_GameTime = BaseStageInfo->GetGameTimeHandler();
	if (TimeHandler_GameTime.IsValid() == false)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	float fTime = 0.0f;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->CurrentGameMode == EGamePlayModeTypeEnum::VE_Colosseum)
	{
		fTime = RGameInstance->RealTimeModManager->GetGameTime();
	}
	else
	{
		fTime = World->GetTimerManager().GetTimerRemaining(TimeHandler_GameTime);
	}
		

	
	int32 nMinute = fTime / 60;
	int32 nSecond = fTime - (nMinute * 60);
	
	FString Minute_Str	= FString::FromInt(nMinute * 0.1f);
	Minute_Str			+= FString::FromInt(nMinute % 10);

	FString Second_Str	= FString::FromInt(nSecond * 0.1f);
	Second_Str			+= FString::FromInt(nSecond % 10);

	Minute_Str			+= FString(":");
	Minute_Str			+= Second_Str;

// 	if (CheckStateValue <= 1)
// 	{
// 		CheckWarningTime(nMinute, nSecond);
// 	}
	//Update Widget
	if(Text_TimeCount)
	{
		Text_TimeCount->SetText(FText::FromString(Minute_Str));
	}
}

void URBattleTimer::CheckWarningTime(int32 InMinute, int32 InSecond)
{
	switch (CheckStateValue)
	{
		case 0:
		{
			Text_TimeCount->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));

			//Text_TimeCount->SetColorAndOpacity(DefaultColor);
			++CheckStateValue;
		}
		break;
		case 1:
		{
			if (InMinute <= 0)
			{
				if (InSecond <= 30)
				{
					++CheckStateValue;
					Text_TimeCount->SetColorAndOpacity(OverTimeColor);
				}
			}
		}
		break;
		default:
		break;
	}
}