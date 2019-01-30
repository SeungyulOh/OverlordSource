// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"
#include "UC_Notify.h"
#include "NotifyList/NotifyEventList.h"


void UUC_Notify::NativeConstruct()
{
	Super::NativeConstruct();
	this->SetVisibility(ESlateVisibility::Collapsed);
	//auto RGameInstance = RGAMEINSTANCE(this);
	//if ( IsValid(RGameInstance->NotifyManager))
	//{
	//	RGameInstance->NotifyManager->OnNotifyDelegate.RemoveDynamic(this, &UUC_Notify::SetVisibleByNotify);
	//	RGameInstance->NotifyManager->OnNotifyDelegate.AddDynamic(this, &UUC_Notify::SetVisibleByNotify);

	//	
	//	if (RGameInstance->NotifyManager->IsNotify(NotifyState))
	//	{
	//		this->SetVisibility(ESlateVisibility::HitTestInvisible);
	//	}
	//	else
	//	{
	//		this->SetVisibility(ESlateVisibility::Collapsed);
	//	}
	//	
	//	//SetNotify();
	//}
}

void UUC_Notify::NativeDestruct()
{
	//auto RGameInstance = RGAMEINSTANCE(this);
	//if ( IsValid(RGameInstance->NotifyManager))
	//{
	//	RGameInstance->NotifyManager->OnNotifyDelegate.RemoveDynamic(this, &UUC_Notify::SetVisibleByNotify);
	//}

	Super::NativeDestruct();
}

void UUC_Notify::CheckNotify()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->NotifyManager->IsNotify(NotifyState))
	{
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_Notify::SetVisibleByNotify(ENotifyStateFlagEnum InNotifyState, bool bState)
{
	//Current Notify Type
	if(InNotifyState == NotifyState)
	{
		if (bState)
		{
			this->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			this->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//Top_bar
	if (NotifyState == ENotifyStateFlagEnum::BM_Topbar)
	{
		auto RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->NotifyEventList))
		{
			if (RGameInstance->NotifyEventList->NotifyList.Num() != 0)
			{
				this->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			else
			{
				this->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUC_Notify::SetNotify()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->NotifyManager))
	{
		bool bIsState = false;
		switch (NotifyState)
		{
		case ENotifyStateFlagEnum::BM_Achievement:
			if (IsValid(RGameInstance->AchievementManager))
			{
				bIsState = RGameInstance->AchievementManager->FindNewNotify();
			}
			break;

		case ENotifyStateFlagEnum::BM_Topbar:
			bIsState = (RGameInstance->NotifyEventList->NotifyList.Num() != 0) ? true : false;
			break;

		case ENotifyStateFlagEnum::BM_Hero:
			bIsState = RGameInstance->NotifyManager->IsNotify(ENotifyStateFlagEnum::BM_Hero);
			break;

		default:
			break;
		}

		RGameInstance->NotifyManager->SetNotify(NotifyState, bIsState);
	}
}
