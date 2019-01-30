// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_SkillSetting.h"
//#include "HUDBase.h"

#include "GlobalIntegrated.h"
#include "GameInfo/PlayStateInfo.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"

#include "UI/RWidgetManager.h"
#include "UI/UI_Battle/UC_SkillHeroIcon.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/Component/RAutoSkillIcon.h"

#include "UtilFunctionIntegrated.h"




void UUC_SkillSetting::NativeConstruct()
{
	Super::NativeConstruct();
	
	InvalidateData();
}

void UUC_SkillSetting::InvalidateData()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->PlayStateInfo))
	{
		for (UCanvasPanel* panel : HeroCanvasPanelList)
		{
			if (IsValid(panel))
			{
				panel->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		const TArray<FHeroSlotInfo>& ArrayList = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
		for (int32 i = 0; i < ArrayList.Num(); ++i)
		{
			const FHeroSlotInfo& info = ArrayList[i];

			if (HeroCanvasPanelList.IsValidIndex(i))
			{
				if(IsValid(HeroCanvasPanelList[i]))
					HeroCanvasPanelList[i]->SetVisibility(ESlateVisibility::Visible);
			}
		}

		URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
		if (IsValid(RGameUserSetting))
		{
			/*OnOffBtnClick(0, RGameUserSetting->DeckHero1);
			OnOffBtnClick(1, RGameUserSetting->DeckHero2);
			OnOffBtnClick(2, RGameUserSetting->DeckHero3);
			OnOffBtnClick(3, RGameUserSetting->DeckHero4);
			OnOffBtnClick(4, RGameUserSetting->DeckHero5);*/
		}
	}
}

void UUC_SkillSetting::OkBtnClick()
{
	int32 OnCount = 0;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if ( IsValid(RGameInstance->PlayStateInfo) && IsValid(RGameUserSetting))
	{
		RGameUserSetting->ApplySettings(true);

		const TArray<FHeroSlotInfo>& ArrayList = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
		/*if (ArrayList.IsValidIndex(0))
		{
			if (RGameUserSetting->DeckHero1)
				++OnCount;
		}
		if (ArrayList.IsValidIndex(1))
		{
			if (RGameUserSetting->DeckHero2)
				++OnCount;
		}
		if (ArrayList.IsValidIndex(2))
		{
			if (RGameUserSetting->DeckHero3)
				++OnCount;
		}
		if (ArrayList.IsValidIndex(3))
		{
			if (RGameUserSetting->DeckHero4)
				++OnCount;
		}
		if (ArrayList.IsValidIndex(4))
		{
			if (RGameUserSetting->DeckHero5)
				++OnCount;
		}*/
	}

	if (OnCount == 0)
	{
		if (IsValid(ParentWidget))
		{

			URBaseIngameHUD* OneGameHUD = Cast<URBaseIngameHUD>(ParentWidget);
			if (IsValid(OneGameHUD) && IsValid(OneGameHUD->AutoSkillIcon))
			{
				OneGameHUD->AutoSkillIcon->NextAutoSkillType(true);
			}
		}
	}

	if (UGameplayStatics::IsGamePaused(this))
	{
		UGamePlayBluePrintFunction::SetPause(false);
	}
	
	if ( IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_InGameSkillSetting);
	}
}

void UUC_SkillSetting::AllSelectBtnClick()
{
	for (int32 i = 0; i < 5; ++i)
	{
		OnOffBtnClick(i, true);
	}

	InvalidateData();
}

void UUC_SkillSetting::AllDeSelectBtnClick()
{
	for (int32 i = 0; i < 5; ++i)
	{
		OnOffBtnClick(i, false);
	}

	InvalidateData();
}

void UUC_SkillSetting::OnOffBtnClick(int32 idx, bool bOn)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->PlayStateInfo))
	{
		const TArray<FHeroSlotInfo>& ArrayList = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
		if (ArrayList.IsValidIndex(idx))
		{
			const FHeroSlotInfo& info = ArrayList[idx];
			USkillEntityComponent* SkillEntityComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(info.SpawnedTargetActor);
			

			if (HeroBtnSwitcherList.IsValidIndex(idx))
			{
				if (IsValid(HeroBtnSwitcherList[idx]))
				{
					HeroBtnSwitcherList[idx]->SetActiveWidgetIndex(bOn == true ? 0 : 1);
				}
			}

			if (HeroIconList.IsValidIndex(idx))
			{
				if (IsValid(HeroIconList[idx]))
				{
					HeroIconList[idx]->SetHero(info);
				}
			}

// 			URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
// 			if (IsValid(RGameUserSetting))
// 			{
// 				switch (idx)
// 				{
// 				case 0:
// 					RGameUserSetting->DeckHero1 = bOn;
// 					break;
// 				case 1:
// 					RGameUserSetting->DeckHero2 = bOn;
// 					break;
// 				case 2:
// 					RGameUserSetting->DeckHero3 = bOn;
// 					break;
// 				case 3:
// 					RGameUserSetting->DeckHero4 = bOn;
// 					break;
// 				case 4:
// 					RGameUserSetting->DeckHero5 = bOn;
// 					break;
// 				}
// 			}
		}
	}
}

void UUC_SkillSetting::SkillCastingOnOff(bool bOn)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
}