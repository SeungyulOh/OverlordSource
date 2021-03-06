// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_InGameHUD_ArenaTemp.h"
#include "ProjectRGameMode.h"
#include "GlobalIntegrated.h"
//





#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"

//#include "Character/Party/Party.h"
//#include "UI/UI_Battle/Component/RCameraIcon.h"
#include "UI/RWidgetManager.h"
#include "UI/InGameHUD/PVP/UC_HUD_Icon_Hero_Arena.h"
#include "UI/InGameHUD/PVP/UC_HUD_PVP_TotalHP.h"
#include "UI/InGameHUD/UC_HUD_KillNotify.h"
#include "UI/UI_Battle/RSkillIcon.h"
#include "UI/UI_Battle/Component/RToggleCameraIcon.h"
#include "UI/UI_Battle/RBattleOptionMenu.h"
#include "GameInfo/UserInfo.h"
#include "Network/PacketDataStructures.h"
#include "UtilFunctionIntegrated.h"
#include "Object/BaseCameraActor.h"


void UUP_InGameHUD_ArenaTemp::NativeConstruct()
{
	Super::NativeConstruct();

	SequenceCamNames.Reset();

	SequenceCamNames.Emplace(FName(TEXT("Arena_Cam01_Loop")));
	SequenceCamNames.Emplace(FName(TEXT("Arena_Cam02_Loop")));


}

void UUP_InGameHUD_ArenaTemp::Init()
{
	CameraMode = EArenaCameraMode::VE_Sequence_1;

	RGameInstance = RGAMEINSTANCE(this);
	if(RGameInstance)
	{
		CurrentGamePlayMode = UUtilFunctionLibrary::GetGamePlayModeEnum();

		if (RGameInstance->EventManager)
		{
			if(RGameInstance->EventManager->OnClickToggleCameraIcon.IsAlreadyBound( this, &UUP_InGameHUD_ArenaTemp::OnClickCameraMode ) )
				RGameInstance->EventManager->OnClickToggleCameraIcon.RemoveDynamic(this, &UUP_InGameHUD_ArenaTemp::OnClickCameraMode);

			RGameInstance->EventManager->OnClickToggleCameraIcon.AddDynamic(this, &UUP_InGameHUD_ArenaTemp::OnClickCameraMode);
		}
		PrepareWidget();
	}
}

TArray<AActor*>& UUP_InGameHUD_ArenaTemp::GetUDList()
{
	switch (CurrentGamePlayMode)
	{
	case EGamePlayModeTypeEnum::VE_Arena:
	{
		if(IsValid(RGameInstance->ArenaManager))
		{
			return RGameInstance->ArenaManager->mMyHeroList;
		}
	}
	break;
	
	case EGamePlayModeTypeEnum::VE_MirrorDungeon:
	{
		if (IsValid(RGameInstance->MirrorDungeonManager))
		{
			return RGameInstance->MirrorDungeonManager->mMyHeroList;
		}
	}
	break;

	default:
		break;
	}

	static TArray<AActor*> EmptyArray;
	return EmptyArray;
}

TArray<AActor*>& UUP_InGameHUD_ArenaTemp::GetEnemyUDList()
{
	switch (CurrentGamePlayMode)
	{
	case EGamePlayModeTypeEnum::VE_Arena:
	{
		if (IsValid(RGameInstance->ArenaManager))
		{
			return RGameInstance->ArenaManager->mEnemyList;
		}
	}
	break;

	case EGamePlayModeTypeEnum::VE_MirrorDungeon:
	{
		if (IsValid(RGameInstance->MirrorDungeonManager))
		{
			return RGameInstance->MirrorDungeonManager->mEnemyList;
		}
	}
	break;

	default:
		break;
	}

	static TArray<AActor*> EmptyArray;
	return EmptyArray;
}

int32 UUP_InGameHUD_ArenaTemp::GetMaximumTotalHP()
{
	switch (CurrentGamePlayMode)
	{
	case EGamePlayModeTypeEnum::VE_Arena:
	{
		if (IsValid(RGameInstance->ArenaManager))
		{
			return RGameInstance->ArenaManager->GetMyMaximumTotalHP();
		}
	}
	break;

	case EGamePlayModeTypeEnum::VE_MirrorDungeon:
	{
		if (IsValid(RGameInstance->MirrorDungeonManager))
		{
			return RGameInstance->MirrorDungeonManager->GetMyMaximumTotalHP();
		}
	}
	break;

	default:
		break;
	}

	return 0;
}

int32 UUP_InGameHUD_ArenaTemp::GetTargetMaximumTotalHP()
{
	switch (CurrentGamePlayMode)
	{
	case EGamePlayModeTypeEnum::VE_Arena:
	{
		if (IsValid(RGameInstance->ArenaManager))
		{
			return RGameInstance->ArenaManager->GetTargetMaximumTotalHP();
		}
	}
	break;

	case EGamePlayModeTypeEnum::VE_MirrorDungeon:
	{
		if (IsValid(RGameInstance->MirrorDungeonManager))
		{
			return RGameInstance->MirrorDungeonManager->GetTargetMaximumTotalHP();
		}
	}
	break;

	default:
		break;
	}

	return 0;
}

void UUP_InGameHUD_ArenaTemp::GetMatchTargetInfo(FBATTLE_USER& target_info)
{
	switch (CurrentGamePlayMode)
	{
	case EGamePlayModeTypeEnum::VE_Arena:
		if (IsValid(RGameInstance->ArenaManager))
		{
			RGameInstance->ArenaManager->GetMatchTargetInfo(target_info);
		}
		break;
	case EGamePlayModeTypeEnum::VE_MirrorDungeon:
		if (IsValid(RGameInstance->MirrorDungeonManager))
		{
			RGameInstance->MirrorDungeonManager->GetMatchTargetInfo(target_info);
		}
	default:
		break;
	}
}

void UUP_InGameHUD_ArenaTemp::InitMyHeroIcons()
{
	for (int32 nIndex = 0; nIndex < EnemyIconHeroArenaArray.Num(); ++nIndex)
	{
		bool bUsing = false;

		TArray<AActor*>& rMyHeroList = GetUDList();
		if (rMyHeroList.IsValidIndex(nIndex))
		{
			AActor* Spawner = rMyHeroList[nIndex];
			if (IsValid(Spawner))
			{
				UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Spawner);
				if (IsValid(BattleEntityComponent))
				{
					MyIconHeroArenaArray[nIndex]->Init(Spawner, true);
					MyIconHeroArenaArray[nIndex]->InitHero();
					bUsing = true;
				}
			}
		}
		if (bUsing == false)
			MyIconHeroArenaArray[nIndex]->SetVisibility(ESlateVisibility::Hidden);
	}
	if(UC_TotalHP)
	{
		FAVATAR& Avatar = RGameInstance->UserInfo->GetAvatarData();
		FString guildnamedummy = TEXT("");
		UC_TotalHP->InitAvatarInfo(Avatar.nick, guildnamedummy, true);
		//UC_TotalHP->InitHP(0, GetMaximumTotalHP(), true);
	}
}

void UUP_InGameHUD_ArenaTemp::InitTargetHeroIcons()
{
	FBATTLE_USER match_target;
	GetMatchTargetInfo(match_target);
	//RGameInstance->ArenaManager->GetMatchTargetInfo(match_target);

	TArray<AActor*>& rEnemyList = GetEnemyUDList();
	for (int32 nIndex = 0; nIndex < EnemyIconHeroArenaArray.Num(); ++nIndex)
	{
		bool bUsing = false;
		if (rEnemyList.IsValidIndex(nIndex))
		{
			AActor* Spawner = rEnemyList[nIndex];
			if (IsValid(Spawner))
			{
				UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Spawner);
				if (IsValid(BattleEntityComponent))
				{
					EnemyIconHeroArenaArray[nIndex]->Init(Spawner, false);
					EnemyIconHeroArenaArray[nIndex]->InitHero();
					EnemyIconHeroArenaArray[nIndex]->PlayAni(TEXT("Enemy"));
					bUsing = true;
				}
			}
		}
		if (bUsing == false)
			EnemyIconHeroArenaArray[nIndex]->SetVisibility(ESlateVisibility::Hidden);
	}
			
	if (UC_TotalHP)
	{
		FString guildnamedummy = TEXT("");
		UC_TotalHP->InitAvatarInfo(match_target.nick, guildnamedummy, false);
		//UC_TotalHP->InitHP(0, GetTargetMaximumTotalHP(), false);
	}
}

void UUP_InGameHUD_ArenaTemp::PrepareWidget()
{
	FindChildWidgetsWithCertainName<UUC_HUD_Icon_Hero_Arena>(this, MyIconHeroArenaArray,TEXT("UC_HUD_Icon_MyHero_"));
	FindChildWidgetsWithCertainName<UUC_HUD_Icon_Hero_Arena>(this, EnemyIconHeroArenaArray, TEXT("UC_HUD_Icon_EnemyHero_"));
	InitMyHeroIcons();
	InitTargetHeroIcons();
	UC_TotalHP->SetAvatarInfo(true);
	UC_TotalHP->SetAvatarInfo(false);
	UC_HUD_PVP_Kill->SetVisibility(ESlateVisibility::Collapsed);
}

void UUP_InGameHUD_ArenaTemp::UpdateMyTotalHP(int32 curr_hp, int32 max_hp, float percent)
{
// 	if (UC_TotalHP)
// 	{
// 		UC_TotalHP->ChangeHP(curr_hp, max_hp, true);
// 	}
// 
// 	for (auto hero_icon : MyIconHeroArenaArray)
// 	{
// 		hero_icon->UpdateHeroHP();
// 	}
}

void UUP_InGameHUD_ArenaTemp::UpdateEnemyTotalHP(int32 curr_hp, int32 max_hp, float percent)
{
// 	if (UC_TotalHP)
// 	{
// 		UC_TotalHP->ChangeHP(curr_hp, max_hp, false);
// 	}
// 
// 	for (auto hero_icon : EnemyIconHeroArenaArray)
// 	{
// 		hero_icon->UpdateHeroHP();
// 	}
}

void UUP_InGameHUD_ArenaTemp::SetToggleCameraIcon(int32 Index)
{
	if (IsValid(UC_HUD_ToggleCamera))
	{
		UC_HUD_ToggleCamera->SetActiveIconIndex(Index);
	}

}

void UUP_InGameHUD_ArenaTemp::OnClickCameraMode(int32 Index)
{
	CameraMode = (EArenaCameraMode)Index;

	switch (CameraMode)
	{
	case EArenaCameraMode::VE_Sequence_1:
	{	
		PlaySequence(SequenceCamNames[0]);
	}
	break;
	case EArenaCameraMode::VE_Sequence_2:
	{		
		PlaySequence(SequenceCamNames[1]);
	}
	break;
	default:
	{				
		ChangeDefaultCamera();
	}
	break;
	}
	
}

void UUP_InGameHUD_ArenaTemp::ResetCameraMode()
{
	ABasePlayerController* controller = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
	if (controller)
	{
		ABaseCameraActor* camera = controller->GetCameraActor();
		if (camera)
		{
			if (false == camera->IsDynamicCamera)
			{
				float dist = camera->GetCamLongDist();
				camera->ZoomInAndOut(dist, false);
				camera->SetDynamicCamera(true);
				camera->ChangeCameraOriginDistance(true, false);
			}
		}
	}
}

void UUP_InGameHUD_ArenaTemp::SetArenaCamera()
{
// 	ABasePlayerController* controller = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
// 	if (controller)
// 	{
// 		ABaseCameraActor* camera = controller->GetCameraActor();
// 		if (camera)
// 			camera->SetArenaCamera(true);
// 	}
}

bool UUP_InGameHUD_ArenaTemp::PlaySequence(FName SequenceID)
{
	if( SequenceID.IsNone())
		return false;

	if( !IsValid(RGameInstance))
		return false;
	
	if( !IsValid( RGameInstance->SequenceManager) )
		return false;
	
	if(RGameInstance->SequenceManager->IsPlaying(SequenceID) )
		return false;

	StopSequence();
	
	ABaseSequenceActor* SequenceActor = RGameInstance->SequenceManager->SpawnSequence(SequenceID,true,true);
	if (SequenceActor)
	{
		return true;
	}

	return false;
}

void UUP_InGameHUD_ArenaTemp::StopSequence()
{
// 	if (!IsValid(RGameInstance))
// 		return;

	if (!IsValid(RGameInstance->SequenceManager))
		return;

	ABaseSequenceActor* SequenceActor = nullptr;

	for (FName camName : SequenceCamNames)
	{
		SequenceActor = RGameInstance->SequenceManager->FindSequence(camName);
		if (SequenceActor)
		{
			SequenceActor->Stop();
		}
	}
}

void UUP_InGameHUD_ArenaTemp::ChangeDefaultCamera()
{
	StopSequence();
	UUtilFunctionLibrary::ViewTargetBaseCamera(GetWorld());
}



void UUP_InGameHUD_ArenaTemp::IsPrevProcess()
{
	if (!IsValid(UC_HUD_Option))
		return;

	UC_HUD_Option->IsPrevProcess();
}