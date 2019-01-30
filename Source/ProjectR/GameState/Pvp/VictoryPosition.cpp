// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "VictoryPosition.h"
#include "GlobalIntegrated.h"

#include "GameInfo/PlayStateInfo.h"
#include "CustomStructures/HeroSlotInfo.h"
#include "Table/CharacterTableInfo.h"
#include "UtilFunctionIntegrated.h"

#include "ContentGameMode/Modes/StageGameMode.h"
#include "ContentGameMode/Modes/ArenaGameMode.h"
#include "ContentGameMode/Modes/MirrorGameMode.h"

#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"

// Sets default values
AVictoryPosition::AVictoryPosition() {}

void AVictoryPosition::SetActorVictoryPosition()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	AProjectRGameMode*	pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
	if (Cast<AStageGameMode>(pGamePlayMode))
		SetActorVictoryPosition_Campain();
	if (Cast<AArenaRGameMode>(pGamePlayMode))
		SetActorVictoryPosition_Arena();
	if (Cast<AMirrorGameMode>(pGamePlayMode))
		SetActorVictoryPosition_MirrorDungeon();
}

void AVictoryPosition::SetActorVictoryPosition_Campain()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray< AActor* >	HideIgnoreActors;

// 	URCampaignHUD_ThreeControl* ThreeGameHUD = Cast<URCampaignHUD_ThreeControl>(UUIFunctionLibrary::GetInGameHUD());
// 	if (ThreeGameHUD)
// 		ThreeGameHUD->UC_CombineBar->EndProcess(false);
	RGameInstance->SequenceManager->ClearSequence();

	int32 PositionIndex = 0;
	const TArray<FHeroSlotInfo>& HeroSlotArray = RGameInstance->PlayStateInfo->GetSelectedCharacterArray();
	for (auto HeroSlotInfo : HeroSlotArray)
	{
		if (IsValid(HeroSlotInfo.SpawnedTargetActor))
		{
			if (HeroSlotInfo.CharacterRole == ECharacterRoleEnum::VE_Leader)
			{
				USkeletalMeshComponent* MeshComp = HeroSlotInfo.SpawnedTargetActor->FindComponentByClass<USkeletalMeshComponent>();
				int32 MatNum = MeshComp->GetNumMaterials();
				for (size_t i = 0; i < MatNum; ++i)
				{
					UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(MeshComp->GetMaterial(i));
					if (nullptr != MID)
					{
						MID->SetVectorParameterValue(FName(TEXT("CustomLightColor")), FLinearColor(0.f, 0.f, 0.f));
						MeshComp->SetMaterial(i, MID);
					}
				}
			}
			

			if (HeroSlotInfo.SlotSpawnState == EHeroSpawnStateEnum::VE_Spawned)
			{
				UEntityAIComponent* AiComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(HeroSlotInfo.SpawnedTargetActor);
				if (AiComp)
					AiComp->AIStop();

				UAnimStateComponent* AnimComp = UCharacterHelpFunctionLibrary::GetAnimStateComponent(HeroSlotInfo.SpawnedTargetActor);
				if (AnimComp)
				{
					AnimComp->SetAnimComponentState(EAnimStateEnum::VE_Idle);
				}
				

				if (PositionCompList.IsValidIndex(PositionIndex))
				{
					if (IsValid(PositionCompList[PositionIndex]))
					{
						FVector PositionLocation = PositionCompList[PositionIndex]->GetComponentLocation();
						FRotator PositionRotation = PositionCompList[PositionIndex]->GetComponentRotation();

						FVector ResultLocation;
						if (UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(RGameInstance, PositionLocation, ResultLocation))
						{
							UCapsuleComponent* ActorCapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(HeroSlotInfo.SpawnedTargetActor);
							if (ActorCapsuleComponent)
							{
								ResultLocation.Z += ActorCapsuleComponent->GetScaledCapsuleHalfHeight();
							}
						}
						else
						{
							ResultLocation = PositionLocation;
						}

						HeroSlotInfo.SpawnedTargetActor->SetActorLocation(ResultLocation, false, nullptr, ETeleportType::TeleportPhysics);
						HeroSlotInfo.SpawnedTargetActor->SetActorRotation(PositionRotation);

						HideIgnoreActors.Emplace(HeroSlotInfo.SpawnedTargetActor);

						++PositionIndex;
					}
				}
			}
		}
	}


	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.SetAllObjectVisible(false, HideIgnoreActors);
	}
}

void AVictoryPosition::SetActorVictoryPosition_Arena()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray< AActor* >	HideIgnoreActors;

	int32 PositionIndex = 0;
	
	const TArray<AActor*>& HeroSlotArray = RGameInstance->ArenaManager->mMyHeroList;
	for (auto HeroSlotInfo : HeroSlotArray)
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(HeroSlotInfo);
		if (IsValid(BattleEntityComponent) && BattleEntityComponent->IsDead() == false )
		{
			if (PositionCompList.IsValidIndex(PositionIndex))
			{
				if (IsValid(PositionCompList[PositionIndex]))
				{
					FVector PositionLocation = PositionCompList[PositionIndex]->GetComponentLocation();
					FRotator PositionRotation = PositionCompList[PositionIndex]->GetComponentRotation();

					FVector ResultLocation;
					if (UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(RGameInstance, PositionLocation, ResultLocation))
					{
						UCapsuleComponent* ActorCapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(HeroSlotInfo);
						if (ActorCapsuleComponent)
						{
							ResultLocation.Z += ActorCapsuleComponent->GetScaledCapsuleHalfHeight();
						}
					}
					else
					{
						ResultLocation = PositionLocation;
					}

					HeroSlotInfo->SetActorLocation(ResultLocation, false, nullptr, ETeleportType::TeleportPhysics);
					HeroSlotInfo->SetActorRotation(PositionRotation);

					HideIgnoreActors.Emplace(HeroSlotInfo);

					++PositionIndex;
				}
			}
		}
	}

// 	UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
// 	if (pGamePlayMode)
// 	{
// 		pGamePlayMode->SetAllObjectVisible(false, HideIgnoreActors);
// 	}
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.SetAllObjectVisible(false, HideIgnoreActors);
	}
}

void AVictoryPosition::SetActorVictoryPosition_MirrorDungeon()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray< AActor* >	HideIgnoreActors;

	int32 PositionIndex = 0;

	const TArray<AActor*>& HeroSlotArray = RGameInstance->MirrorDungeonManager->mMyHeroList;
	for (auto HeroSlotInfo : HeroSlotArray)
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(HeroSlotInfo);
		if (IsValid(BattleEntityComponent) && BattleEntityComponent->IsDead() == false)
		{
			if (PositionCompList.IsValidIndex(PositionIndex))
			{
				if (IsValid(PositionCompList[PositionIndex]))
				{
					FVector PositionLocation = PositionCompList[PositionIndex]->GetComponentLocation();
					FRotator PositionRotation = PositionCompList[PositionIndex]->GetComponentRotation();

					FVector ResultLocation;
					if (UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(RGameInstance, PositionLocation, ResultLocation))
					{
						UCapsuleComponent* ActorCapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(HeroSlotInfo);
						if (ActorCapsuleComponent)
						{
							ResultLocation.Z += ActorCapsuleComponent->GetScaledCapsuleHalfHeight();
						}
					}
					else
					{
						ResultLocation = PositionLocation;
					}

					HeroSlotInfo->SetActorLocation(ResultLocation, false, nullptr, ETeleportType::TeleportPhysics);
					HeroSlotInfo->SetActorRotation(PositionRotation);

					HideIgnoreActors.Emplace(HeroSlotInfo);

					++PositionIndex;
				}
			}
		}
	}


	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.SetAllObjectVisible(false, HideIgnoreActors);

	}
}
