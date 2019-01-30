// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_EpisodeCard.h"


#include "GlobalIntegrated.h"

#include "UI/Component/LocalizingTextBlock.h"
#include "Public/components/TextBlock.h"
#include "Public/components/ProgressBar.h"
#include "Public/components/Image.h"
#include "Public/components/WidgetSwitcher.h"





#include "UtilFunctionIntegrated.h"

#include "SharedConstants/GlobalConstants.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/BattleBaseAnimInstance.h"

#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/UserInfo.h"



void UUC_EpisodeCard::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUC_EpisodeCard::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_EpisodeCard::SetEpisodeInfo(FName InEpisodeID)
{
	EpisodeID		= InEpisodeID;
	bOpened = true;

	SetEpisodeInfo_Impl();
}

void UUC_EpisodeCard::SetEpisodeInfo_Impl()
{
// 	FCampaignEpisodeTableInfo* EpisodeTableInfo = nullptr;
// 	switch (EpisodeType)
// 	{
// 		case EDungeonTypeEnum::VE_Normal:
// 			EpisodeTableInfo = UTableManager::GetInstancePtr()->GetCampaignEpisodeRow(EpisodeID);
// 			break;
// 		case EDungeonTypeEnum::VE_HeroCard:
// 			EpisodeTableInfo = UTableManager::GetInstancePtr()->GetCampaignEpisodeHeroRow(EpisodeID);
// 			break;		
// 	}
// 				
// 	if (EpisodeTableInfo)
// 	{
// 		EpisodeName = EpisodeTableInfo->CardName;
// 		EpisodeImage_Key = EpisodeTableInfo->EpisodeKey_Texture;
// 
// 		if (IsValid(EpisodeNameText))
// 		{
// 			EpisodeNameText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, EpisodeTableInfo->Title));
// 		}
// 
// 		if (IsValid(EpisodeNameText1))
// 		{
// 			EpisodeNameText1->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, EpisodeTableInfo->Title));
// 		}
// 
// 		if (IsValid(EpisodeIdxText))
// 		{
// 			EpisodeIdxText->SetText(FText::AsNumber(EpisodeTableInfo->EpisodeStep));
// 		}
// 
// 		if (IsValid(EpisodeUnLockText))
// 		{
// 			EpisodeUnLockText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, EpisodeTableInfo->UnlockText1));
// 		}
// 
// 		if (IsValid(EpisodeUnLockLevelText))
// 		{
// 			EpisodeUnLockLevelText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, EpisodeTableInfo->UnlockText2));
// 		}
// 
// 		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 		if ( IsValid(RGameInstance->UserInfo))
// 		{
// 			if (EpisodeTableInfo->UnlockRequire > 0)
// 			{
// 				if (EpisodeTableInfo->UnlockRequire > RGameInstance->UserInfo->Avatar.level)
// 				{
// 					bOpened = false;
// 				}
// 			}
// 		}
// 	}
}

bool UUC_EpisodeCard::IsEnable()
{
	return bOpened;
}

void UUC_EpisodeCard::OnClick_EpisodeCard()
{
	if (IsOpenedEpisode() == false)
		return;

	auto RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager) && IsOpenedEpisode())
	{
		//RGameInstance->EventManager->OnClickEpisodeCard.Broadcast(EpisodeID);
		OnClickEpisodeCard.Broadcast(EpisodeID);
	}
}

void UUC_EpisodeCard::InvalidateData()
{
	Refresh();
}

void UUC_EpisodeCard::Refresh()
{
	SetCardImage_Texture();		
		
	SetCardSelectedState_Impl();
}

void UUC_EpisodeCard::RefreshSelection()
{
	SetCardSelectedState_Impl();
}

void UUC_EpisodeCard::SetCardSelectedState_Impl()
{
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	
// 	{
// 		FName SelectedEpisodeID = UGamePlayBluePrintFunction::GetBaseStageInfo()->EpisodeID;
// 		if (Image_SelectedEpisode)
// 		{
// 			if (SelectedEpisodeID == EpisodeID)
// 			{
// 				Image_SelectedEpisode->SetVisibility(ESlateVisibility::Visible);
// 			}
// 			else
// 			{
// 				Image_SelectedEpisode->SetVisibility(ESlateVisibility::Collapsed);
// 			}
// 		}
// 	}
}

void UUC_EpisodeCard::SetLockImage()
{
	bOpened = false;
	if (Image_ContentsLock)
	{
		Image_ContentsLock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}	
}

void UUC_EpisodeCard::SetCardImage_Texture()
{	
	if (Image_Episode)
		UUIFunctionLibrary::SetImageBrush( UUIFunctionLibrary::GetIconImageFromTable(EpisodeImage_Key), Image_Episode);
}

bool UUC_EpisodeCard::IsOpenedEpisode()
{
	/// get from campaign mamager

	bool IsUnlock = IsContentsUnlock();
	bOpened = bOpened && IsUnlock ? true : false;

	return bOpened;
}

bool UUC_EpisodeCard::IsContentsUnlock()
{
	return true;
}