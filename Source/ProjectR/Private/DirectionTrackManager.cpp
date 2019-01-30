// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "DirectionTrackManager.h"
#include "Global/RGameInstance.h"
#include "Global/SequenceManager.h"
#include "Global/RealTimeModManager.h"
#include "Global/SoundManager.h"

#include "CustomActors/BaseSequenceActor.h"

#include "Utils/RenderFunctionLibrary.h"
#include "Utils/UtilFunctionLibrary.h"
#include "Utils/GamePlayBluePrintFunction.h"
#include "Object/BaseCameraActor.h"
#include "TutorialLevelScriptActor.h"

#include "Network/RTSManager.h"

#include "GameState/PlayState.h"

#include "GameInfo/PlayStateInfo.h"

#include "ContentGameMode/Modes/StageGameMode.h"
#include "UIFunctionLibrary.h"
#include "Network/PacketFunctionLibrary.h"
#include "CharacterHelpFunctionLibrary.h"
#include "Character/BattleBaseAnimInstance.h"

void UDirectionTrackManager::Update(float Delta)
{
	if (CurrentState == nullptr)
		return;
	if (!CurrentState->IsValidLowLevel())
		return;

	NextState = CurrentState->Update(Delta);

	if (NextState == CurrentState->CurrentStateEnum || NextState == EDirectionStateTypeEnum::VE_Max)
		return;

	//transition
	if (DirectionStateArray.IsValidIndex((int32)NextState))
	{
		if(CurrentState)
			CurrentState->Leave();

		CurrentState = DirectionStateArray[(int32)NextState];
		CurrentState->Enter();
	}

}


void UDirectionTrackManager::Empty_Heap()
{
	DE_Heap.Empty((int32)EDirectionEffectTypeEnum::DE_Max);
}

UDirectionState* UDirectionTrackManager::GetDirectionState(EDirectionStateTypeEnum InState)
{
	if(DirectionStateArray.IsValidIndex((int32)InState))
		return DirectionStateArray[(int32)InState];

	return nullptr;
}

int32 UDirectionTrackManager::GetSizeHeap()
{
	int32 size = (int32)DE_Heap.Num();

	return size;
}

void UDirectionTrackManager::Push_DirectionEffect(EDirectionEffectTypeEnum EffectType)
{
	if (URealTimeModManager::IsRealTimeGameMode() && EffectType < EDirectionEffectTypeEnum::DE_RTS_CS_PLAYLOSE)
		return;

	//Force state change to Idle state if input's priority is higher
	if(DE_Heap.Num() > 0 && EffectType < DE_Heap.HeapTop())
		Force_IdleState();

	while (DE_Heap.Num() > 0 && EffectType < DE_Heap.HeapTop())
	{
		EDirectionEffectTypeEnum TopNode;
		DE_Heap.HeapPop(TopNode);
	}

	DE_Heap.HeapPush(EffectType);
}

void UDirectionTrackManager::Pop_DirectionEffect(EDirectionEffectTypeEnum EffectType)
{
	if (URealTimeModManager::IsRealTimeGameMode() && EffectType < EDirectionEffectTypeEnum::DE_RTS_CS_PLAYLOSE)
		return;

	if (DE_Heap.Num() == 0)
		return;

	if (DE_Heap.Num() > 0 && DE_Heap.HeapTop() == EffectType)
		Force_IdleState();

	while (true)
	{
		if (DE_Heap.Num() == 0)
			break;

		EDirectionEffectTypeEnum top = DE_Heap.HeapTop();
		if (top == EffectType)
		{
			EDirectionEffectTypeEnum TopNode;
			DE_Heap.HeapPop(TopNode);
		}
		else if(top != EffectType)
			break;
	}	

	
}

void UDirectionTrackManager::Force_IdleState()
{
	if (DirectionStateArray.IsValidIndex((int32)EDirectionStateTypeEnum::VE_IdleDireciton))
	{
		if (CurrentState)
			CurrentState->Leave();

		CurrentState = DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_IdleDireciton];
		CurrentState->Enter();
	}
}

EDirectionStateTypeEnum UDirectionTrackManager::Check_CorresspondingState()
{
	if (DE_Heap.Num() == 0)
		return EDirectionStateTypeEnum::VE_IdleDireciton;

	EDirectionEffectTypeEnum top = DE_Heap.HeapTop();

	switch (top)
	{
	case EDirectionEffectTypeEnum::DE_Story_Dialogue:
		return EDirectionStateTypeEnum::VE_StoryDirection;
	case EDirectionEffectTypeEnum::DE_Stage_BossSpawnSequence:
	case EDirectionEffectTypeEnum::DE_Stage_VictoryPosition:
	case EDirectionEffectTypeEnum::DE_Stage_BossDead:
	case EDirectionEffectTypeEnum::DE_Stage_FailEvent:
		return EDirectionStateTypeEnum::VE_StageDirection;
	case EDirectionEffectTypeEnum::DE_Battle_CombineEffect:
	case EDirectionEffectTypeEnum::DE_Battle_CrewChangeEffect:
	case EDirectionEffectTypeEnum::DE_Battle_Vignette:
		return EDirectionStateTypeEnum::VE_BattleDirection;
	case EDirectionEffectTypeEnum::DE_RTS_CS_PLAYLOSE:
	case EDirectionEffectTypeEnum::DE_RTS_CS_PLAYWIN:
		return EDirectionStateTypeEnum::VE_RTSDirection;
	}

	return EDirectionStateTypeEnum::VE_Max;
	
}

void UStoryDirectionState::Enter()
{
	PPVolume->Settings = InitialPPSettings;

	//Disable Blendable Weight because blendables are only for Outline and RadialBlur Currently.
	for (size_t i = 0; i < PPVolume->Settings.WeightedBlendables.Array.Num(); ++i)
	{
		PPVolume->Settings.WeightedBlendables.Array[i].Weight = 0;
	}

	PPVolume->Settings.bOverride_DepthOfFieldMethod = true;
	PPVolume->Settings.bOverride_DepthOfFieldFocalDistance = true;
	PPVolume->Settings.bOverride_DepthOfFieldFarTransitionRegion = true;
	PPVolume->Settings.bOverride_MobileHQGaussian = false;
	PPVolume->Settings.bOverride_DepthOfFieldScale = true;

	PPVolume->Settings.DepthOfFieldMethod = EDepthOfFieldMethod::DOFM_Gaussian;
	PPVolume->Settings.DepthOfFieldFarTransitionRegion = 5000;
	PPVolume->Settings.DepthOfFieldFocalDistance = 150;
	PPVolume->Settings.DepthOfFieldScale = 1;

	PPVolume->Settings.bOverride_VignetteIntensity = false;

	UGamePlayBluePrintFunction::Deferred_ShowDialogue();

	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (IsValid(pGameMode))
	{
		pGameMode->ObjectHelper.SetAllObjectPause(MAX_FLOAT, 0.0f);
		pGameMode->ObjectHelper.SetAllObjectHPBarVisible(false);
		UUIFunctionLibrary::OnShowInGameHUD(RGAMEINSTANCE(GEngine)->GetWorld(), false);
		UUIFunctionLibrary::ActivateJoystick(RGAMEINSTANCE(GEngine)->GetWorld(), false);
	}
	

	ABaseCameraActor* BaseCam = RGAMEINSTANCE(GEngine)->CameraActor;
	if (BaseCam)
	{
		UCameraComponent* CamComp = BaseCam->FindComponentByClass<UCameraComponent>();
		if (CamComp)
		{
			APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(PPVolume->GetWorld(), 0);

			PPVolume->Settings.bOverride_DirectionalLightColorMovable = true;
			PPVolume->Settings.bOverride_DirectionalLightDirectionMovable = true;
			PPVolume->Settings.bOverride_DirectionalLightIntensityMovable = true;
			
			PPVolume->Settings.DirectionalLightDirectionMovable = PCM->GetCameraRotation();
			PPVolume->Settings.DirectionalLightIntensityMovable = 5.f;
			PPVolume->Settings.DirectionalLightColorMovable = FLinearColor(1.f, 1.f, 1.f);

			CamComp->PostProcessSettings.bOverride_DirectionalLightColorMovable = false;
			CamComp->PostProcessSettings.bOverride_DirectionalLightDirectionMovable = false;
			CamComp->PostProcessSettings.bOverride_DirectionalLightIntensityMovable = false;			
		}
	}
	
}

void UStoryDirectionState::Leave()
{
	//Enable Blendable Weight because blendables are only for Outline and RadialBlur Currently.
	for (size_t i = 0; i < PPVolume->Settings.WeightedBlendables.Array.Num(); ++i)
	{
		PPVolume->Settings.WeightedBlendables.Array[i].Weight = 1;
	}

	PPVolume->Settings.bOverride_DepthOfFieldMethod = false;
	PPVolume->Settings.bOverride_DepthOfFieldFocalDistance = false;
	PPVolume->Settings.bOverride_DepthOfFieldFarTransitionRegion = false;
	PPVolume->Settings.bOverride_MobileHQGaussian = false;
	PPVolume->Settings.bOverride_DepthOfFieldScale = false;
	PPVolume->Settings.bOverride_VignetteIntensity = true;
	
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)));
	if (IsValid(pGameMode))
	{
		pGameMode->ObjectHelper.SetAllObjectBreak();
		pGameMode->ObjectHelper.SetAllObjectHPBarVisible(true);
		UUIFunctionLibrary::OnShowInGameHUD(pGameMode->GetWorld(), true);
		UUIFunctionLibrary::InvalidateJoystick(pGameMode->GetWorld());
	}

	if (UUtilFunctionLibrary::isTutorialGameMode())
	{
		UUtilFunctionLibrary::GetTutorialLevelScriptActor()->OnDialogueCompleted.Broadcast();
	}
	
	
}


EDirectionStateTypeEnum UStoryDirectionState::Update(float Delta)
{
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(PPVolume->GetWorld(), 0);

	if(PCM->GetCameraRotation() != PPVolume->Settings.DirectionalLightDirectionMovable)
		PPVolume->Settings.DirectionalLightDirectionMovable = PCM->GetCameraRotation();

	return Super::Update(Delta);
}

void UStoryDirectionState::Cache_PPSettings()
{
	CachePPSettings = PPVolume->Settings;
	PPVolume->Settings = InitialPPSettings;

	//Disable Blendable Weight because blendables are only for Outline and RadialBlur Currently.
	for (size_t i = 0; i < PPVolume->Settings.WeightedBlendables.Array.Num(); ++i)
		PPVolume->Settings.WeightedBlendables.Array[i].Weight = 0;

	UPlayState* PlayState = Cast<UPlayState>(RGAMEINSTANCE(GEngine)->GetCurrentState());
	if (PlayState)
	{
		for (auto& Element : PlayState->WallMap)
			Element.Value->SetActorHiddenInGame(true);
	}

	AStageGameMode* StageGameMode = Cast<AStageGameMode>(UGameplayStatics::GetGameMode(PPVolume->GetWorld()));
	if (StageGameMode)
	{
		for (size_t i = 0; i < StageGameMode->DropItemActors.Num(); ++i)
		{
			if(StageGameMode->DropItemActors.IsValidIndex(i) && StageGameMode->DropItemActors[i])
				StageGameMode->DropItemActors[i]->SetActorHiddenInGame(true);
		}
	}

	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo)
	{
		TArray<AActor*> HeroArray = BaseStageInfo->GetHero();
		for (size_t i = 0; i < HeroArray.Num(); ++i)
		{
			if (HeroArray.IsValidIndex(i) && HeroArray[i])
				HeroArray[i]->SetActorHiddenInGame(true);
		}
	}
}

void UStoryDirectionState::Revert_PPSettings()
{
	PPVolume->Settings = CachePPSettings;

	UPlayState* PlayState = Cast<UPlayState>(RGAMEINSTANCE(GEngine)->GetCurrentState());
	if (PlayState)
	{
		for (auto& Element : PlayState->WallMap)
			Element.Value->SetActorHiddenInGame(false);
	}

	AStageGameMode* StageGameMode = Cast<AStageGameMode>(UGameplayStatics::GetGameMode(PPVolume->GetWorld()));
	if (StageGameMode)
	{
		for (size_t i = 0; i < StageGameMode->DropItemActors.Num(); ++i)
		{
			if (StageGameMode->DropItemActors.IsValidIndex(i) && StageGameMode->DropItemActors[i])
				StageGameMode->DropItemActors[i]->SetActorHiddenInGame(false);
		}
	}

	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo)
	{
		TArray<AActor*> HeroArray = BaseStageInfo->GetHero();
		for (size_t i = 0; i < HeroArray.Num(); ++i)
		{
			if (HeroArray.IsValidIndex(i) && HeroArray[i])
				HeroArray[i]->SetActorHiddenInGame(false);
		}
	}

}

void UBattleDirectionState::Enter()
{
	PPVolume->Settings = InitialPPSettings;

	if (UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.Num() == 0)
		return;

	EDirectionEffectTypeEnum CurrentEffect = UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.HeapTop();
	if (CurrentEffect == EDirectionEffectTypeEnum::DE_Battle_CrewChangeEffect)
	{
		FPostProcessStruct PPStruct;
		PPStruct.ePPType = FPostProcessStruct::SLOW;
		PPStruct.TargetValue = 0.2f;
		PPStruct.Duration = 0.2f;
		PPStruct.InitialValue = 1.f;
		PPArray.Emplace(PPStruct);
	}
	else if (CurrentEffect == EDirectionEffectTypeEnum::DE_Battle_Vignette)
	{
		FPostProcessStruct PPStruct;
		PPStruct.ePPType = FPostProcessStruct::VIGNETTE;
		PPStruct.TargetValue = InitialPPSettings.VignetteIntensity;
		PPStruct.Duration = 0.5f;
		PPStruct.InitialValue = InitialPPSettings.VignetteIntensity * 3.f;
		PPArray.Emplace(PPStruct);
	}
	else if (CurrentEffect == EDirectionEffectTypeEnum::DE_Battle_CombineEffect)
	{
		//Disable Blendable Weight because blendables are only for Outline and RadialBlur Currently.
		for (size_t i = 0; i < PPVolume->Settings.WeightedBlendables.Array.Num(); ++i)
		{
			PPVolume->Settings.WeightedBlendables.Array[i].Weight = 0;
		}
	}
}

void UBattleDirectionState::Leave()
{
	PPArray.Empty();

	EDirectionEffectTypeEnum CurrentEffect = UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.HeapTop();
	if (CurrentEffect == EDirectionEffectTypeEnum::DE_Battle_CombineEffect)
	{
		USequenceManager* SequenceManager = RGAMEINSTANCE(GEngine)->SequenceManager;
		if (SequenceManager)
		{
			ABaseSequenceActor* CurrentSeqActor = SequenceManager->GetCurrentSequenceActor();
			if (CurrentSeqActor)
				CurrentSeqActor->Stop();
		}
	}
	UGameplayStatics::SetGlobalTimeDilation(PPVolume->GetWorld(), 1.f);
}

EDirectionStateTypeEnum UBattleDirectionState::Update(float Delta)
{
	PostProcessUpdate(Delta);

	EDirectionStateTypeEnum ResultState = Exceptional_Transition();
	if (ResultState != EDirectionStateTypeEnum::VE_Max)
		return ResultState;

	return Super::Update(Delta);
}

EDirectionStateTypeEnum UBattleDirectionState::Exceptional_Transition()
{
	EDirectionEffectTypeEnum topEffect = UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.HeapTop();
	if (topEffect == EDirectionEffectTypeEnum::DE_Battle_Vignette)
	{
		if (PPArray.Num() == 0)
		{
			UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_Vignette);
			return EDirectionStateTypeEnum::VE_IdleDireciton;
		}
		else
			return EDirectionStateTypeEnum::VE_BattleDirection;
	}
	

	return EDirectionStateTypeEnum::VE_Max;
}

void UStageDirectionState::Enter()
{
	/*
	ToDo : this Resource should have been cached before. Fix later
	*/
	UObject* FoundObject = StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/Data/GameSetting/BossDead_SlowCurve.BossDead_SlowCurve'"));
	if (Cast<UCurveFloat>(FoundObject))
		BossDeadHelper.BossDeadSlowCurve = Cast<UCurveFloat>(FoundObject);

	FoundObject = StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/Data/GameSetting/BossDead_BlurCurve.BossDead_BlurCurve'"));
	if (Cast<UCurveFloat>(FoundObject))
		BossDeadHelper.BossDeadBlurCurve = Cast<UCurveFloat>(FoundObject);

	FoundObject = StaticLoadObject(USoundCue::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/Sound_0/Boss_Spawn/Boss_Die_Cue.Boss_Die_Cue'"));
	if (Cast<USoundCue>(FoundObject))
		BossDeadHelper.BossDeadSound = Cast<USoundCue>(FoundObject);
	//////////////////////////////////////////////////

	PPVolume->Settings = InitialPPSettings;

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(PPVolume->GetWorld(), 0);
	if (PCM)
	{
		PCM->StopAllCameraShakes(true);
		PCM->StopCameraFade();
	}

	//Disable Blendable Weight because blendables are only for Outline and RadialBlur Currently.
	for (size_t i = 0; i < PPVolume->Settings.WeightedBlendables.Array.Num(); ++i)
	{
		PPVolume->Settings.WeightedBlendables.Array[i].Weight = 0;
	}

	if (UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.Num() == 0)
		return;

	EDirectionEffectTypeEnum CurrentEffect = UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.HeapTop();
	if (CurrentEffect == EDirectionEffectTypeEnum::DE_Stage_BossDead)
	{
		if (BossDeadHelper.BossDeadSlowCurve)
		{
			TArray<FRichCurveKey> KeyArray = BossDeadHelper.BossDeadSlowCurve->FloatCurve.Keys;
			if (KeyArray.IsValidIndex(0))
			{
				BossDeadHelper.ElapsedTime = KeyArray[0].Time;
				BossDeadHelper.Duration += BossDeadHelper.ElapsedTime;
			}

			ABaseCameraActor * BaseCam = RGAMEINSTANCE(GEngine)->CameraActor;
			if (!BaseCam)
				return;

			BossDeadHelper.FirstCameraDistance = BaseCam->Camera_Distance;
		}

		AStageGameMode* StageGameMode = Cast<AStageGameMode>(UGameplayStatics::GetGameMode(PPVolume->GetWorld()));
		if(StageGameMode)
			StageGameMode->ObjectHelper.SetAllAIStop();

		if (IsValid(BossDeadHelper.BossDeadSound))
		{
			USoundManager::GetInstancePtr()->PlayComputeSound2D(RGAMEINSTANCE(this)->GetWorld(), BossDeadHelper.BossDeadSound);
			//UGameplayStatics::PlaySound2D(RGAMEINSTANCE(this)->GetWorld(), BossDeadHelper.BossDeadSound);
		}
	}
	else if (CurrentEffect == EDirectionEffectTypeEnum::DE_Stage_FailEvent)
	{
		if (BossDeadHelper.BossDeadSlowCurve)
		{
			TArray<FRichCurveKey> KeyArray = BossDeadHelper.BossDeadSlowCurve->FloatCurve.Keys;
			if (KeyArray.IsValidIndex(0))
			{
				BossDeadHelper.ElapsedTime = KeyArray[0].Time;
				BossDeadHelper.Duration += BossDeadHelper.ElapsedTime;
			}

			ABaseCameraActor * BaseCam = RGAMEINSTANCE(GEngine)->CameraActor;
			if (!BaseCam)
				return;

			BossDeadHelper.FirstCameraDistance = 1600.f;// BaseCam->Camera_Distance;
		}

		UPlayStateInfo* PlayStateInfo = RGAMEINSTANCE(GEngine)->PlayStateInfo;
		if (PlayStateInfo)
		{
			auto ForceDead = [&](ECharacterRoleEnum InRole) {
				AActor* ActiveCrew = PlayStateInfo->GetBattleActiveCrewActor(InRole);
				if (ActiveCrew)
				{
					UAnimStateComponent* AnimStateComp = UCharacterHelpFunctionLibrary::GetAnimStateComponent(ActiveCrew);
					if (AnimStateComp)
						AnimStateComp->DoCommand(ECharacterCommandEnum::VE_Die);

					UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(ActiveCrew);
					if (AIComp)
						AIComp->AIStop();

					UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(ActiveCrew);
					if (BattleEntityComp)
						BattleEntityComp->DeadClear();

					int32 Idx = PlayStateInfo->GetSelectedCharacterSlotIndex(ActiveCrew);
					PlayStateInfo->SetSpawnState(Idx, EHeroSpawnStateEnum::VE_Dead);
				}
			};

			AProjectRGameMode* ProjRGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)->GetWorld()));
			if (ProjRGameMode)
			{
				TArray<TWeakObjectPtr<AActor>> EnemyList;
				ProjRGameMode->ObjectHelper.GetEnemyList(EnemyList, PlayStateInfo->GetBattleActiveCrewActor(ECharacterRoleEnum::VE_Leader));
				for (size_t i = 0; i < EnemyList.Num(); ++i)
				{
					UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(EnemyList[i].Get());
					if (AIComp)
						AIComp->AIStop();
				}
			}

			ForceDead(ECharacterRoleEnum::VE_Leader);
			ForceDead(ECharacterRoleEnum::VE_Crew1);
			ForceDead(ECharacterRoleEnum::VE_Crew2);
			
		}
	}
	else if (CurrentEffect == EDirectionEffectTypeEnum::DE_Stage_VictoryPosition)
	{
		URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);

		AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance->GetWorld()));
		if (RGameMode == nullptr)
			return;

		if (RGameMode->GameContextState == EStageContextEnum::VE_Fail)
			return;

		RGameMode->StageClearSuccess(RGameMode->StarGrade);

		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_End, true);
		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Result, true);
		UUtilFunctionLibrary::ShowVictorySequence(GameInstance);
		UUtilFunctionLibrary::ShowVictoryAnim(GameInstance);
		
	}
	else if (CurrentEffect == EDirectionEffectTypeEnum::DE_Stage_BossSpawnSequence)
	{
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		TArray<AActor*>	Boss = BaseStageInfo->GetBoss();

		if (Boss.GetAllocatedSize() != 0)
		{
			if (Boss.Num() > 1)
				UGamePlayBluePrintFunction::SpawnMultiBoss(Boss);
			else
				UGamePlayBluePrintFunction::SpawnSingleBoss(Boss);
		}

		UGamePlayBluePrintFunction::PlayEventSequence();
	}
	
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo)
		BaseStageInfo->PauseTimer();


}

void UStageDirectionState::Leave()
{
	//Enable Blendable Weight because blendables are only for Outline and RadialBlur Currently.
	for (size_t i = 0; i < PPVolume->Settings.WeightedBlendables.Array.Num(); ++i)
	{
		PPVolume->Settings.WeightedBlendables.Array[i].Weight = 1;
	}

	UGameplayStatics::SetGlobalTimeDilation(PPVolume->GetWorld(), 1.f);
	PPArray.Empty();


	EDirectionEffectTypeEnum CurrentEffect = UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.HeapTop();
	if (CurrentEffect == EDirectionEffectTypeEnum::DE_Stage_BossDead)
	{
		UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		if (!BaseStageInfo)
			return;

		ALevelActor*	LevelActor = BaseStageInfo->GetLevelActor();
		if (LevelActor)
		{
			LevelActor->ConditionCheck(EPhaseClearEnumType::VE_BossKill);
			LevelActor->ConditionCheck(EPhaseClearEnumType::VE_CharacterAllSurvival);
		}
	}
	else if (CurrentEffect == EDirectionEffectTypeEnum::DE_Stage_FailEvent)
	{
		URGameInstance*	GameInstance = RGAMEINSTANCE(GEngine);

		AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance->GetWorld()));
		if (RGameMode == nullptr)
			return;
		if (RGameMode->GameContextState == EStageContextEnum::VE_Success)
			return;
		RGameMode->StageClearFail();

		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_End, true);
		UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum::VE_Result, true);
	}
	
	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo)
		BaseStageInfo->UnPauseTimer();

	
}

EDirectionStateTypeEnum UStageDirectionState::Update(float Delta)
{
	PostProcessUpdate(Delta);
	PostProcessUpdate_WithCurve(Delta);

	return Super::Update(Delta);
}

void UStageDirectionState::PostProcessUpdate_WithCurve(float Delta)
{
	if (UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.Num() == 0)
		return;

	EDirectionEffectTypeEnum CurrentEffect = UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.HeapTop();
	if (CurrentEffect == EDirectionEffectTypeEnum::DE_Stage_BossDead || 
		CurrentEffect == EDirectionEffectTypeEnum::DE_Stage_FailEvent)
	{
		if (BossDeadHelper.BossDeadSlowCurve) // slow
		{
			float CurrentValue = 0.f;
			if (BossDeadHelper.ElapsedTime < 0) 
			{
				CurrentValue = BossDeadHelper.BossDeadSlowCurve->GetFloatValue(BossDeadHelper.ElapsedTime);
				UGameplayStatics::SetGlobalTimeDilation(PPVolume->GetWorld(), CurrentValue);
			}
			else
			{
				CurrentValue = BossDeadHelper.BossDeadSlowCurve->GetFloatValue(BossDeadHelper.ElapsedTime / BossDeadHelper.Duration);
				UGameplayStatics::SetGlobalTimeDilation(PPVolume->GetWorld(), CurrentValue);
			}
		}

		if (BossDeadHelper.BossDeadBlurCurve) // blur
		{
			float CurrentValue = 0.f;
			if (BossDeadHelper.ElapsedTime < 0)
			{
				CurrentValue = BossDeadHelper.BossDeadBlurCurve->GetFloatValue(BossDeadHelper.ElapsedTime);
			}
			else
			{
				CurrentValue = BossDeadHelper.BossDeadBlurCurve->GetFloatValue(BossDeadHelper.ElapsedTime / BossDeadHelper.Duration);
			}

			ABaseCameraActor * BaseCam = RGAMEINSTANCE(GEngine)->CameraActor;
			if (!BaseCam)
				return;

			BaseCam->SetCameraDist(BossDeadHelper.FirstCameraDistance - BossDeadHelper.CameraDecreasedValue * CurrentValue);

			UCameraComponent* CamComp = BaseCam->GetCameraComponent();
			if (!CamComp)
				return;

			if (PPVolume->Settings.WeightedBlendables.Array.IsValidIndex(0))
			{
				FWeightedBlendable BlendMat = CamComp->PostProcessSettings.WeightedBlendables.Array[0];

				if (!PPVolume->Settings.WeightedBlendables.Array.IsValidIndex(1))
					PPVolume->Settings.WeightedBlendables.Array.Emplace(BlendMat);

				PPVolume->Settings.WeightedBlendables.Array[1].Weight = CurrentValue;
			}			
		}

		


		BossDeadHelper.ElapsedTime += Delta;
	}
}


void UDirectionTrackManager::Initialize()
{
	if(!DirectionStateArray.Num())
		DirectionStateArray.Empty((int32)EDirectionStateTypeEnum::VE_Max);

	UWorld* CurrentWorld = RGAMEINSTANCE(GEngine)->GetWorld();
	APostProcessVolume* PPVolume = nullptr;
	if (CurrentWorld)
	{
		for (size_t i = 0; i < CurrentWorld->PostProcessVolumes.Num(); ++i)
		{
			PPVolume = Cast<APostProcessVolume>(CurrentWorld->PostProcessVolumes[i]);
			//Enable Blendable Weight because blendables are only for Outline and RadialBlur Currently.
			for (size_t j = 0; j < PPVolume->Settings.WeightedBlendables.Array.Num(); ++j)
			{
				PPVolume->Settings.WeightedBlendables.Array[j].Weight = 1;
			}
			break;
		}
	}

	UDirectionState* DirectionState = nullptr;
	if (DirectionStateArray.IsValidIndex((int32)EDirectionStateTypeEnum::VE_IdleDireciton) && IsValid(DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_IdleDireciton]))
		DirectionState = DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_IdleDireciton];
	else
	{
		DirectionState = NewObject<UIdleDirectionState>((UObject*)GetTransientPackage(), UIdleDirectionState::StaticClass());
		DirectionStateArray.Emplace(DirectionState);
	}
	DirectionState->CurrentStateEnum = EDirectionStateTypeEnum::VE_IdleDireciton;
	DirectionState->PPVolume = PPVolume;
	DirectionState->InitialPPSettings = PPVolume->Settings;
	CurrentState = DirectionState;


	if (DirectionStateArray.IsValidIndex((int32)EDirectionStateTypeEnum::VE_BattleDirection) && IsValid(DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_BattleDirection]))
		DirectionState = DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_BattleDirection];
	else
	{
		DirectionState = NewObject<UBattleDirectionState>((UObject*)GetTransientPackage(), UBattleDirectionState::StaticClass());
		DirectionStateArray.Emplace(DirectionState);
	}
	DirectionState->CurrentStateEnum = EDirectionStateTypeEnum::VE_BattleDirection;
	DirectionState->PPVolume = PPVolume;
	DirectionState->InitialPPSettings = PPVolume->Settings;

	if (DirectionStateArray.IsValidIndex((int32)EDirectionStateTypeEnum::VE_StoryDirection) && IsValid(DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_StoryDirection]))
		DirectionState = DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_StoryDirection];
	else
	{
		DirectionState = NewObject<UStoryDirectionState>((UObject*)GetTransientPackage(), UStoryDirectionState::StaticClass());
		DirectionStateArray.Emplace(DirectionState);
	}
	DirectionState->CurrentStateEnum = EDirectionStateTypeEnum::VE_StoryDirection;
	DirectionState->PPVolume = PPVolume;
	DirectionState->InitialPPSettings = PPVolume->Settings;
	

	if (DirectionStateArray.IsValidIndex((int32)EDirectionStateTypeEnum::VE_StageDirection) && IsValid(DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_StageDirection]))
		DirectionState = DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_StageDirection];
	else
	{
		DirectionState = NewObject<UStageDirectionState>((UObject*)GetTransientPackage(), UStageDirectionState::StaticClass());
		DirectionStateArray.Emplace(DirectionState);
	}
	DirectionState->CurrentStateEnum = EDirectionStateTypeEnum::VE_StageDirection;
	DirectionState->PPVolume = PPVolume;
	DirectionState->InitialPPSettings = PPVolume->Settings;

	if (DirectionStateArray.IsValidIndex((int32)EDirectionStateTypeEnum::VE_RTSDirection) && IsValid(DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_RTSDirection]))
		DirectionState = DirectionStateArray[(int32)EDirectionStateTypeEnum::VE_RTSDirection];
	else
	{
		DirectionState = NewObject<URTSDirectionState>((UObject*)GetTransientPackage(), URTSDirectionState::StaticClass());
		DirectionStateArray.Emplace(DirectionState);
	}
	DirectionState->CurrentStateEnum = EDirectionStateTypeEnum::VE_RTSDirection;
	DirectionState->PPVolume = PPVolume;
	DirectionState->InitialPPSettings = PPVolume->Settings;


	DE_Heap.Empty((int32)EDirectionEffectTypeEnum::DE_Max);
	DE_Heap.Heapify([](const EDirectionEffectTypeEnum& Left , const EDirectionEffectTypeEnum& Right) {
		return Left > Right;
	});


}


EDirectionStateTypeEnum UDirectionState::Update(float Delta)
{

	EDirectionStateTypeEnum CurrentState = UUtilFunctionLibrary::GetDirectionTrackManager()->Check_CorresspondingState();
	if (CurrentState != EDirectionStateTypeEnum::VE_Max)
		return CurrentState;

	return EDirectionStateTypeEnum::VE_Max;
}

void UDirectionState::PostProcessUpdate(float Delta)
{
	for (size_t i = 0; i < PPArray.Num(); )
	{
		if (PPArray[i].ElapsedTime > PPArray[i].Duration)
		{
			PPArray.RemoveAt(i);
			continue;
		}

		PPArray[i].ElapsedTime += Delta;
		float CurrentValue = FMath::Lerp<float>(PPArray[i].InitialValue, PPArray[i].TargetValue, PPArray[i].ElapsedTime / PPArray[i].Duration);
		UE_LOG(LogClass, Log, TEXT("%f"), CurrentValue);

		switch (PPArray[i].ePPType)
		{
		case FPostProcessStruct::VIGNETTE:
			PPVolume->Settings.VignetteIntensity = CurrentValue;
			break;
		case FPostProcessStruct::BLOOM:
			PPVolume->Settings.BloomIntensity = CurrentValue;
			break;
		case FPostProcessStruct::SLOW:
			UGameplayStatics::SetGlobalTimeDilation(PPVolume->GetWorld(), CurrentValue);
			break;
		case FPostProcessStruct::RADIALBLUR:
			ABaseCameraActor * BaseCam = RGAMEINSTANCE(GEngine)->CameraActor;
			if (!BaseCam)
				break;

			UCameraComponent* CamComp = BaseCam->GetCameraComponent();
			if (!CamComp)
				break;
			
			if (PPVolume->Settings.WeightedBlendables.Array.IsValidIndex(0))
			{
				FWeightedBlendable BlendMat = CamComp->PostProcessSettings.WeightedBlendables.Array[0];

				if (!PPVolume->Settings.WeightedBlendables.Array.IsValidIndex(1))
					PPVolume->Settings.WeightedBlendables.Array.Emplace(BlendMat);

				PPVolume->Settings.WeightedBlendables.Array[1].Weight = CurrentValue;
			}
			break;
		}
		

		++i;
	}
}

void UIdleDirectionState::Enter()
{
	PPVolume->Settings = InitialPPSettings;

}

void UIdleDirectionState::Leave()
{

}


void URTSDirectionState::Enter()
{
	/*
	ToDo : this Resource should have been cached before. Fix later
	*/
	UObject* FoundObject = StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/Data/GameSetting/BossDead_SlowCurve.BossDead_SlowCurve'"));
	if (Cast<UCurveFloat>(FoundObject))
		ColosseumEndHelper.BossDeadSlowCurve = Cast<UCurveFloat>(FoundObject);

	FoundObject = StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/Data/GameSetting/BossDead_BlurCurve.BossDead_BlurCurve'"));
	if (Cast<UCurveFloat>(FoundObject))
		ColosseumEndHelper.BossDeadBlurCurve = Cast<UCurveFloat>(FoundObject);
	//////////////////////////////////////////////////

	PPVolume->Settings = InitialPPSettings;

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(PPVolume->GetWorld(), 0);
	if (PCM)
	{
		PCM->StopAllCameraShakes(true);
		PCM->StopCameraFade();
	}

	//Disable Blendable Weight because blendables are only for Outline and RadialBlur Currently.
	for (size_t i = 0; i < PPVolume->Settings.WeightedBlendables.Array.Num(); ++i)
	{
		PPVolume->Settings.WeightedBlendables.Array[i].Weight = 0;
	}

	if (UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.Num() == 0)
		return;

	EDirectionEffectTypeEnum CurrentEffect = UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.HeapTop();
	if (CurrentEffect == EDirectionEffectTypeEnum::DE_RTS_CS_PLAYLOSE)
	{
		if (ColosseumEndHelper.BossDeadSlowCurve)
		{
			TArray<FRichCurveKey> KeyArray = ColosseumEndHelper.BossDeadSlowCurve->FloatCurve.Keys;
			if (KeyArray.IsValidIndex(0))
			{
				ColosseumEndHelper.ElapsedTime = KeyArray[0].Time;
				ColosseumEndHelper.Duration += ColosseumEndHelper.ElapsedTime;
			}

			ABaseCameraActor * BaseCam = RGAMEINSTANCE(GEngine)->CameraActor;
			if (!BaseCam)
				return;

			ColosseumEndHelper.FirstCameraDistance = 1600.f;// BaseCam->Camera_Distance;
		}

		UPlayStateInfo* PlayStateInfo = RGAMEINSTANCE(GEngine)->PlayStateInfo;
		if (PlayStateInfo)
		{
			AProjectRGameMode* ProjectRGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)->GetWorld()));
			if (IsValid(ProjectRGameMode))
			{
				AActor* ActiveLeader = PlayStateInfo->GetBattleActiveCrewActor(ECharacterRoleEnum::VE_Leader);

				TArray<TWeakObjectPtr<AActor>> EnemyArray;
				ProjectRGameMode->ObjectHelper.GetEnemyList(EnemyArray, ActiveLeader);

				for (size_t i = 0; i < EnemyArray.Num(); ++i)
				{
					if (!EnemyArray[i].IsValid())
						continue;

					UBattleBaseAnimInstance* BattleAnim = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(EnemyArray[i].Get());
					if (BattleAnim)
					{
						BattleAnim->PlayAnimMontageEventSection(TEXT("BattleWin"));
					}
				}
			}


			auto ForceDead = [&](ECharacterRoleEnum InRole) {
				AActor* ActiveCrew = PlayStateInfo->GetBattleActiveCrewActor(InRole);
				if (ActiveCrew)
				{
					UAnimStateComponent* AnimStateComp = UCharacterHelpFunctionLibrary::GetAnimStateComponent(ActiveCrew);
					if (AnimStateComp)
						AnimStateComp->DoCommand(ECharacterCommandEnum::VE_Die);

					UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(ActiveCrew);
					if (AIComp)
						AIComp->AIStop();

					UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(ActiveCrew);
					if (BattleEntityComp)
						BattleEntityComp->DeadClear();

					int32 Idx = PlayStateInfo->GetSelectedCharacterSlotIndex(ActiveCrew);
					PlayStateInfo->SetSpawnState(Idx, EHeroSpawnStateEnum::VE_Dead);
				}
			};

			ForceDead(ECharacterRoleEnum::VE_Leader);
			ForceDead(ECharacterRoleEnum::VE_Crew1);
			ForceDead(ECharacterRoleEnum::VE_Crew2);
		}
	}
	else if (CurrentEffect == EDirectionEffectTypeEnum::DE_RTS_CS_PLAYWIN)
	{
		if (ColosseumEndHelper.BossDeadSlowCurve)
		{
			TArray<FRichCurveKey> KeyArray = ColosseumEndHelper.BossDeadSlowCurve->FloatCurve.Keys;
			if (KeyArray.IsValidIndex(0))
			{
				ColosseumEndHelper.ElapsedTime = KeyArray[0].Time;
				ColosseumEndHelper.Duration = 2.f; //ColosseumEndHelper.ElapsedTime;
			}

			ABaseCameraActor * BaseCam = RGAMEINSTANCE(GEngine)->CameraActor;
			if (!BaseCam)
				return;

			ColosseumEndHelper.FirstCameraDistance = 1600.f;// BaseCam->Camera_Distance;
		}

		UPlayStateInfo* PlayStateInfo = RGAMEINSTANCE(GEngine)->PlayStateInfo;
		if (PlayStateInfo)
		{
			AProjectRGameMode* ProjectRGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)->GetWorld()));
			if (IsValid(ProjectRGameMode))
			{
				AActor* ActiveLeader = PlayStateInfo->GetBattleActiveCrewActor(ECharacterRoleEnum::VE_Leader);

				TArray<TWeakObjectPtr<AActor>> EnemyArray;
				ProjectRGameMode->ObjectHelper.GetEnemyList(EnemyArray, ActiveLeader);

				for (size_t i = 0; i < EnemyArray.Num(); ++i)
				{
					if (!EnemyArray[i].IsValid())
						continue;

					UAnimStateComponent* AnimStateComp = UCharacterHelpFunctionLibrary::GetAnimStateComponent(EnemyArray[i].Get());
					if (AnimStateComp)
						AnimStateComp->DoCommand(ECharacterCommandEnum::VE_Die);

					UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(EnemyArray[i].Get());
					if (AIComp)
						AIComp->AIStop();

					UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(EnemyArray[i].Get());
					if (BattleEntityComp)
						BattleEntityComp->DeadClear();
				}

				TArray<TWeakObjectPtr<AActor>> FriendArray;
				ProjectRGameMode->ObjectHelper.GetFriendList(FriendArray, ActiveLeader);
				for (size_t i = 0; i < FriendArray.Num(); ++i)
				{
					if (!FriendArray[i].IsValid())
						continue;

					UBattleBaseAnimInstance* BattleAnim = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(FriendArray[i].Get());
					if (BattleAnim)
					{
						BattleAnim->PlayAnimMontageEventSection(TEXT("BattleWin"));
					}
				}
			}
		}
	}
}

void URTSDirectionState::Leave()
{
	//Enable Blendable Weight because blendables are only for Outline and RadialBlur Currently.
	for (size_t i = 0; i < PPVolume->Settings.WeightedBlendables.Array.Num(); ++i)
	{
		PPVolume->Settings.WeightedBlendables.Array[i].Weight = 1;
	}

	UGameplayStatics::SetGlobalTimeDilation(PPVolume->GetWorld(), 1.f);
	PPArray.Empty();


	UPacketFunctionLibrary::COLOSSEUM_PLAY_RESULT_RQ(RGAMEINSTANCE(GEngine)->RealTimeModManager->mRoomKey);
	URTSManager::GetInstancePtr()->REQ_BT_EXIT();
}

EDirectionStateTypeEnum URTSDirectionState::Update(float Delta)
{
	PostProcessUpdate(Delta);
	PostProcessUpdate_WithCurve(Delta);

	return Super::Update(Delta);
}

void URTSDirectionState::PostProcessUpdate_WithCurve(float Delta)
{
	if (UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.Num() == 0)
		return;

	EDirectionEffectTypeEnum CurrentEffect = UUtilFunctionLibrary::GetDirectionTrackManager()->DE_Heap.HeapTop();
	if (ColosseumEndHelper.BossDeadSlowCurve) // slow
	{
		float CurrentValue = 0.f;
		if (ColosseumEndHelper.ElapsedTime < 0)
		{
			CurrentValue = ColosseumEndHelper.BossDeadSlowCurve->GetFloatValue(ColosseumEndHelper.ElapsedTime);
			UGameplayStatics::SetGlobalTimeDilation(PPVolume->GetWorld(), CurrentValue);
		}
		else
		{
			CurrentValue = ColosseumEndHelper.BossDeadSlowCurve->GetFloatValue(ColosseumEndHelper.ElapsedTime / ColosseumEndHelper.Duration);
			UGameplayStatics::SetGlobalTimeDilation(PPVolume->GetWorld(), CurrentValue);
		}
	}

	if (ColosseumEndHelper.BossDeadBlurCurve) // blur
	{
		float CurrentValue = 0.f;
		if (ColosseumEndHelper.ElapsedTime < 0)
		{
			CurrentValue = ColosseumEndHelper.BossDeadBlurCurve->GetFloatValue(ColosseumEndHelper.ElapsedTime);
		}
		else
		{
			CurrentValue = ColosseumEndHelper.BossDeadBlurCurve->GetFloatValue(ColosseumEndHelper.ElapsedTime / ColosseumEndHelper.Duration);
		}

		ABaseCameraActor * BaseCam = RGAMEINSTANCE(GEngine)->CameraActor;
		if (!BaseCam)
			return;

		BaseCam->SetCameraDist(ColosseumEndHelper.FirstCameraDistance - ColosseumEndHelper.CameraDecreasedValue * CurrentValue);

		UCameraComponent* CamComp = BaseCam->GetCameraComponent();
		if (!CamComp)
			return;

		if (PPVolume->Settings.WeightedBlendables.Array.IsValidIndex(0))
		{
			FWeightedBlendable BlendMat = CamComp->PostProcessSettings.WeightedBlendables.Array[0];

			if (!PPVolume->Settings.WeightedBlendables.Array.IsValidIndex(1))
				PPVolume->Settings.WeightedBlendables.Array.Emplace(BlendMat);

			PPVolume->Settings.WeightedBlendables.Array[1].Weight = CurrentValue;
		}
	}

	ColosseumEndHelper.ElapsedTime += Delta;

	if (CurrentEffect == EDirectionEffectTypeEnum::DE_RTS_CS_PLAYWIN)
	{
		if (ColosseumEndHelper.ElapsedTime > ColosseumEndHelper.Duration)
			UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_RTS_CS_PLAYWIN);
	}
}
