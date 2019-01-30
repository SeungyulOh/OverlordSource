// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RCampaignHUD_OneControl.h"

#include "ProjectRGameMode.h"
#include "GameInfo/PlayStateInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/UserInfo.h"
#include "GlobalIntegrated.h"

#include "ContentGameMode/Modes/StageGameMode.h"
#include "ContentGameMode/Modes/ArenaGameMode.h"
#include "ContentGameMode/Modes/MirrorGameMode.h"

#include "RSkillSet_New.h"

#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
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
#include "UI/UI_Battle/InGameHeroUI/HeroUIforLeader.h"

#include "UI/HeroManagement/UC_Hero_Skill_Icon.h"

#include "UI/Chat/UC_Chat_Ingame.h"
#include "UI/InGameHUD/PVP/UC_HUD_PVP_TotalHP.h"
#include "UI/InGameHUD/UC_HUD_PhaseProgress.h"
#include "UI/InGameHUD/UC_HUD_TowerHP.h"
#include "UI/InGameHUD/UC_HUD_Goal.h"
#include "UI/GuildRaid/UC_GuildRaid_Group.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Skill_V2/NetSkillManager.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "Network/HttpRequestClient.h"
#include "Network/RTSManager.h"

#include "Runtime/UMG/Public/Components/SafeZone.h"
#include "GameState/Stage/Spawner/LevelActor.h"


#include "CustomActors/CrewSpiritParticleActor.h"
#include "Skill_V2/SharedSkill/SkillSlotContainer.h"

#define Hero_Icons_Y_General	19.0f
#define Hero_Icons_Y_Colosseum	100.0f

void URCampaignHUD_OneControl::NativeConstruct()
{
	Super::NativeConstruct();

	HideUnusedCharacterUI();



	SpiritHelper.ParentWidget = this;

	FHeroSlotInfo* battlecrew1Info = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetBattleActiveCrewSlot(ECharacterRoleEnum::VE_Crew1);
	FHeroSlotInfo* battlecrew2Info = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetBattleActiveCrewSlot(ECharacterRoleEnum::VE_Crew2);

	if (battlecrew1Info && battlecrew2Info)
		SpiritHelper.bActiveSpiritAll = (!battlecrew1Info->HeroUD.IsEmpty() && !battlecrew2Info->HeroUD.IsEmpty());
	else
		SpiritHelper.bActiveSpiritAll = false;

	SpiritHelper.Update(ECharacterRoleEnum::VE_Crew1);
	SpiritHelper.Update(ECharacterRoleEnum::VE_Crew2);
}

void URCampaignHUD_OneControl::NativeDestruct()
{
	for (auto& Element : SpiritHelper.CrewSpiritMap)
		Element.Value->Destroy();

	SpiritHelper.CrewSpiritMap.Empty();

	Super::NativeDestruct();
}

void URCampaignHUD_OneControl::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	

}



void URCampaignHUD_OneControl::UpdateHeroIconSlot()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;


	if(IsValid(LeaderIconUI))
		LeaderIconUI->UpdateIcon();

}

//////////////////////////////////





void URCampaignHUD_OneControl::HideUnusedCharacterUI()
{
	TArray<FHeroSlotInfo> HeroSlotInfoArray = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray();

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


	LeaderIconUI->SetHeroSlot(HeroSlotInfoArray[(uint8)EPartyRole::Leader]);
	LeaderIconUI->SetVisibility(ESlateVisibility::HitTestInvisible);
	LeaderIconUI->SetUIForCampaign(true);
	LeaderIconUI->HideUnUsedSpiritSkillIcon(bCrew1Battle, bCrew2Battle);

	//SkillSet->HideUnusedCharacterUI(bCanSwapCrew1, bCanSwapCrew2);
}





void URBaseIngameHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (RGAMEINSTANCE(this) == nullptr)
		return;

	if (RSafeZone && RSafeZone->MySafeZone.IsValid())
	{
		FMargin safemargin = RSafeZone->MySafeZone.Get()->GetSafeMargin();
		//if (IsValid(SkillSet))
		{
			//SkillSet->safe_margin = safemargin;
			//SkillSet->RefreshUIPos();
		}
	}

	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());

	if (IsValid(BasePlayerController))
		BasePlayerController->InGameHUD = this;

	JoyStickDirection = Cast<UUserWidget>(GetWidgetFromName(TEXT("UC_JoyStick")));
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo->ClearCondition == EPhaseClearEnumType::VE_Protect)
	{
		UI_ProtectMode->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	UpdateHeroIconSlot();

	if (IsValid(RGAMEINSTANCE(this)->EventManager))
	{
		RGAMEINSTANCE(this)->EventManager->OnIndicator.RemoveDynamic(this, &URCampaignHUD_OneControl::OnSkillIndicator);
		RGAMEINSTANCE(this)->EventManager->OnIndicator.AddDynamic(this, &URCampaignHUD_OneControl::OnSkillIndicator);
	}

	auto HeroIconsCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Hero_Icons);
	if (HeroIconsCanvasSlot)
	{
		FVector2D HeroIconsPosition = HeroIconsCanvasSlot->GetPosition();
		HeroIconsPosition.Y = Hero_Icons_Y_General;
		HeroIconsCanvasSlot->SetPosition(HeroIconsPosition);
	}

	/*Title Text Settings*/
	FFloorStageDetail* StageTableInfo = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
	if (StageTableInfo)
	{
		if (IsValid(LocalizingTextBlock_TitleText))
		{
			LocalizingTextBlock_TitleText->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->IngameSubTitle));
		}

		if (IsValid(LocalizingTextBlock_SubTitle))
		{
			LocalizingTextBlock_SubTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, StageTableInfo->Title));
		}
	}
	/**/

	bPause = UGameplayStatics::IsGamePaused(this);

	if (URealTimeModManager::IsRealTimeGameMode())
		AutoSkillIcon->SetVisibility(ESlateVisibility::Collapsed);
}

void URBaseIngameHUD::NativeDestruct()
{
	Super::NativeDestruct();
}

void URBaseIngameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo)
	{
		int32 nRemainTime = (int32)BaseStageInfo->GetRemainingGameTime();

		SetTimeText(nRemainTime);
	}

	UpdateHeroIconSlot();
}

void URBaseIngameHUD::SetTimeText(int32 InRemainTime)
{
	int32 nMin = 0;
	int32 nSec = 0;

	if (InRemainTime > 0)
	{
		nMin = (InRemainTime % 3600) / 60;
		nSec = InRemainTime % 60;
	}

	FText MinText = UUIFunctionLibrary::Get_0n_Number(nMin);
	FText SecText = UUIFunctionLibrary::Get_0n_Number(nSec);

	if (!RemainMinuteText || !RemainSecondText)
		return;

	RemainMinuteText->SetText(MinText);
	RemainSecondText->SetText(SecText);

	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo)
	{
		ALevelActor* lvActor = BaseStageInfo->GetLevelActor();
		if (lvActor)
		{
			ALevelActorPhase*	Phase = lvActor->GetLevelActorPhase();
			if (Phase && (Phase->ClearCondition == EPhaseClearEnumType::VE_Resisting || lvActor->IsResisting))
			{
				int32	ElapsedGameTime = lvActor->GetElapsedGameTime();
				nMin = ((ElapsedGameTime % 3600) / 60);
				nSec = (ElapsedGameTime % 60);

				MinText = UUIFunctionLibrary::Get_0n_Number(nMin);
				SecText = UUIFunctionLibrary::Get_0n_Number(nSec);

				if (IsValid(UI_ProtectMode->Text_TimeMinutes))
					UI_ProtectMode->Text_TimeMinutes->SetText(MinText);
				if (IsValid(UI_ProtectMode->Text_TimeSecond))
					UI_ProtectMode->Text_TimeSecond->SetText(SecText);

				if (IsValid(UI_ProtectMode2->Text_TimeMinutes))
					UI_ProtectMode2->Text_TimeMinutes->SetText(MinText);
				if (IsValid(UI_ProtectMode2->Text_TimeSecond))
					UI_ProtectMode2->Text_TimeSecond->SetText(SecText);
			}
		}
	}
}

int32 URBaseIngameHUD::GetSlotPosition(FHeroSlotInfo& InSlot)
{
	if (InSlot.CharacterRole == ECharacterRoleEnum::VE_Leader)
		return (int32)EPartyRole::Leader;
	else if (InSlot.CharacterRole == ECharacterRoleEnum::VE_None)
		return (int32)EPartyRole::None;
	else
	{
		if (InSlot.CharacterRole == ECharacterRoleEnum::VE_Crew1)
		{
			if (InSlot.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
				return (int32)EPartyRole::Crew1_First;
			else
				return (int32)EPartyRole::Crew1_Second;
		}
		else
		{
			if (InSlot.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
				return (int32)EPartyRole::Crew2_First;
			else
				return (int32)EPartyRole::Crew2_Second;
		}
	}
}

void URBaseIngameHUD::SetStarHUD(EPlayStageStarEnum StarCnt)
{
	if (!IsValid(UC_HUD_Star))
		return;

	UC_HUD_Star->UpdateImage(StarCnt);
}

void URBaseIngameHUD::ShowSkillIndicator(FName id, float time)
{
	if (IsValid(SkillIndicator))
	{
		SkillIndicator->SetVisibility(ESlateVisibility::HitTestInvisible);
		ShowTime = time;
		SkillIndicator->SetIndicator(id);
	}
}

void URBaseIngameHUD::OnSkillIndicator(FName id)
{
	if (id.IsNone())
	{
		if (IsValid(SkillIndicator))
		{
			SkillIndicator->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		ShowSkillIndicator(id, 3.0f);
	}
}

void FSpiritHelper::SetSpiritEffect(ECharacterRoleEnum InRole)
{
	TArray<FHeroSlotInfo> SlotArray = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetSelectedCharacterArray();
	for (size_t i = 0; i < SlotArray.Num(); ++i)
	{
		FHeroSlotInfo& HeroSlot = SlotArray[i];
		if (HeroSlot.CharacterRole != InRole)
			continue;

		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroSlot.HeroIDName);
		if (!ensure(CharacterTableInfo != nullptr))
			continue;

		FSkillTableInfo_V2* SpiritSkillTableInfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(CharacterTableInfo->SpiritPassive);
		if (!ensure(SpiritSkillTableInfo != nullptr))
			continue;

		USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SlotArray[ROLE_LEADER_INDEX].SpawnedTargetActor);
		if (!ensure(SkillComp != nullptr))
			continue;

		if (HeroSlot.CharacterCrewState != ECharacterCrewStateEnum::VE_Battle)
		{
			SkillComp->GetSkillSlotContainer()->RemoveSlotbyResourceID(SpiritSkillTableInfo->SkillResourceID);
			continue;
		}

		//SkillComp->GetSkillSlotContainer()->AddSpiritSkill(SpiritSkillTableInfo, CharacterTableInfo->SpiritPassive, SlotArray[ROLE_LEADER_INDEX].SpawnedTargetActor);

		//UParticleSystem* ps = UUtilFunctionLibrary::GetParticleSystemFromAssetID(SpiritSkillTableInfo->ParticleResourceAsset);//UUtilFunctionLibrary::GetParticleSystemFromTable(SpiritSkill->SkillResourceID);
		UParticleSystem* ps = UUtilFunctionLibrary::GetParticleSystemFromTable(SpiritSkillTableInfo->SkillResourceID);
		USkeletalMeshComponent* skelcomp = SlotArray[ROLE_LEADER_INDEX].SpawnedTargetActor->FindComponentByClass<USkeletalMeshComponent>();
		FVector location = skelcomp->GetSocketLocation(SOCKET_ROOT);

		if (CrewSpiritMap.Contains(InRole))
		{
			ACrewSpiritParticleActor* SpiritParticle = CrewSpiritMap[InRole];
			SpiritParticle->SetParticleData(ps, skelcomp, InRole);
		}
		else
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = SlotArray[ROLE_LEADER_INDEX].SpawnedTargetActor;
			SpawnParams.Instigator = SpawnParams.Owner->Instigator;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnParams.bNoFail = true;

			ACrewSpiritParticleActor* SpiritParticle = SlotArray[ROLE_LEADER_INDEX].SpawnedTargetActor->GetWorld()->SpawnActor<ACrewSpiritParticleActor>(ACrewSpiritParticleActor::StaticClass(), location, FRotator::ZeroRotator, SpawnParams);
			SpiritParticle->SetParticleData(ps, skelcomp, InRole);
			SpiritParticle->bAlone = !bActiveSpiritAll;
			CrewSpiritMap.Emplace(InRole, SpiritParticle);
		}

		// 		URBaseIngameHUD* OneGameHUD = Cast<URBaseIngameHUD>(GetParentWidget());
		// 		if (IsValid(OneGameHUD))
		// 		{
		// 			OneGameHUD->LeaderIconUI->SetActiveSpiritSkillIcon(InRole == ECharacterRoleEnum::VE_Crew1 ? 0 : 1, CharacterTableInfo->SpiritPassive);
		// 		}
	}
}

void FSpiritHelper::Update(ECharacterRoleEnum InRole)
{
	
	auto FindSkillName = [](FHeroSlotInfo& slotinfo) ->FName {
		FCharacterTableInfo* CharacterTableInfo1 = UTableManager::GetInstancePtr()->GetCharacterRow(slotinfo.HeroIDName);
		if (CharacterTableInfo1)
		{
			FName SkillIDName = CharacterTableInfo1->SpiritPassive;
			return SkillIDName;
		}
		return TEXT("");
	};

	FHeroSlotInfo* battleSlot = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetBattleActiveCrewSlot(InRole);

	if (battleSlot)
	{
		if(InRole == ECharacterRoleEnum::VE_Crew1)
			ParentWidget->LeaderIconUI->ActiveSpiritSkill01->SetSpiritSkill(FindSkillName(*battleSlot));
		else
			ParentWidget->LeaderIconUI->ActiveSpiritSkill02->SetSpiritSkill(FindSkillName(*battleSlot));

		SetSpiritEffect(InRole);
	}

	
		
	
	
}
