// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"



#include "SharedConstants/GlobalConstants.h"

#include "UI/RWidgetManager.h"

#include "UtilFunctionIntegrated.h"

#include "Components/TextBlock.h"
#include "UC_Raid_Notify.h"


void UUC_Raid_Notify::NativeConstruct()
{	
	Super::NativeConstruct();

	Notify_Ref = GetWidgetAnimFromName(TEXT("Notification"));

	if (IsValid(Panel_Canvas))
	{
		Panel_Canvas->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnRaidNotify.Clear();
		RGameInstance->EventManager->OnRaidNotify.AddDynamic(this, &UUC_Raid_Notify::Notify);

// 		RGameInstance->EventManager->OnRealTimeModNotify.Clear();
// 		RGameInstance->EventManager->OnRealTimeModNotify.AddDynamic(this, &UUC_Raid_Notify::OnRealTimeModNotify);
	}
}

void UUC_Raid_Notify::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUC_Raid_Notify::Notify(FName key)
{
	if (IsValid(Panel_Canvas))
	{
		Panel_Canvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (IsValid(TextMsg))
	{
		TextMsg->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Skill, key));
	}

	if (IsValid(Notify_Ref))
	{
		PlayAnimation(Notify_Ref);
	}
}

void UUC_Raid_Notify::OnRealTimeModNotify(FText msg)
{
	if (IsValid(Panel_Canvas))
	{
		Panel_Canvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (IsValid(TextMsg))
	{
		TextMsg->SetText(msg);
	}

	if (IsValid(Notify_Ref))
	{
		PlayAnimation(Notify_Ref);
	}
}


