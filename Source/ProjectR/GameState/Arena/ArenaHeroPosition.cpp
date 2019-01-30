// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ArenaHeroPosition.h"

#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Modes/ArenaGameMode.h"
#include "ContentGameMode/Modes/MirrorGameMode.h"
#include "Global/ArenaManager.h"
#include "Global/MirrorDungeonManager.h"


// Sets default values
AArenaHeroPosition::AArenaHeroPosition()
{
}

void AArenaHeroPosition::SaveHeroPosition()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		FVector pos = GetActorLocation();
		FRotator rot = GetActorRotation();
		
		AProjectRGameMode* GameMode = UUtilFunctionLibrary::GetGamePlayMode();
		if (!GameMode)
			return;
// 		EGamePlayModeTypeEnum eModeType = GameMode->GetGamePlayModeState();
		if(Cast<AArenaRGameMode>(GameMode))
		{
			if (IsValid(RGameInstance->ArenaManager))
			{
				RGameInstance->ArenaManager->SetArenaHeroPosition(MyHeroPosition, PositionIndex, pos, rot);

				//UE_LOG(LogTemp, Warning, TEXT("BP_HeroPos : My(%d) Idx(%d) Pos(%.02f, %.02f, %.02f) Rot(%.02f, %.02f, %.02f)"),
				//	MyHeroPosition, PositionIndex, pos.X, pos.Y, pos.Z, rot.Yaw, rot.Roll, rot.Pitch);
			}
		}
		else if(Cast<AMirrorGameMode>(GameMode))
		{
			if (IsValid(RGameInstance->MirrorDungeonManager))
			{
				RGameInstance->MirrorDungeonManager->SetMirrorHeroPosition(MyHeroPosition, PositionIndex, pos, rot);
			}
		}
	}
}