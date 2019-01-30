// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_InGameHUD_Arena.h"
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
#include "Global/RGameInstance.h"
#include "GameInfo/GuildInfo.h"


void UUP_InGameHUD_Arena::NativeConstruct()
{
	Super::NativeConstruct();

	SequenceCamNames.Reset();

	SequenceCamNames.Emplace(FName(TEXT("Arena_Cam01_Loop")));
	SequenceCamNames.Emplace(FName(TEXT("Arena_Cam02_Loop")));
}

void UUP_InGameHUD_Arena::Init()
{
	CameraMode = EArenaCameraMode::VE_Default;

	RGameInstance = RGAMEINSTANCE(this);
	if(RGameInstance)
	{
		
		CurrentGamePlayMode = UUtilFunctionLibrary::GetGamePlayModeEnum();

		if (RGameInstance->EventManager)
		{
			if(RGameInstance->EventManager->OnClickToggleCameraIcon.IsAlreadyBound( this, &UUP_InGameHUD_Arena::OnClickCameraMode ) )
				RGameInstance->EventManager->OnClickToggleCameraIcon.RemoveDynamic(this, &UUP_InGameHUD_Arena::OnClickCameraMode);

			RGameInstance->EventManager->OnClickToggleCameraIcon.AddDynamic(this, &UUP_InGameHUD_Arena::OnClickCameraMode);
		}
		PrepareWidget();
	}
}

TArray<AActor*>& UUP_InGameHUD_Arena::GetUDList()
{
	if(IsValid(RGameInstance->ArenaManager))
		return RGameInstance->ArenaManager->mMyHeroList;

	static TArray<AActor*> EmptyArray;
	return EmptyArray;
}

TArray<AActor*>& UUP_InGameHUD_Arena::GetEnemyUDList()
{
	if (IsValid(RGameInstance->ArenaManager))
		return RGameInstance->ArenaManager->mEnemyList;

	static TArray<AActor*> EmptyArray;
	return EmptyArray;
}

int32 UUP_InGameHUD_Arena::GetMaximumTotalHP()
{
	if (IsValid(RGameInstance->ArenaManager))
		return RGameInstance->ArenaManager->GetMyMaximumTotalHP();

	return 0;
}

int32 UUP_InGameHUD_Arena::GetTargetMaximumTotalHP()
{
	if (IsValid(RGameInstance->ArenaManager))
		return RGameInstance->ArenaManager->GetTargetMaximumTotalHP();

	return 0;
}

void UUP_InGameHUD_Arena::GetMatchTargetInfo(FBATTLE_USER& target_info)
{
	if (IsValid(RGameInstance->ArenaManager))
		RGameInstance->ArenaManager->GetMatchTargetInfo(target_info);
}

void UUP_InGameHUD_Arena::InitMyHeroIcons()
{
	for (int32 nIndex = 0; nIndex < MyIconHeroArenaArray.Num(); ++nIndex)
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
		FString guildName;
		if (RGameInstance->GuildInfo->MyGuildExist)
			guildName = RGameInstance->GuildInfo->MyGuildInfo.name;
		else
			guildName = TEXT("");
		UC_TotalHP->InitAvatarInfo(Avatar.nick, guildName, false);
		//UC_TotalHP->InitHP(0, GetMaximumTotalHP(), false);
	}
}

void UUP_InGameHUD_Arena::InitTargetHeroIcons()
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
					EnemyIconHeroArenaArray[nIndex]->SetVisibility(ESlateVisibility::Visible);
					EnemyIconHeroArenaArray[nIndex]->Init(Spawner, false);
					EnemyIconHeroArenaArray[nIndex]->InitHero();
					EnemyIconHeroArenaArray[nIndex]->PlayAni(TEXT("EnemyAni"));
					EnemyIconHeroArenaArray[nIndex]->PlayChildIconAni(TEXT("Enemy"));
					bUsing = true;
				}
			}
		}
		if (bUsing == false)
			EnemyIconHeroArenaArray[nIndex]->SetVisibility(ESlateVisibility::Hidden);
	}
			
	if (UC_TotalHP)
	{
		FString guildName;
		guildName = match_target.guild;
		UC_TotalHP->InitAvatarInfo(match_target.nick, guildName, true);
		//UC_TotalHP->InitHP(0, GetTargetMaximumTotalHP(), true);
	}
}

void UUP_InGameHUD_Arena::PrepareWidget()
{
	FindChildWidgetsWithCertainName<UUC_HUD_Icon_Hero_Arena>(this, MyIconHeroArenaArray,TEXT("UC_HUD_Icon_MyHero_"));
	FindChildWidgetsWithCertainName<UUC_HUD_Icon_Hero_Arena>(this, EnemyIconHeroArenaArray, TEXT("UC_HUD_Icon_EnemyHero_"));
	InitMyHeroIcons();
	InitTargetHeroIcons();
	UC_TotalHP->SetAvatarInfo(true);
	UC_TotalHP->SetAvatarInfo(false);
	if(IsValid(UC_HUD_PVP_Kill))
		UC_HUD_PVP_Kill->SetVisibility(ESlateVisibility::Collapsed);
}

void UUP_InGameHUD_Arena::UpdateMyTotalHP(int32 curr_hp, int32 max_hp, float percent)
{
// 	if (UC_TotalHP)
// 	{
// 		UC_TotalHP->ChangeHP(curr_hp, max_hp, false);
// 	}
// 
// 	for (auto hero_icon : MyIconHeroArenaArray)
// 	{
// 		hero_icon->UpdateHeroHP();
// 	}
}

void UUP_InGameHUD_Arena::UpdateEnemyTotalHP(int32 curr_hp, int32 max_hp, float percent)
{
// 	if (UC_TotalHP)
// 	{
// 		UC_TotalHP->ChangeHP(curr_hp, max_hp, true);
// 	}
// 
// 	for (auto hero_icon : EnemyIconHeroArenaArray)
// 	{
// 		hero_icon->UpdateHeroHP();
// 	}
}

void UUP_InGameHUD_Arena::SetToggleCameraIcon(int32 Index)
{
	if (IsValid(UC_HUD_ToggleCamera))
	{
		UC_HUD_ToggleCamera->SetActiveIconIndex(Index);
	}

}

void UUP_InGameHUD_Arena::OnClickCameraMode(int32 Index)
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

void UUP_InGameHUD_Arena::ResetCameraMode()
{
	ABasePlayerController* controller = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
	if (controller)
	{
		ABaseCameraActor* camera = controller->GetCameraActor();
		if (camera)
		{
// 			if (false == camera->IsDynamicCamera)
			{
				float dist = camera->GetCamLongDist();
				camera->ZoomInAndOut(dist, false);
				camera->SetDynamicCamera(true);
				camera->ChangeCameraOriginDistance(true, false);
			}
		}
	}
}

void UUP_InGameHUD_Arena::SetArenaCamera()
{
// 	ABasePlayerController* controller = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
// 	if (controller)
// 	{
// 		ABaseCameraActor* camera = controller->GetCameraActor();
// 		if (camera)
// 		{
// 			camera->SetArenaCamera(true);
// 		}
// 
// 	}
}

bool UUP_InGameHUD_Arena::PlaySequence(FName SequenceID)
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

void UUP_InGameHUD_Arena::StopSequence()
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

void UUP_InGameHUD_Arena::ChangeDefaultCamera()
{
	StopSequence();
	UUtilFunctionLibrary::ViewTargetBaseCamera(GetWorld());
}



void UUP_InGameHUD_Arena::IsPrevProcess()
{
	if (!IsValid(UC_HUD_Option))
		return;

	UC_HUD_Option->IsPrevProcess();
}