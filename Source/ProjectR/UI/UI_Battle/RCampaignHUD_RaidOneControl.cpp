// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RCampaignHUD_RaidOneControl.h"
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

void URCampaignHUD_RaidOneControl::NativeConstruct()
{
	if (!HeroIconArray.Num())
	{
		HeroIconArray.Empty(5);
		HeroIconArray.Emplace(LeaderIconUI);
		HeroIconArray.Emplace(MemberUI_01);
		HeroIconArray.Emplace(MemberUI_02);
		HeroIconArray.Emplace(MemberUI_03);
		HeroIconArray.Emplace(MemberUI_04);
	}
	
	Super::NativeConstruct();

}

void URCampaignHUD_RaidOneControl::NativeDestruct()
{
	Super::NativeDestruct();
}

void URCampaignHUD_RaidOneControl::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UpdateHeroIconSlot();
	RenderTime();
}


void URCampaignHUD_RaidOneControl::UpdateHeroIconSlot()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;

	for (URHeroUIforBase* slot : HeroIconArray)
	{
		slot->RTSUpdateIcon();
	}
}

void URCampaignHUD_RaidOneControl::RenderTime()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FString MinText, SecText;
	UDescriptionFunctionLibrary::GetTimeStr(RGameInstance->RealTimeModManager->GetGameTime(), MinText, SecText);
	if (IsValid(Text_Min))
		Text_Min->SetText(FText::FromString(MinText));
	if (IsValid(Text_Sec))
		Text_Sec->SetText(FText::FromString(SecText));
}





void URCampaignHUD_RaidOneControl::HideUnusedCharacterUI()
{
	TMap<int16, FRTS_ACTOR>			ActorList = RGAMEINSTANCE(this)->RealTimeModManager->ActorList;

	for (URHeroUIforBase* faceicon : HeroIconArray)
	{
		faceicon->SetVisibility(ESlateVisibility::Collapsed);
	}
	ARaidRGameMode* GameMode = Cast<ARaidRGameMode>(UUtilFunctionLibrary::GetGamePlayMode());
	if (!IsValid(GameMode))
		return;

	int32 Position = 1;

	for (size_t i = 1; i <= ActorList.Num(); ++i)
	{
		if (ActorList[i].team == UObjectFunctionLibrary::GetMyTeamKey())
		{
			TWeakObjectPtr<AActor> PlayerActor = GameMode->ObjectHelper.GetObjectByPlayerUK(ActorList[i].actorUK);
			if (PlayerActor.IsValid())
			{
				FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ActorList[i].actorId);

				if (CharacterTableInfo == nullptr)
					continue;

				int32 RealPosition = Position;

				if (RGAMEINSTANCE(this)->HttpClient->GetKID() == ActorList[i].kId)
					RealPosition = 0;

				FHeroSlotInfo CommandSlot;
				CommandSlot.SpawnedTargetActor = PlayerActor.Get();
				CommandSlot.UserKID = ActorList[i].kId;
				CommandSlot.HeroUD = ActorList[i].heroUD;
				CommandSlot.HeroLevel = ActorList[i].level;
				CommandSlot.HeroIDName = ActorList[i].actorId;
				CommandSlot.HeroHP = ActorList[i].curHp;
				CommandSlot.PlayerUK = ActorList[i].actorUK;
				CommandSlot.CharacterCategoryType = ECharacterCategoryTypeEnum::VE_Hero;
				CommandSlot.SlotSpawnState = EHeroSpawnStateEnum::VE_Ready;
				CommandSlot.PortraitTextureKey = CharacterTableInfo->PortraitTextureKey;
				CommandSlot.CharacterClassType = CharacterTableInfo->Class;
				CommandSlot.CharacterCrewState = UGamePlayFunctionLibrary::IndexToCrewState(RealPosition);
				CommandSlot.CharacterRole = UGamePlayFunctionLibrary::IndexToRoleEnum(RealPosition);

				HeroIconArray[RealPosition]->SetRTSHeroSlot(CommandSlot);
				HeroIconArray[RealPosition]->RTSUpdateIcon();
				HeroIconArray[RealPosition]->SetVisibility(ESlateVisibility::HitTestInvisible);

				if (RGAMEINSTANCE(this)->HttpClient->GetKID() != ActorList[i].kId)
					Position++;
			}
		}
	}

	LeaderIconUI->SetUIForMultiRaid();

}

