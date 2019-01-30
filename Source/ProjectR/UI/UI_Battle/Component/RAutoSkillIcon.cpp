// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAutoSkillIcon.h"

#include "CustomStructures/HeroSlotInfo.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"

#include "UtilFunctionIntegrated.h"
#include "GameInfo/PlayStateInfo.h"
#include "GlobalIntegrated.h"


#include "ContentGameMode/Modes/TestGameMode.h"
#include "ProjectRGameMode.h"
#include "LeaderControlHelper.h"
#include "TutorialGameMode.h"


void URAutoSkillIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if(URealTimeModManager::IsRealTimeGameMode())
		return;
	
	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &URAutoSkillIcon::BtnEventOnClicked);
		Button_Click->OnClicked.AddDynamic(this, &URAutoSkillIcon::BtnEventOnClicked);
	}
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);


	if (IsValid(Button_tutorial))
	{
		Button_tutorial->SetVisibility(ESlateVisibility::Collapsed);
	}

	BasePlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld()));

	EGamePlayModeTypeEnum GamePlayModeType = UUtilFunctionLibrary::GetGamePlayModeEnum();
// 	if (EGamePlayModeTypeEnum::VE_RaidKillBoss == GamePlayModeType ||
// 		EGamePlayModeTypeEnum::VE_BattleGround == GamePlayModeType ||
// 		EGamePlayModeTypeEnum::VE_GuildRaid == GamePlayModeType)
	if (EGamePlayModeTypeEnum::VE_RaidKillBoss == GamePlayModeType)
	{
		if (IsValid(Image_BG))
		{
			Image_BG->SetVisibility(ESlateVisibility::Hidden);
		}

		if (IsValid(Switcher_IconImage))
		{
			Switcher_IconImage->SetVisibility(ESlateVisibility::Hidden);
		}

		if (IsValid(Button_Click))
		{
			Button_Click->SetVisibility(ESlateVisibility::Hidden);
		}

		if (IsValid(BasePlayerController))
		{

			AActor* SelectCharacter = BasePlayerController->GetSelectedCharacter();
			const TArray<FHeroSlotInfo>& HeroArray = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
			for (const FHeroSlotInfo& Element : HeroArray)
			{
				if (SelectCharacter == Element.SpawnedTargetActor)
				{
					BasePlayerController->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);
				}

				auto SkillEntityComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Element.SpawnedTargetActor);
				if (SkillEntityComp)
				{
					SkillEntityComp->ChangeAISkillMode(EAIControlModeEnum::VE_ManualMode);
				}
			}
		}
	}
	else
	{
		
		{
			RGameInstance->EventManager->OnRefreshAutoSkillIcon.RemoveDynamic(this, &URAutoSkillIcon::RefreshAutoSkillIcon);
			RGameInstance->EventManager->OnRefreshAutoSkillIcon.AddDynamic(this, &URAutoSkillIcon::RefreshAutoSkillIcon);
		}


		if (bUnlockAutoSkillStage)
		{
			URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
			if (EAIControlModeEnum::VE_SemiAutoMode == RGameUserSetting->AutoSkillMode)
			{
				RGameUserSetting->AutoSkillMode = EAIControlModeEnum::VE_SemiAutoMode;
			}
			ActiveSwitchIndex = int32(RGameUserSetting->AutoSkillMode);
		}
		else
		{
			ActiveSwitchIndex = int32(EAIControlModeEnum::VE_ManualMode);
		}
		RefreshAutoSkillIcon();
	}

	
}

void URAutoSkillIcon::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	{
		RGameInstance->EventManager->OnRefreshAutoSkillIcon.RemoveDynamic(this, &URAutoSkillIcon::RefreshAutoSkillIcon);
	}

	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &URAutoSkillIcon::BtnEventOnClicked);
	}


	Super::NativeDestruct();
}

void URAutoSkillIcon::BtnEventOnClicked()
{
#if WITH_EDITOR
	
	{
		ATestGameMode* pGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (pGameMode)
		{
			NextAutoSkillType(false);
			return;
		}
	}
#endif

	if (bUnlockAutoSkillStage)
	{
		NextAutoSkillType(true);
	}
	else
	{
		FText LocalizedText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Battle_Controller_04"));
		FFormatArgumentValue FormatArg1 = FText::FromString(TEXT("1-6"));
		LocalizedText = FText::Format(LocalizedText, FormatArg1);
		UUtilFunctionLibrary::ShowIndicator(LocalizedText, 2.0f, true, true);
	}
}

void URAutoSkillIcon::NextAutoSkillType(bool InIsSave)
{
// 	if (!IsValid(RGameInstance))
// 		return;
	if (!IsValid(BasePlayerController))
		return;

	if (Switcher_IconImage)
	{
		++ActiveSwitchIndex;
		if (2 < ActiveSwitchIndex)
		{
			ActiveSwitchIndex = 0;
		}
	}

	if (InIsSave)
	{
		URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
		if (IsValid(RGameUserSetting))
		{
			RGameUserSetting->AutoSkillMode = (EAIControlModeEnum)ActiveSwitchIndex;
			RGameUserSetting->ApplySettings(true);
		}
	}

	RefreshAutoSkillIcon();
}

void URAutoSkillIcon::RefreshAutoSkillIcon()
{
// 	if (!IsValid(RGameInstance))
// 		return;
	if (!IsValid(BasePlayerController))
		return;
	if (0 > ActiveSwitchIndex || 2 < ActiveSwitchIndex)
		return;

	if (IsValid(Image_BG))
	{
		if (false == Image_BG->IsVisible())
		{
			Image_BG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	if (IsValid(Switcher_IconImage))
	{
		if (false == Switcher_IconImage->IsVisible())
		{
			Switcher_IconImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		Switcher_IconImage->SetActiveWidgetIndex(ActiveSwitchIndex);
	}

	if (IsValid(Button_Click))
	{
		if (false == Button_Click->IsVisible())
		{
			Button_Click->SetVisibility(ESlateVisibility::Visible);
		}
	}

	bool bAutoMode = (ActiveSwitchIndex != 0);
	ATestGameMode* TestGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)->GetWorld()));
	ATutorialGameMode* TutorialGameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)->GetWorld()));
	if(!TestGameMode && !TutorialGameMode)
		BasePlayerController->AIControlMode_UserInput = (EAIControlModeEnum)ActiveSwitchIndex;

	BasePlayerController->SetLeaderCharacterAIAutoMode(BasePlayerController->AIControlMode_UserInput);

}