// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "TutorialLevelScriptActor.h"
#include "GamePlayBluePrintFunction.h"
#include "DirectionTrackManager.h"
#include "UI/Dialogue/UP_Dialogue.h"
#include "UtilFunctionLibrary.h"
#include "GameState/TowerLobbyState.h"
#include "UI/RWidgetManager.h"
#include "Global/RGameInstance.h"
#include "CharacterHelpFunctionLibrary.h"
#include "ObjectFunctionLibrary.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "GameInfo/PlayStateInfo.h"
#include "UIFunctionLibrary.h"
#include "Character/Components/SpeechBubbleComponent.h"
#include "TutorialGameMode.h"
#include "UI/UI_Battle/Component/RAutoSkillIcon.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/RCampaignHUD_OneControl.h"
#include "ResourceCacheManager.h"
#include "UI/UI_Battle/RBattleOptionMenu.h"


void ATutorialLevelScriptActor::ShowDialogue(TArray<int32> DialogueIndexes, TMap<int32, FCustomizeDialogue> OverrideMap)
{
	UGamePlayBluePrintFunction::SavedDialogueInfo = DialogueIndexes;
	UGamePlayBluePrintFunction::SavedInfoMap = OverrideMap;
	UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Story_Dialogue);
}

AActor* ATutorialLevelScriptActor::SpawnLeader(AActor* PositionActor, FName CharacterID)
{
	URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	/**/
	FTransform transform;
	transform.SetLocation(PositionActor->GetActorLocation());

	bool bFirst = false;
	if (IsValid(GameInstance->PlayStateInfo))
	{
		TArray<int32> Skill;
		Skill.Init(-1, 5);
		TArray<FTestItemInfo> Item;
		GameInstance->PlayStateInfo->AddCharacterSlotForTestPlay(CharacterID, 1, Skill, Item);
	}
		

	bool bMy = true;
	int32 UserKID = bMy ? RGAMEINSTANCE(this)->HttpClient->GetKID() : KID_NONE;
	int32 GroupKey = bMy ? GroupKey_SINGLE_MY : GroupKey_SINGLE_ENEMY;
	int32 PartyID = bMy ? PartyID_PC_MY : PartyID_NPC_ENEMY;

	AActor* character = UObjectFunctionLibrary::SpawnCharacter(transform, CharacterID, ECharacterCategoryTypeEnum::VE_Hero,
		UserKID, HERO_UD_NONE, GroupKey, PartyID, PLAYER_UK_NONE,
		1, 1, true, ROLE_LEADER_INDEX);
	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(character);
	UBattleEntityComponent* BattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(character);
	if (IsValid(character))
	{
		if (bMy)
		{
			AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameWorld));
			if (StagePC)
			{
				StagePC->ActorSettingOnTheCamera(character, 0.5f, 0.5f);
				StagePC->SetSelectedCharacter(character);
				StagePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);

				

				ATutorialGameMode* TutorialGameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(GameWorld));
				if (TutorialGameMode)
				{
					TutorialGameMode->ObjectHelper.AddObject(character, PLAYER_UK_NONE);
				}

			}
			GameInstance->PlayStateInfo->SetSpawnedCharacter(ROLE_LEADER_INDEX, character);

		}

		
		UUIFunctionLibrary::ActivateJoystick(GameInstance->GetWorld(), false);

		ATutorialGameMode* TutorialGameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
		if (TutorialGameMode)
		{
			TutorialGameMode->ObjectHelper.SetAllObjectHPBarVisible(false);
		}

		return character;
	}
	
	return nullptr;
	/**/
}

AActor* ATutorialLevelScriptActor::SpawnEnemy(AActor* PositionActor, FName EnemyID)
{
	FTransform transform;
	transform.SetLocation(PositionActor->GetActorLocation());
	FRotator rot = FRotator(0.f, -180.f, 0.f);
	transform.SetRotation(rot.Quaternion());

	int32 UserKID = KID_NONE;
	int32 GroupKey = GroupKey_SINGLE_ENEMY;
	int32 PartyID = PartyID_NPC_ENEMY;

	AActor* SpawnedActor = UObjectFunctionLibrary::SpawnCharacter(transform, EnemyID,  ECharacterCategoryTypeEnum::VE_Npc , UserKID, HERO_UD_NONE, GroupKey, PartyID, PLAYER_UK_NONE, 1);
	ATutorialGameMode* TutorialGameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
	if (TutorialGameMode)
	{
		TutorialGameMode->ObjectHelper.AddObject(SpawnedActor, PLAYER_UK_NONE);
	}

	UBattleBaseAnimInstance* BattleAnim = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(SpawnedActor);
	if (BattleAnim)
	{
		BattleAnim->PlayAnimMontageEventSection(TEXT("BattleReady"));
	}

	return SpawnedActor;
}

AActor* ATutorialLevelScriptActor::SpawnNeutral(AActor* PositionActor, FName NeutralID)
{
	FTransform transform;
	transform.SetLocation(PositionActor->GetActorLocation());

	int32 UserKID = KID_NONE;
	int32 GroupKey = GroupKey_SINGLE_ENEMY;
	int32 PartyID = PartyID_NPC_ENEMY;

	AActor* SpawnedActor = UObjectFunctionLibrary::SpawnCharacter(transform, NeutralID, ECharacterCategoryTypeEnum::VE_NeutralNpc, UserKID, HERO_UD_NONE, GroupKey, PartyID, PLAYER_UK_NONE, 1);
	ATutorialGameMode* TutorialGameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
	if (TutorialGameMode)
	{
		TutorialGameMode->ObjectHelper.AddObject(SpawnedActor, PLAYER_UK_NONE);
	}
	return SpawnedActor;
}

void ATutorialLevelScriptActor::SpeechBubblePlay(AActor* TargetActor, FName InSpeechKey, float InSpeechShowTime, float InDelay)
{
	UEntityRenderComponent* RenderComp = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(TargetActor);
	if (IsValid(RenderComp))
	{
		FTransform HeadUpTransform = RenderComp->GetHeadUpTransform();
		USpeechBubbleComponent* SpeechBubbleComponent = UUtilFunctionLibrary::AttachCharacterSpeechBubbleWidget(TargetActor, FName("WBP_SpeechBubble"), HeadUpTransform);
		if (IsValid(SpeechBubbleComponent))
		{
			SpeechBubbleComponent->SetSpeechBubbleShowTime(InSpeechShowTime);
			SpeechBubbleComponent->AddSpeechBubble(InSpeechKey, InDelay);
		}
	}
}

void ATutorialLevelScriptActor::MoveCamera(AActor* PositionActor, float InCameraMove , float InCameraExp)
{
	AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(GEngine)->GetWorld()));
	if (StagePC)
	{
		StagePC->ActorSettingOnTheCamera(PositionActor, InCameraMove, InCameraExp);
	}
}

void ATutorialLevelScriptActor::ActorsForceStop(TArray<AActor*> ActorList, bool bStop)
{
	for (auto actor : ActorList)
	{
		if (IsValid(actor))
		{
			UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(actor);
			if (IsValid(AIComp))
				AIComp->SetForcePause(bStop, MAX_FLOAT);
		}
	}

	UUIFunctionLibrary::OnShowInGameHUD(RGAMEINSTANCE(this)->GetWorld(), !bStop);
}

void ATutorialLevelScriptActor::ForceTargetSetting(AActor* OwnerActor, AActor* TargetActor)
{
	UEntityAIComponent* AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor);
	if (AIComponent)
	{
		AIComponent->SetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget, TargetActor);

		//UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Stage_TutorialBattleStart);
	}
}

void ATutorialLevelScriptActor::ChangeLeaderAI(EAIControlModeEnum InType)
{
	AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GetWorld()));
	if (StagePC)
	{
		StagePC->AIControlMode_UserInput = InType;
		StagePC->SetLeaderCharacterAIAutoMode(InType);
	}
}

void ATutorialLevelScriptActor::CameraFade(bool bStart, float Duration)
{
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(this)->GetWorld(), 0);
	if (PCM)
	{
		ATutorialGameMode* TutorialGameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
		if (TutorialGameMode)
		{
			TutorialGameMode->ObjectHelper.SetAllObjectHPBarVisible(false);
		}

		if (bStart)
		{
			PCM->StartCameraFade(0.f, 1.f, Duration, FLinearColor::Black , false, true);
		}
		else
		{
			PCM->StartCameraFade(1.f, 0.f, Duration, FLinearColor::Black);
		}
	}
}

void ATutorialLevelScriptActor::BattleStart()
{
	ATutorialGameMode* TutorialGameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
	if (TutorialGameMode)
	{
		
		TutorialGameMode->ObjectHelper.SetAllObjectHPBarVisible(true);
		UUIFunctionLibrary::ActivateJoystick(RGAMEINSTANCE(this)->GetWorld(), true);

		RGAMEINSTANCE(this)->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_InGameHUD_Three);
		URCampaignHUD_ThreeControl* hud = Cast<URCampaignHUD_ThreeControl>(RGAMEINSTANCE(this)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Campaign_Three));
		if (hud)
		{
			hud->CampaignStageInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
			hud->AutoSkillIcon->SetVisibility(ESlateVisibility::Collapsed);
			hud->SkillSet->Variables.BG_3Combine->SetVisibility(ESlateVisibility::Collapsed);
			hud->SkillSet->Variables.FocusAttackIcon->SetVisibility(ESlateVisibility::Collapsed);
			hud->SkillSet->Variables.CancelIcon->SetVisibility(ESlateVisibility::Collapsed);
			hud->SkillSet->Variables.MeetingIcon->SetVisibility(ESlateVisibility::Collapsed);
			hud->SkillSet->Variables.SearchTargetIcon->SetVisibility(ESlateVisibility::Collapsed);
			hud->OptionMenu->SetVisibility(ESlateVisibility::Collapsed);
		}

		AStagePlayerController* StagePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GetWorld()));
		if (StagePC)
		{
			StagePC->AIControlMode_UserInput = EAIControlModeEnum::VE_SemiAutoMode;
			StagePC->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_SemiAutoMode);
		}
			
	}
}

void ATutorialLevelScriptActor::EndTutorial()
{
	URGameInstance* GameInst = RGAMEINSTANCE(GEngine);

	UResourceCacheManager* ResourceCacheManager = GameInst->ResourceCacheManager;
	if (IsValid(ResourceCacheManager))
		ResourceCacheManager->Clear();

	if (IsValid(GameInst))
	{
		FStreamableDelegate Callback;
		Callback.BindLambda([GameInst]() {
			GameInst->ResourceCacheManager->OnAsyncLoadedCompleted.ExecuteIfBound();

			GameInst->RWidgetManager->AddUIEventInt(EUIEventEnum::VE_GoContent, (int32)EUIMenuIDEnum::VE_ShopHero);
			UTowerLobbyState* TowerState = Cast<UTowerLobbyState>(*(GameInst->GameStates.Find(EGameStateEnum::VE_Lobby)));
			if (TowerState)
				TowerState->SetIndoorStart(true);

			GameInst->ChangeState(EGameStateEnum::VE_Lobby);
		});

		GameInst->ResourceCacheManager->cache_HeroShop(Callback);
	}
}
