// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RaidPlayerController.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"


#include "UI/UI_Battle/InGameHeroUI/HeroUIforBase.h"

ARaidPlayerController::ARaidPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super( ObjectInitializer )
{
}

void ARaidPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(RGAMEINSTANCE(this)->EventManager))
	{
		//RGAMEINSTANCE(this)->EventManager->OnCrewDead.Clear();
		//RGAMEINSTANCE(this)->EventManager->OnCrewDead.AddDynamic(this, &ARaidPlayerController::OnRTSCrewDead);
	}
}

void ARaidPlayerController::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(LeaderControlHelper))
	{
		LeaderControlHelper->Tick(DeltaTime);
	}
}

void ARaidPlayerController::OnRTSCrewDead(int32 InSlotIndex)
{
// 	FHeroSlotInfo& DeadSlot = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterSlot(InSlotIndex);
// 
// 	if (DeadSlot.SlotSpawnState == EHeroSpawnStateEnum::VE_None)
// 		return;
// 
// 	for (FHeroSlotInfo& slot : RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray())
// 	{
// 		if (slot.CharacterRole != DeadSlot.CharacterRole)
// 			continue;
// 		if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Rest)
// 		{
// 			if (slot.SlotSpawnState != EHeroSpawnStateEnum::VE_Dead)
// 			{
// 
// 				if (LeaderControlHelper != nullptr)
// 				{
// 					auto LeaderControl = LeaderControlHelper->GetLeaderSkillControl();
// 					if (LeaderControl)
// 					{
// 						//if three hero control mode in RTS
// 						if (URTSManager::GetInstancePtr()->IsConnectedMode((int32)EPVP::SR) ||
// 							URTSManager::GetInstancePtr()->IsConnectedMode((int32)EPVP::CS))
// 						{
// 
// 							if (DeadSlot.CharacterRole == ECharacterRoleEnum::VE_Crew1)
// 								RGAMEINSTANCE(GEngine)->RealTimeModManager->ChangeCrew(ESkillOrigin::VE_Crew1, DeadSlot.SpawnedTargetActor->GetActorLocation(), DeadSlot.SpawnedTargetActor->GetActorForwardVector());
// 							else if (DeadSlot.CharacterRole == ECharacterRoleEnum::VE_Crew2)
// 								RGAMEINSTANCE(GEngine)->RealTimeModManager->ChangeCrew(ESkillOrigin::VE_Crew2, DeadSlot.SpawnedTargetActor->GetActorLocation(), DeadSlot.SpawnedTargetActor->GetActorForwardVector());
// 
// 						}
// 						else // normal
// 							LeaderControl->CrewChangeEffect.ChangeCrew(DeadSlot.CharacterRole);
// 					}
// 
// 				}
// 
// 				if (DPadSkillInterface.IsValid() == false)
// 					DPadSkillInterface = UUIFunctionLibrary::GetSkillSet(this);
// 				if (DPadSkillInterface.IsValid())
// 				{
// 					//InGameHUD Crew Position Change when crew1 is dead
// 					if (InSlotIndex != ROLE_LEADER_INDEX && InSlotIndex < ROLE_CREW2_BATTLE_INDEX)
// 					{
// 						if (IsValid(InGameHUD))
// 						{
// 							InGameHUD->PlayAni(TEXT("Crew1Change"));
// 
// 							URCampaignHUD_ThreeControl* ThreeGameHUD = Cast<URCampaignHUD_ThreeControl>(InGameHUD);
// 							if (ThreeGameHUD)
// 							{
// 								for (size_t i = 0; i < ThreeGameHUD->HeroIconArray.Num(); ++i)
// 								{
// 									if (ThreeGameHUD->HeroIconArray[i]->GetHeroSlotInfo().CharacterRole == ECharacterRoleEnum::VE_Crew1)
// 										ThreeGameHUD->HeroIconArray[i]->PlayAni(TEXT("CrewChange"));
// 								}
// 							}
// 						}
// 					}
// 					else
// 					{
// 						if (IsValid(InGameHUD))
// 						{
// 							InGameHUD->PlayAni(TEXT("Crew2Change"));
// 
// 							URCampaignHUD_ThreeControl* ThreeGameHUD = Cast<URCampaignHUD_ThreeControl>(InGameHUD);
// 							if (ThreeGameHUD)
// 							{
// 								for (size_t i = 0; i < ThreeGameHUD->HeroIconArray.Num(); ++i)
// 								{
// 									if (ThreeGameHUD->HeroIconArray[i]->GetHeroSlotInfo().CharacterRole == ECharacterRoleEnum::VE_Crew2)
// 										ThreeGameHUD->HeroIconArray[i]->PlayAni(TEXT("CrewChange"));
// 								}
// 							}
// 						}
// 					}
// 
// 					if (DPadSkillInterface.IsValid())
// 						DPadSkillInterface->SetChangeCrewButtonEnable(DeadSlot.CharacterRole);
// 				}
// 
// 				break;
// 			}
// 		}
// 	}
// 
// 	if (DPadSkillInterface.IsValid())
// 		DPadSkillInterface->ChangeCrewActiveIcon();
}
