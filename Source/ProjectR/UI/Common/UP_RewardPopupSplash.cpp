// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_RewardPopupSplash.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "UI/Common/UC_Item_Icons.h"
#include "UI/RWidgetManager.h"


void UUP_RewardPopupSplash::NativeConstruct()
{
	Super::NativeConstruct();

	SplashAnim = GetWidgetAnimFromName("Splash_Anim");

	
}

void UUP_RewardPopupSplash::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bPlayStart)
	{
		if (!IsAnimationPlaying(SplashAnim))
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			
			{
				if (IsValid(RGameInstance->EventManager))
				{
// 					if (SplashType == ERewardSplashType::VE_GuildRaid)
// 					{
// 						RGameInstance->EventManager->OnModRoomGuildRaidDiceEnd.Broadcast();
// 					}
// 					
//					RGameInstance->EventManager->OnRewardPopupSplashEnd.Broadcast();
				}
				
			}

			bPlayStart = false;
		}
	}
}

void UUP_RewardPopupSplash::RefreshUI(ERewardSplashType InType, FText& InOwnerName, FText& InItemName, TArray<FREWARD>& InRewardData)
{
	SplashType = InType;

	if (IsValid(OwnerText))
	{
		OwnerText->SetText(InOwnerName);
	}
	if (IsValid(ItemText))
	{
		ItemText->SetText(InItemName);
	}
	if (IsValid(RewardIcon))
	{
		for (FREWARD& RewardData : InRewardData)
		{
			FITEM_ICON_INFO RewardItemIconInfo;
			UUIFunctionLibrary::GetRewardIconByRewardType(RewardData, RewardItemIconInfo);
			RewardIcon->SetItemIconStructure(RewardItemIconInfo);
		}
	}

	PlayAnimation(SplashAnim);

	bPlayStart = true;
}
