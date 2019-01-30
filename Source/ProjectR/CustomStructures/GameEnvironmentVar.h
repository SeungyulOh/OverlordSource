// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "GameEnvironmentVar.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FGameEnvironmentVar
{
	GENERATED_USTRUCT_BODY()

public:
	FGameEnvironmentVar();

	/*
	Lobby
	*/
	UPROPERTY()
		bool LobbyIsNight = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AOutdoorLobbyStateActor)
	float RightAbsOrbitYaw = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AOutdoorLobbyStateActor)
		float LeftAbsOrbitYaw = 20.0f;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AOutdoorLobbyStateActor)
// 		float MaxAbsOrbitXaw = 20.0f;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AOutdoorLobbyStateActor)
// 		float MinAbsOrbitXaw = 20.0f;

	// Ultimate Matinee
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GEV_Matinee)
	bool bUseUltimateMatinee = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GEV_Matinee)
	bool bUseUltimateMatineeCameraBlend = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Matinee)
	float UltimateMatineeCameraBlendTime = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Matinee)
	bool bUseUltimateMatineeFollowTarget = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Matinee)
	bool bUseUltimateReturnCamera = false;

	// SyncPeer
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_SyncPeer)
// 	float SyncPeerManagerInterval = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_SyncPeer)
	float RemoteMoveIgnoreDistance = 20.0f;

	// Network
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Colosseum)
	float NetConnectExpireTime = 25.0f;

	// Colosseum
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Colosseum)
// 	float ColosseumDeckSettingTime = 30.0f;
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Colosseum)
// 	float ColosseumHeroPickTime = 5.0f;
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Colosseum)
// 	float ColosseumHeroSpawnDelay = 5.0f;
// 	
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Colosseum)
// 	bool ColosseumRTSMode_Attack = true;
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Colosseum)
// 	int32 ColosseumRTSMode_TestUK = 0;
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Colosseum)
// 	int32 ColosseumRTSMode_SpawnCount = 5;
// 
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Colosseum)
// 	bool ColosseumAIModeTest = false;
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Colosseum)
// 	bool BattleFieldSyncTest = false;
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Colosseum)
// 	bool GuildRaidSyncTest = false;
	
	// Special effect

	// CommonHitEffect
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	FName CommonHitEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	FName CriticalHitEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	FName SynergyHitEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float HitHightlightDuration = 0.1f;	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	FColor HitHightlightColor = FColor::White;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	FColor HitCriticalHightlightColor = FColor::Red;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float ControlledCharacter_HitHightlightDuration = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	FColor ControlledCharacter_HitHightlightColor = FColor::White;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	FColor ControlledCharacter_HitCriticalHightlightColor = FColor::Red;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float Character_Selection_Decal_ScaleFactor = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float Character_SelectionGlow_Decal_ScaleFactor = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float Character_EnemyGlow_Decal_ScaleFactor = 1.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float PullMovingDuration = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float PushMovingDuration = 1.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float BulletSpeedMultifier = 1.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float DeadLaunchPower = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float					ZPos_SkillArea = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float					ZPos_SkillPickArea = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float					ZPos_Targeting = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float					ZPos_Select = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_MediaFrameWork)
	float Movie_Callback_WaitTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	float					DeathEffectDuration = 2.0f;

	/*
	Rule
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Rule)
	bool AlwaysShakeCameraOnHit = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Rule)
	float ExceptionVibrationRadius = 110.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GEV_Rule)
	float					OtherHeroDamageScale = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GEV_Rule)
	float					ArenaHeroDamageScale = 0.7f;


	/*
	AI
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_AI)
	float FollowLeaderDistance = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_AI)
	float CharacterOverlapScale = 0.7f;


	/*
	ForceTutorial
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Tutorial)
	bool ForceTutorialEnable = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Tutorial)
	bool InitTutorialEnable = true;

	/*
	Debugging
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	bool ShowUpHeadDebugingPanel = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	bool EnableResourceCache = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Effect)
	bool EnableSkillLog = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Debug)
	bool AlwaysShowHPBar = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Debug)
	bool LobbyPinchTest = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Debug)
	float LobbyPinchRestoreSensitivity = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Debug)
	bool LobbyHeroToManagement = false;
	
	/*
	Stage
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Stage)
	float									StageDirectionDist = 600.0f;

	// (Editor) Release level. Affects Prelogin only in Editor
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RGameInstance)
	EReleaseEnv							ReleaseLevel = EReleaseEnv::None;

	// battle object vibrate on damage
	float	DurationVibrationTime_Critical = 1.0f;
	float	DurationVibrationTime_Normal = 0.5f;
	
	/*
	Dialogue
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Render)
	float									DialogueIntensity_Main = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Render)
	float									DialogueIntensity_Rim = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_SkillTest)
	float									LeaderSightRange = 5000.0f;

	/*
	Target Effect
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Render)
	FColor									TargetEffectLightColor = FColor::Magenta;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Render)
	float									TargetEffectIntensity = 1.0f;

	/*
	Vignette value for CombineSkill
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Render)
	float									VignetteIntensity = 1.2f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Render)
	float									VignetteApplyDuration = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Render)
	float									PlayAloneCameraDistance = 1250.0f;
	
	/*
	Items - Bag
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Items)
	bool									IgnoreBagCapacityExceeded = false;

	/*
	SpiritSkill SkillEffect Control Value
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Spirit)
	float SpiritBackDistance = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Spirit)
	float SpiritLeftRightDistance = 50.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Spirit)
	float SpiritHeightDistance = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GEV_Spirit)
	float SpiritSpeed = 100.0f;


	/*OutLine Variables*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Outline_Targetting")
	float				OutLineElapsedTime = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outline_Targetting")
	float				OutLineDuration = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outline_Targetting")
	float				OutLineMinOffSet = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outline_Targetting")
	int32				FlickerCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_SkillMode")
	EAIControlModeEnum		AIControlMode = EAIControlModeEnum::VE_AutoMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GEV_Unlock)
	bool				bCheckContentsUnlock = true;

};