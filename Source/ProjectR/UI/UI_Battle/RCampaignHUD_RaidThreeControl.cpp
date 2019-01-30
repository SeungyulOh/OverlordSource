// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RCampaignHUD_RaidThreeControl.h"
#include "ProjectRGameMode.h"
#include "GameInfo/PlayStateInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/UserInfo.h"
#include "GlobalIntegrated.h"
#include "ContentIntergrated.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Party/Party.h"

#include "UI/UIDefs.h"
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

#include "Runtime/UMG/Public/Components/SafeZone.h"

#define Hero_Icons_Y_General	19.0f
#define Hero_Icons_Y_Colosseum	100.0f

#define CountDownSecond 5.0f

void UHUD_RaidThreeControl::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Text_CountDown))
	{
		Text_CountDown->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHUD_RaidThreeControl::NativeDestruct()
{
	Super::NativeDestruct();
}

void UHUD_RaidThreeControl::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	RenderTime();
}

void UHUD_RaidThreeControl::UpdateHeroIconSlot()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	
	for (URHeroUIforBase* slot : HeroIconArray)
	{
		slot->RTSUpdateIcon();
	}
}

void UHUD_RaidThreeControl::RenderTime()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FString MinText, SecText;
	float fRemainGameTime = RGameInstance->RealTimeModManager->GetGameTime();
	UDescriptionFunctionLibrary::GetTimeStr(fRemainGameTime, MinText, SecText);
	if (IsValid(Text_Min))
		Text_Min->SetText(FText::FromString(MinText));
	if (IsValid(Text_Sec))
		Text_Sec->SetText(FText::FromString(SecText));

	if (IsValid(Text_CountDown))
	{
		if (fRemainGameTime <= CountDownSecond)
		{
			int32 nRemainTime = (int32)fRemainGameTime;
			if (nPrevRemainTime != nRemainTime)
			{
				nPrevRemainTime = nRemainTime;

				Text_CountDown->SetVisibility(ESlateVisibility::HitTestInvisible);
				Text_CountDown->SetText(FText::AsNumber(nRemainTime));
				
				this->PlayAni("CountEffect");
			}
		}
	}
}

void UHUD_RaidThreeControl::ColosseumInit(int32& InMyTeamTotalHP, int32& InOtherTeamTotalHP)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FString OtherNick = TEXT("");
	for (TPair<int16, FRTS_PLAYER> Player : RGameInstance->RealTimeModManager->RTSPlayerList)
	{
		if(RGameInstance->HttpClient->GetKID() != Player.Value.kId)
			OtherNick = Player.Value.nick;
	}

	if (UC_TotalHP)// my deck
	{
		FAVATAR& Avatar = RGameInstance->UserInfo->GetAvatarData();
		FString guildnamedummy = TEXT("");
		UC_TotalHP->InitAvatarInfo(Avatar.nick, guildnamedummy, true);
		UC_TotalHP->InitHP(0, 0, false);
	}

	if (UC_TotalHP)// other deck
	{
		FString guildnamedummy = TEXT("");
		UC_TotalHP->InitAvatarInfo(OtherNick, guildnamedummy, false);
		UC_TotalHP->InitHP(0, 0, true);
	}

	// colosseum 모드는 UC_TotalHP의 타이머를 사용한다.
	TimerPanel->SetVisibility(ESlateVisibility::Collapsed);

	bool bMyTeamLeft = UObjectFunctionLibrary::GetMyTeamKey() == 1;
	UC_TotalHP->BP_Init(!bMyTeamLeft);
}

void UHUD_RaidThreeControl::ColosseumPointUpdate(int32 InMyPoint, int32 InOtherPoint)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FString OtherNick = TEXT("");
	for (TPair<int16, FRTS_PLAYER> Player : RGameInstance->RealTimeModManager->RTSPlayerList)
	{
		if (RGameInstance->HttpClient->GetKID() != Player.Value.kId)
			OtherNick = Player.Value.nick;
	}

	bool bMyTeamLeft = UObjectFunctionLibrary::GetMyTeamKey() == 1;

	
	if (UC_TotalHP)// my deck
	{
		FAVATAR& Avatar = RGameInstance->UserInfo->GetAvatarData();
		FString guildnamedummy = TEXT("");
		UC_TotalHP->InitAvatarInfo(Avatar.nick, guildnamedummy, bMyTeamLeft);
		UC_TotalHP->PointUpdate(InMyPoint, bMyTeamLeft);
	}

	if (UC_TotalHP)// other deck
	{
		FString guildnamedummy = TEXT("");
		UC_TotalHP->InitAvatarInfo(OtherNick, guildnamedummy, !bMyTeamLeft);
		UC_TotalHP->PointUpdate(InOtherPoint, !bMyTeamLeft);
	}

	// colosseum 모드는 UC_TotalHP의 타이머를 사용한다.
	TimerPanel->SetVisibility(ESlateVisibility::Collapsed);

	
	UC_TotalHP->BP_Init(!bMyTeamLeft);
}

void UHUD_RaidThreeControl::HideUnusedCharacterUI()
{
	TArray<FHeroSlotInfo> HeroSlotInfoArray = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray();

	for (URHeroUIforBase* faceicon : HeroIconArray)
	{
		faceicon->SetVisibility(ESlateVisibility::Collapsed);
	}
	for (size_t i = 0; i < HeroSlotInfoArray.Num(); ++i)
	{
		int32 nPosition = GetSlotPosition(HeroSlotInfoArray[i]);

		if(HeroIconArray.IsValidIndex(nPosition))
		{
			HeroIconArray[nPosition]->SetRTSHeroSlot(HeroSlotInfoArray[i]);
			//HeroIconArray[nPosition]->RTSUpdateIcon();
			HeroIconArray[nPosition]->SetVisibility(ESlateVisibility::HitTestInvisible);

			if (nPosition == (int32)EPartyRole::Leader)
			{
				URHeroUIforLeader* leadericon = nullptr;
				leadericon = Cast<URHeroUIforLeader>(HeroIconArray[nPosition]);
				if (IsValid(leadericon))
				{
					leadericon->SetUIForSingleRaid();
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

void UHUD_RaidThreeControl::UpdateMyTotalHP(int32 curr_hp, int32 max_hp)
{
	if (UC_TotalHP)
	{
		UC_TotalHP->ChangeHP(curr_hp, max_hp, false);
	}

// 	for (auto hero_icon : MyIconHeroArenaArray)
// 	{
// 		hero_icon->UpdateHeroHP();
// 	}
}

void UHUD_RaidThreeControl::UpdateEnemyTotalHP(int32 curr_hp, int32 max_hp)
{
	if (UC_TotalHP)
	{
		UC_TotalHP->ChangeHP(curr_hp, max_hp, true);
	}

// 	for (auto hero_icon : EnemyIconHeroArenaArray)
// 	{
// 		hero_icon->UpdateHeroHP();
// 	}
}



