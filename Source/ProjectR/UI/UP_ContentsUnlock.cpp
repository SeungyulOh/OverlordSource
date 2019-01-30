// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UI/UC_ContentsType.h"
#include "UP_ContentsUnlock.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"




void UUP_ContentsUnlock::NativeConstruct()
{
	Super::NativeConstruct();

	FindChildWidgets<UUC_ContentsType>(this, ContentsTypeArray);
}

void UUP_ContentsUnlock::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	/*
	if (bStartAni)
	{
		if (!IsPlayAni(TEXT("Popup_Appear")))
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if ( IsValid(RGameInstance->ContentsUnLockManager))
			{
				RGameInstance->ContentsUnLockManager->CloseContentsUnlockNotification();
				bStartAni = false;
			}
		}
	}
	//*/
}

void UUP_ContentsUnlock::InvalidateUI()
{
	if (ContentsTypeArray.Num() > 0)
	{
		for (auto widget : ContentsTypeArray)
		{
			if(IsValid(widget))
				widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
		
}

void UUP_ContentsUnlock::SetInfo(EGamePlayModeTypeEnum InContents, FText Content, FText Desc, int32 SlotIndex)
{
	if(!ContentsTypeArray.IsValidIndex(SlotIndex))
		return;

	if( IsValid( ContentsTypeArray[SlotIndex]) )
	{
		ContentsTypeArray[SlotIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ContentsTypeArray[SlotIndex]->LocTextBlock_Contents->SetText(Content);
		ContentsTypeArray[SlotIndex]->LocTextBlock_Desc->SetText(Desc);
		ContentsTypeArray[SlotIndex]->eContentsType = InContents;
	}
}

void UUP_ContentsUnlock::RefreshUI()
{
//	bStartAni = true;
	PlayAni(TEXT("Popup_Appear"));
	OnRefreshUI();
}

void UUP_ContentsUnlock::OnRefreshUI()
{
	USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/ui/UP_Campaign_Campaign_Stage_Appear_Cue.UP_Campaign_Campaign_Stage_Appear_Cue'")));
	USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), Sound);
	//UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}
