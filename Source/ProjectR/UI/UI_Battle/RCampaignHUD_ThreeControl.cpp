// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RCampaignHUD_ThreeControl.h"

#include "ProjectRGameMode.h"
#include "GameInfo/PlayStateInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/UserInfo.h"
#include "GlobalIntegrated.h"


#include "ContentGameMode/Modes/StageGameMode.h"
#include "ContentGameMode/Modes/ArenaGameMode.h"
#include "ContentGameMode/Modes/MirrorGameMode.h"

//#include "Global/BattleFieldManager.h"


#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
//#include "Character/Party/Party.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Battle/RSkillIcon.h"
#include "UI/UI_Battle/RBossBar.h"
#include "UI/UI_Battle/RMultiBossBar.h"
#include "UI/UI_Battle/RBattleTimer.h"
#include "UI/UI_Battle/Component/RCameraIcon.h"
#include "UI/UI_Battle/RBattleOptionMenu.h"
#include "UI/UI_Battle/Component/RAutoSkillIcon.h"
#include "UI/UI_Battle/Component/RMeetingIcon.h"
#include "UI/UI_Battle/Component/RHudStar.h"
#include "UI/UI_Battle/UC_SkillSetting.h"
#include "UI/UI_Battle/UC_SkillIndicator.h"
#include "UI/UI_Battle/UC_CombineBenefitNoti.h"

#include "UI/UI_Battle/InGameHeroUI/HeroUIforBase.h"
#include "UI/UI_Battle/InGameHeroUI/HeroUIforLeader.h"
#include "UI/UI_Battle/InGameHeroUI/HeroUIforCrew.h"

#include "UI/Chat/UC_Chat_Ingame.h"
#include "UI/InGameHUD/PVP/UC_HUD_PVP_TotalHP.h"
#include "UI/InGameHUD/UC_HUD_PhaseProgress.h"
#include "UI/InGameHUD/UC_HUD_TowerHP.h"
#include "UI/InGameHUD/UC_HUD_Goal.h"
#include "UI/UI_Battle/RHeroIcon_ForInGame.h"
#include "UI/GuildRaid/UC_GuildRaid_Group.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Skill_V2/NetSkillManager.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "Network/HttpRequestClient.h"
#include "Network/RTSManager.h"
#include "UI/InGameHUD/SkillIcon_New.h"
#include "Runtime/UMG/Public/Components/SafeZone.h"
#include "Engine/UserInterfaceSettings.h"

#define Hero_Icons_Y_General	19.0f
#define Hero_Icons_Y_Colosseum	100.0f

void URCampaignHUD_ThreeControl::NativeConstruct()
{

	if (!HeroIconArray.Num())
	{
		HeroIconArray.Empty(5);
		HeroIconArray.Emplace(LeaderIconUI);
		HeroIconArray.Emplace(Crew1BattleIconUI);
		HeroIconArray.Emplace(Crew1RestIconUI);
		HeroIconArray.Emplace(Crew2BattleIconUI);
		HeroIconArray.Emplace(Crew2RestIconUI);
	}
	
	

	//tuto
// 	Button_ForTuto_1->OnClicked.RemoveDynamic(this, &URCampaignHUD_ThreeControl::OnClickFirstTutoSkill);
// 	Button_ForTuto_1->OnClicked.AddDynamic(this, &URCampaignHUD_ThreeControl::OnClickFirstTutoSkill);


	ShowCrewFaceUI(true);
	HideUnusedCharacterUI();
	Super::NativeConstruct();

}

void URCampaignHUD_ThreeControl::NativeDestruct()
{
	Super::NativeDestruct();
}

void URCampaignHUD_ThreeControl::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}





void URCampaignHUD_ThreeControl::UpdateHeroIconSlot()
{
	for (URHeroUIforBase* slot : HeroIconArray)
	{
		slot->UpdateIcon();
	}
}




//////////////////////////////////





void URCampaignHUD_ThreeControl::HideUnusedCharacterUI()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray<FHeroSlotInfo> HeroSlotInfoArray = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();

	for (URHeroUIforBase* faceicon : HeroIconArray)
	{
		faceicon->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (size_t i = 0; i < HeroSlotInfoArray.Num(); ++i)
	{
		int32 nPosition = GetSlotPosition(HeroSlotInfoArray[i]);

		if(HeroIconArray.IsValidIndex(nPosition))
		{
			if (!HeroSlotInfoArray[i].HeroUD.IsEmpty())
			{
				HeroIconArray[nPosition]->SetHeroSlot(HeroSlotInfoArray[i]);
				HeroIconArray[nPosition]->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			else
				continue;

			if(nPosition == (int32)EPartyRole::Leader)
			{
				URHeroUIforLeader* leadericon = Cast<URHeroUIforLeader>(HeroIconArray[nPosition]);
				if(IsValid(leadericon))
				{
					leadericon->SetUIForCampaign(false);
				}
			}
			else
			{
				URHeroUIforCrew* crewicon = Cast<URHeroUIforCrew>(HeroIconArray[nPosition]);
				if(IsValid(crewicon))
				{
// 					crewicon->
				}
			}
		}
	}


	bool bCrew1Battle = false;
	bool bCrew1Rest = false;
	bool bCrew2Battle = false;
	bool bCrew2Rest = false;

	for (FHeroSlotInfo& slot : HeroSlotInfoArray)
	{
		if (slot.CharacterRole == ECharacterRoleEnum::VE_Crew1)
		{
			if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
				bCrew1Battle = true;
			else if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Rest)
				bCrew1Rest = true;
		}
		else if (slot.CharacterRole == ECharacterRoleEnum::VE_Crew2)
		{
			if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
				bCrew2Battle = true;
			else if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Rest)
				bCrew2Rest = true;
		}
	}

	bool bCanSwapCrew1 = false;
	bool bCanSwapCrew2 = false;

	if (bCrew1Battle && bCrew1Rest)
		bCanSwapCrew1 = true;

	if (bCrew2Battle && bCrew2Rest)
		bCanSwapCrew2 = true;

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo->bPlayAlone)
	{
		ChangingImage1->SetVisibility(ESlateVisibility::Collapsed);
		ChangingImage2->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		ChangingImage1->SetVisibility(bCanSwapCrew1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		ChangingImage2->SetVisibility(bCanSwapCrew2 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	//SkillSet->HideUnusedCharacterUI(bCanSwapCrew1, bCanSwapCrew2);
}



void URCampaignHUD_ThreeControl::ChangePortrait1()
{
	if (Crew1BattleIconUI && Crew1RestIconUI)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if (!RGameInstance->PlayStateInfo)
			return;
		TArray<FHeroSlotInfo> SlotInfoArray = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
		for (size_t i = 0; i < SlotInfoArray.Num(); ++i)
		{
			if (SlotInfoArray[i].CharacterRole == ECharacterRoleEnum::VE_Crew1 && SlotInfoArray[i].CharacterCrewState == ECharacterCrewStateEnum::VE_Battle && SlotInfoArray[i].SpawnedTargetActor)
			{
				Crew1BattleIconUI->SetHeroSlot(SlotInfoArray[i]);
				UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SlotInfoArray[i].SpawnedTargetActor);
				if (BattleEntityComp)
				{
					BattleEntityComp->SetHP(BattleEntityComp->GetMaxHP() * SlotInfoArray[i].PrevHeroHP);
				}
				
			}
			else if (SlotInfoArray[i].CharacterRole == ECharacterRoleEnum::VE_Crew1 && SlotInfoArray[i].CharacterCrewState == ECharacterCrewStateEnum::VE_Rest)
			{
				Crew1RestIconUI->SetHeroSlot(SlotInfoArray[i]);
				Crew1RestIconUI->UpdateHp(SlotInfoArray[i].PrevHeroHP);
			}
		}		
	}
}

void URCampaignHUD_ThreeControl::ChangePortrait2()
{
	if (Crew2BattleIconUI && Crew2RestIconUI)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if (!RGameInstance->PlayStateInfo)
			return;
		TArray<FHeroSlotInfo> SlotInfoArray = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
		for (size_t i = 0; i < SlotInfoArray.Num(); ++i)
		{
			if (SlotInfoArray[i].CharacterRole == ECharacterRoleEnum::VE_Crew2 && SlotInfoArray[i].CharacterCrewState == ECharacterCrewStateEnum::VE_Battle && SlotInfoArray[i].SpawnedTargetActor)
			{
				Crew2BattleIconUI->SetHeroSlot(SlotInfoArray[i]);
				UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SlotInfoArray[i].SpawnedTargetActor);
				if (BattleEntityComp)
				{
					BattleEntityComp->SetHP(BattleEntityComp->GetMaxHP() * SlotInfoArray[i].PrevHeroHP);
				}
			}
			else if (SlotInfoArray[i].CharacterRole == ECharacterRoleEnum::VE_Crew2 && SlotInfoArray[i].CharacterCrewState == ECharacterCrewStateEnum::VE_Rest)
			{
				Crew2RestIconUI->SetHeroSlot(SlotInfoArray[i]);
				Crew2RestIconUI->UpdateHp(SlotInfoArray[i].PrevHeroHP);
			}
				
		}
	}
}


void URCampaignHUD_ThreeControl::OnClickFirstTutoSkill()
{
	USkillIcon_New* leaderActive = SkillSet->Variables.SkillIconArray[(int32)ESkillIconSeq::LEADER_ACTIVE1];
	const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	float viewScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));
	if (SkillSet)
	{
		float skillSetLTX;
		float skillSetLTY;

		UCanvasPanelSlot* skillSetSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SkillSet);
		skillSetLTX = viewportSize.X / 2 + skillSetSlot->GetPosition().X * viewScale;
		skillSetLTY = viewportSize.Y / 2 + skillSetSlot->GetPosition().Y * viewScale;
		if (leaderActive)
		{
			float leaderActiveLTX;
			float leaderActiveLTY;
			UCanvasPanelSlot* leaderActiveSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(leaderActive);

			UE_LOG(LogClass, Log, TEXT("%f"), leaderActiveSlot->GetSize().X);
			leaderActiveLTX = skillSetLTX + leaderActiveSlot->GetPosition().X * viewScale + leaderActiveSlot->GetSize().X * viewScale / 2;
			leaderActiveLTY = skillSetLTY + leaderActiveSlot->GetPosition().Y * viewScale + leaderActiveSlot->GetSize().Y * viewScale / 2;

			skillPos = FVector(leaderActiveLTX, leaderActiveLTY, 0);
			UGameplayStatics::SetGlobalTimeDilation(RGAMEINSTANCE(this), 1.0f);
			RGAMEINSTANCE(this)->InputManager->OnInputTouchBeginDelegate.Broadcast(ETouchIndex::Touch1, skillPos);
			FTimerHandle handler;
			RGAMEINSTANCE(this)->TimerManager->SetTimer(handler, this, &URCampaignHUD_ThreeControl::OnReleaseFirstTutoSkill, 0.1f);
		}
	}
}

void URCampaignHUD_ThreeControl::OnReleaseFirstTutoSkill()
{
	RGAMEINSTANCE(this)->InputManager->OnInputTouchEndDelegate.Broadcast(ETouchIndex::Touch1, skillPos);
	UGameplayStatics::SetGlobalTimeDilation(RGAMEINSTANCE(this), 1.0f);
}

void URCampaignHUD_ThreeControl::ShowCrewFaceUI(bool bShow)
{
	if (IsValid(Crew1))
		Crew1->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (IsValid(Crew2))
		Crew2->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (IsValid(ChangingImage1))
		ChangingImage1->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (IsValid(ChangingImage2))
		ChangingImage2->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}



