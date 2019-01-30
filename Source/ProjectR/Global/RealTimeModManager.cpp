// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "RealTimeModManager.h"
#include "GlobalIntegrated.h"
#include "ContentIntergrated.h"
#include "GameInfo/UserInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "GameInfo/PlayStateInfo.h"
#include "GameInfo/RInventory.h"
#include "Skill_V2/NetSkillManager.h"
#include "UI/GuildRaid/UC_GuildRaid_Group.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/RWidgetManager.h"
#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/RemoteMoveComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Network/HttpRequestClient.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/RTSManager.h"
#include "Network/Chat/ChatManager.h"
#include "ContentGameMode/Modes/RaidGameMode.h"
#include "UtilFunctionIntegrated.h"
#include "UI/InGameHUD/SkillIcon_New.h"

#include "UI/UI_Battle/RCampaignHUD_RaidThreeControl.h"

#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/InGameHeroUI/HeroUIforBase.h"
#include "DirectionTrackmanager.h"

#include "RSkillSet_New.h"

int16 msec2sec(int16 n) 
{
	unsigned q, r, t;
	n = n + 500;
	t = (n >> 7) + (n >> 8) + (n >> 12);
	q = (n >> 1) + t + (n >> 15) + (t >> 11) + (t >> 14);
	q = q >> 9;
	r = n - q * 1000;
	return q + ((r + 24) >> 10);
}



bool URealTimeModManager::IsRealTimeGameMode()
{
	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BF) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
		return true;
	return false;
}

bool URealTimeModManager::IsControlWithRTS()
{
	if (IsRealTimeGameMode())
	{
		auto GameInstance = RGAMEINSTANCE(GEngine);
		if (GameInstance->RealTimeModManager == nullptr)
			return false;
		return GameInstance->RealTimeModManager->GetIsControllable();
	}
	return false;
}

void URealTimeModManager::Initialize()
{
	ClearRealTimeModData();
	GameInstance = RGAMEINSTANCE(this);
}

void URealTimeModManager::ClearRealTimeModData()
{
	MoveTickElapsedTime = 0.0f;
	RealModeClearTime = 0.0f;
	TimeHandler_DeckSetting.Invalidate();
	TimeHandler_RemainGame.Invalidate();
	TimeHandler_RemainTimeNotify.Invalidate();
	ClearData();
}
 
void URealTimeModManager::Tick(float DeltaTime)
{
	URTSManager::GetInstancePtr()->FrameMove(DeltaTime);
	if (GameInstance && GameInstance->NetSkillManager)
		GameInstance->NetSkillManager->Update(DeltaTime);

	if(0 < GameTimeSec)
	{
		const FTimespan CurrentTime = FTimespan::FromSeconds(FPlatformTime::Seconds());

		if (CurrentTime.GetTicks() - nCheckTimeSeconds >= FTimespan::FromSeconds(1.0f).GetTicks())
		{
			nCheckTimeSeconds = CurrentTime.GetTicks();
			GameTimeSec--;
		}
	}
}

bool URealTimeModManager::IsTickable() const
{
	return true;
}
TStatId URealTimeModManager::GetStatId() const
{
	return Super::GetStatID();
}

bool URealTimeModManager::IsTickableWhenPaused() const
{
	return true;
}

float URealTimeModManager::GetGameTime() const
{
	return GameTimeSec;
}

void URealTimeModManager::SYNC_TIME_REMAIN()
{
	float tempTime = GameTimeSec - 1;
	if (tempTime >= 0)
	{
		GameTimeSec = tempTime;
		if (OnReceiveRemainTime.IsBound())
			OnReceiveRemainTime.Broadcast(GameTimeSec);
	}
}

void URealTimeModManager::OnACK_BT_SYNC_TIME_REMAIN(float InTimeSec, bool AddNotify)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] SetTimerRemainGame >> TimeSec %f Notify %d"), InTimeSec, AddNotify);
	GameTimeSec = InTimeSec;

	if (InTimeSec == 0)
		GameInstance->CampaignFailReson = EStageFailReason::VE_TIMEOUT_FAIL;
// 	GameInstance->TimerManager->ClearTimer(TimeHandler_RemainGame);
// 	GameInstance->TimerManager->SetTimer(TimeHandler_RemainGame, this, &URealTimeModManager::SYNC_TIME_REMAIN, 1.0f, true);
	if (AddNotify)
	{
		if (OnReceiveRemainTime.IsBound())
			OnReceiveRemainTime.Broadcast(InTimeSec);
	}
}

void URealTimeModManager::OnACK_CS_DECK_TIME_REMAIN(OVERLORD::RTS::S2C::ACK_CS_DECK_TIME_REMAINT* data)
{
	if (OnCSDeckSelectTime.IsBound())
		OnCSDeckSelectTime.Broadcast(data->remainSec);
}

void URealTimeModManager::Connect(class URGameInstance* instance, const FString& tcpServerHost, const FString& tcpServerPort)
{
	GameInstance = instance;
	mTCPTargetHost = tcpServerHost;
	mTCPTargetPort = tcpServerPort;
	URTSManager::GetInstancePtr()->Initialize(instance);
	URTSManager::GetInstancePtr()->SetConnectionParam(mTCPTargetHost, FCString::Atoi(*mTCPTargetPort));
	// URTSManager::GetInstancePtr()->Connect();
}

void URealTimeModManager::DisConnect()
{
	UUIFunctionLibrary::HideMatchingWaiting();
}

void URealTimeModManager::Update_RTS_Team(OVERLORD::RTS::STRUCT::TEAMT* InData)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] Update_RTS_Team >> team %d"), InData->team);

	FRTS_TEAM* FindInfo = Get_RTS_Team(InData->team);
	if (FindInfo == nullptr)
	{
		FRTS_TEAM newTeamInfo;
		newTeamInfo.SetData(InData);
		if (TeamList.Contains(newTeamInfo.team))
			TeamList.Emplace(newTeamInfo.team, newTeamInfo);
	}
	else
	{
		FindInfo->SetData(InData);
	}
}

FRTS_TEAM* URealTimeModManager::Get_RTS_Team(uint16 InTeamID)
{
	if (TeamList.Contains(InTeamID))
		return &TeamList[InTeamID];
	return nullptr;
}

void URealTimeModManager::Update_RTS_Player(OVERLORD::RTS::STRUCT::PLAYERT* InData)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] Update_RTS_Player >> KID %d"), InData->kId);

	FRTS_PLAYER* FindInfo = Get_RTS_Player(InData->team);
	if (FindInfo == nullptr)
	{
		FRTS_PLAYER newPlayerInfo;
		newPlayerInfo.SetData(InData);
		RTSPlayerList.Emplace(newPlayerInfo.kId, newPlayerInfo);
		FindInfo = &RTSPlayerList[newPlayerInfo.kId];
	}
	else
	{
		FindInfo->SetData(InData);
	}

	if (FindInfo)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if (RGameInstance->HttpClient->IsValidKID(FindInfo->kId))
		{
			UObjectFunctionLibrary::SetMyPartyID(FindInfo->party);
			UObjectFunctionLibrary::SetMyTeamKey(FindInfo->team);
		}
	}
	RGAMEINSTANCE(this)->ColosseumManager->OnUpdateColosseumTeamAvatar.Broadcast(InData->kId);
}

// bool URealTimeModManager::Delete_RTS_Actor(uint16 InActorUK)
// {
// 	int16 FindIndex = INDEX_NONE;
// 	if (ActorList.Contains(InActorUK))
// 	{
// 		ActorList.Remove(InActorUK);
// 		return true;
// 	}
// 	return false;
// }

bool URealTimeModManager::Get_RTS_Actor_UKLIstByKID(int16 InUserKID, TArray<int16>& HeroUKList)
{
	bool bResult = false;
	HeroUKList.Empty();

	for (auto V : ActorList)
	{
		if (InUserKID == V.Value.kId)
		{
			HeroUKList.Emplace(V.Value.actorUK);
			bResult = true;
		}
	}

	return bResult;
}

//void URealTimeModManager::NotifyMyHeroDeathHandler()
//{
//	GameInstance->GetTimerManager().ClearTimer(MyHeroDeathHandler);
//	GameInstance->EventManager->OnDisappear_RTS_Player.Broadcast(GameInstance->HttpClient->GetKID());
//}

FRTS_TEAM* URealTimeModManager::GetMyTeam()
{
	return Get_RTS_Team(UObjectFunctionLibrary::GetMyTeamKey());
}

FString URealTimeModManager::GetPlayerNameByKID(int16 InPlayerKID)
{
	FString Name;
	for (auto V : ActorList)
	{
		if (V.Value.kId == InPlayerKID)
			return V.Value.nick;
	}
	return Name;
}

bool URealTimeModManager::GetIsControllable() const
{
	if(	URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BF) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR) ||
		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR)  )
	{
		return bIsControlable;
	}
	return true;
}

void URealTimeModManager::OnACK_DISCONNECTED(uint8 reason)
{

}

void URealTimeModManager::OnACK_PLAY_SEND_RESULT(OVERLORD::RTS::S2C::ACK_PLAY_SEND_RESULTT* Packet)
{
	/*
	ToDo : End Effects..
	*/

	if (Packet->error->type == RTSCE::RTSCE_SUCCESS)
	{
		if (RTSInfo.modType == EPVP::MR || RTSInfo.modType == EPVP::SR)
		{
			if(Packet->result == 1)
				RGAMEINSTANCE(this)->IsSuccess =  true;
			else
				RGAMEINSTANCE(this)->IsSuccess = false;
			UPacketFunctionLibrary::TOWER_BOSS_PLAY_RESULT_RQ(this->mRoomKey);
			URTSManager::GetInstancePtr()->REQ_BT_EXIT();
		}
		else if (RTSInfo.modType == EPVP::CS)
		{
			UHUD_RaidThreeControl *RaidHUD = Cast<UHUD_RaidThreeControl>(UUIFunctionLibrary::GetInGameHUD());
			if (RaidHUD)
			{
				float RequiredMedalCount = RGAMEINSTANCE(this)->RealTimeModManager->RTSInfo.modId == FString::FromInt(1) ? 6 : 21;

				if (Packet->result == 1)
				{
					UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_RTS_CS_PLAYWIN);
					RaidHUD->OnColosseum_PointChanged.ExecuteIfBound(RequiredMedalCount, 0);
				}
				else
				{
					UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_RTS_CS_PLAYLOSE);
					RaidHUD->OnColosseum_PointChanged.ExecuteIfBound(0, RequiredMedalCount);
				}
			}
			//UPacketFunctionLibrary::COLOSSEUM_PLAY_RESULT_RQ(this->mRoomKey);
		}

		//URTSManager::GetInstancePtr()->REQ_BT_EXIT();
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PLAY_SEND_RESULT >> SUCCESS[%d]"), Packet->error->type);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PLAY_SEND_RESULT >> FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::REQ_BT_ACTOR_MOVE(AActor* PlayerActor, bool IsStop, float DeltaTime)
{
	if (false == IsValid(PlayerActor))
		return;
	IEntityBaseProperty* BaseProperty = Cast< IEntityBaseProperty >(PlayerActor);
	if (!BaseProperty)
		return;
	if (BaseProperty->GetGroupKey() != UObjectFunctionLibrary::GetMyTeamKey())
		return;
	if (BaseProperty->GetPartyID() != UObjectFunctionLibrary::GetMyPartyID())
		return;
	if (GetIsControllable() == false)
		return;
	int16 ActorUK = BaseProperty->GetPlayerUK();
	FVector ActorLocation = PlayerActor->GetActorLocation();
	FVector ActorDirection = PlayerActor->GetActorForwardVector();
	if (IsStop == false)
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(PlayerActor);
		if (false == IsValid(BattleEntityComponent))
			return;
		UMapFunctionLibrary::AdjustActorLocation(PlayerActor, ActorLocation);
		UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
		if (BattleInfo)
		{
			float MoveSpeed = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_MoveSpeed);
			FVector Velocity = ActorDirection * MoveSpeed * DeltaTime;
			ActorLocation = ActorLocation + Velocity;
		}
	}
 	//UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] REQ_BT_ACTOR_MOVE >> ActorUK[%d] IsStop[%d] location [%f, %f, %f] direction [%f, %f, %f]"), 
 	//	ActorUK, IsStop, ActorLocation.X, ActorLocation.Y, ActorLocation.Z, ActorDirection.X, ActorDirection.Y, ActorDirection.Z);
	URTSManager::GetInstancePtr()->REQ_BT_ACTOR_MOVE(ActorUK, IsStop, ActorLocation, ActorDirection);
}

void URealTimeModManager::OnACK_BT_ATTACK_NOW(OVERLORD::RTS::S2C::ACK_BT_ATTACK_NOWT* data)
{
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!pGameMode)
		return;
	TWeakObjectPtr<AActor> AttackActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(data->actorUK);
	if (!AttackActor.IsValid())
		return;
	if (false == UUtilFunctionLibrary::IsMyPlayCharacter(AttackActor.Get()))
	{
		UBattleEntityComponent* battle_comp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(AttackActor.Get());
		if (battle_comp)
		{
			TWeakObjectPtr<AActor> TargetActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(data->targetUK);
			battle_comp->RecvRTS_AttackNow(TargetActor.Get(), data->bAttack);
			if (AttackActor.IsValid() && TargetActor.IsValid())
			{
				FVector DirectionVector = TargetActor->GetActorLocation() - AttackActor->GetActorLocation();
				DirectionVector.Z = 0;
				FRotator lookatRotation = FRotationMatrix::MakeFromX(DirectionVector).Rotator();
				AttackActor->SetActorRotation(lookatRotation);
			}
		}
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_ATTACK_NOW >> actorUK %d targetUK %d bAttack %d"), data->actorUK, data->targetUK, data->bAttack);
	}
	else
	{
		UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(AttackActor.Get());
		if (EntityAIComponent)
		{
			UBattleEntityComponent* battle_comp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(AttackActor.Get());
			if (battle_comp)
			{
				TWeakObjectPtr<AActor> TargetActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(data->targetUK);
				battle_comp->RecvRTS_AttackNow(TargetActor.Get(), data->bAttack);

				/*FString str = TEXT("Receive Packet AttackNow Sequence : ") + FString::FromInt(data->sequence);
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, str);*/
			}
		}
	}
}

void URealTimeModManager::OnACK_BT_ATTACK_NORMAL(OVERLORD::RTS::S2C::ACK_BT_ATTACK_NORMALT* data)
{
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!pGameMode)
		return;

	TWeakObjectPtr<AActor> AttackActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(data->actorUK);
	if (!AttackActor.IsValid())
		return;
	
	UBattleEntityComponent* battle_comp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(AttackActor.Get());
	if (battle_comp)
	{
		TWeakObjectPtr<AActor> TargetActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(data->targetUpdate->actorUK);

		battle_comp->RecvRTS_NormalAttack(TargetActor.Get(), (EBattleDamageResultEnum)data->targetUpdate->type, data->targetUpdate->damage, data->targetUpdate->curHp , 0);
		// battle_comp->RecvRTS_NormalAttackNow(TargetActor.Get(), true);
	}

	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_ATTACK_NORMAL >> actorUK %d targetUK %d damage %d curHp %d"), data->actorUK, data->targetUpdate->actorUK, data->targetUpdate->damage, data->targetUpdate->curHp);
}

void URealTimeModManager::OnACK_BT_CHANGE_CREW(OVERLORD::RTS::S2C::ACK_BT_CHANGE_CREWT* Packet)
{
	if (Packet->error->type == RTSCE::RTSCE_SUCCESS)
	{
		for (auto& updateData : Packet->updates)
		{
			OVERLORD::RTS::STRUCT::ACTOR_UPDATET* _actor_update = updateData.get();
			TWeakObjectPtr<AActor> Actor = RaidGameMode->ObjectHelper.GetObjectByPlayerUK(_actor_update->actorUK);
			if (!Actor.IsValid())
			{
				//Update_RTS_Actor(_actor_update->);
				FRTS_ACTOR_UPDATE crewInfo;
				crewInfo.SetData(_actor_update);
				RaidGameMode->OnACK_BT_CHANGE_CREW(&crewInfo);
				Actor = RaidGameMode->ObjectHelper.GetObjectByPlayerUK(crewInfo.actorUK);
				IEntityBaseProperty* BaseProperty = Cast< IEntityBaseProperty >(Actor.Get());
				if (BaseProperty && RGAMEINSTANCE(GEngine)->HttpClient->IsValidKID(BaseProperty->GetUserKID())) {
					if (CrewChangeProcess != ESkillOrigin::VE_Leader && RaidGameMode != nullptr)
					{
						//CamSettings
						//AStagePlayerController* PlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RaidGameMode->GetWorld()));
						//if (PlayerController)
						//{
						//	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(this)->GetWorld(), 0);
						//	ABaseCameraActor* BaseCam = Cast<ABaseCameraActor>(PCM->GetViewTarget());
						//	if (IsValid(BaseCam))
						//	{
						//		BaseCam->MoveStageCamera(Actor->GetActorLocation(), 0.f);
						//		BaseCam->PingPongCameraForward(Actor->GetActorLocation(), 1.f);

						//		BaseCam->AutoZoomInOutCameraByPercent(
						//			true,//bStartZoomIn, 
						//			0.5f,//ZoomPercent, 
						//			0.15f,//StartTime, 
						//			0.0f,//DurationTime, 
						//			0.5f,//EndTime, 
						//			nullptr, nullptr);
						//	}
						//}

						auto SkillEntComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Actor.Get());
						auto BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Actor.Get());

						if (IsValid(SkillEntComp) && IsValid(BattleEntComp))
						{
							UISkill_V2* TempSkill = SkillEntComp->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill02);
							if (IsValid(TempSkill))
							{
								AActor* CurTarget = nullptr;
								if (TempSkill != nullptr && TempSkill->GetSkillInfo()->TargetType != ESkillTargetTypeEnum::VE_Target)
									CurTarget = Actor.Get();
								else
									CurTarget = BattleEntComp->GetCurrentAttackTarget();

								if (!IsValid(CurTarget))
								{
									UCharacterBattleInfo* BattleInfo = BattleEntComp->GetBattleInfo();
									float AttackRange = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);

									BattleEntComp->SetCurrentAttackTarget(URenewal_BattleFunctionLibrary::FindEnemy(Actor.Get(), Actor->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, AttackRange, EEnemySearchTargetTypeEnum::VE_Close));
									CurTarget = BattleEntComp->GetCurrentAttackTarget();
								}

								if (CurTarget)
								{
									FVector Location = UMapFunctionLibrary::GetNavAgentLocation(CurTarget);
									SkillEntComp->CastInputSkill(TempSkill->GetSkillID(), Actor.Get(), CurTarget, Location);
								}
								
								
								ECharacterRoleEnum role = CrewChangeProcess == ESkillOrigin::VE_Crew1 ? ECharacterRoleEnum::VE_Crew1 : ECharacterRoleEnum::VE_Crew2;
								FHeroSlotInfo* BattleSlot = RGAMEINSTANCE(this)->PlayStateInfo->GetBattleSlot(role);
								FHeroSlotInfo* RestSlot = RGAMEINSTANCE(this)->PlayStateInfo->GetRestSlot(role);
								if (BattleSlot)
								{
									BattleSlot->DespawnAll();
									BattleSlot->CharacterCrewState = ECharacterCrewStateEnum::VE_Rest;
								}
								RestSlot->CharacterCrewState = ECharacterCrewStateEnum::VE_Battle;

								/*Set SkillIcon blackboard Data*/
								if (BattleSlot)
								{
									URSkillSet_New* skillset = UUIFunctionLibrary::GetSkillSet(RGAMEINSTANCE(this));
									
									USkillIcon_New* SelectedSkillIcon = nullptr;
									if (role == ECharacterRoleEnum::VE_Crew1)
										SelectedSkillIcon = skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE2];
									else
										SelectedSkillIcon = skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE2];

									if (BattleSlot->SlotSpawnState == EHeroSpawnStateEnum::VE_Dead)
									{
										SelectedSkillIcon->Blackboard.ClearBlackboardData();
										SelectedSkillIcon->Blackboard.SetBlackboardData(EIconState::DEAD, true);
									}
									else
									{
										SelectedSkillIcon->Renderer.Prepare(*BattleSlot, ESkillCategoryTypeEnum::VE_ActiveSkill02);
										SelectedSkillIcon->Blackboard.MaxCoolTime = TempSkill->GetSkillInfo()->CoolTime;
										SelectedSkillIcon->Blackboard.CurrentCoolTime = SelectedSkillIcon->Blackboard.MaxCoolTime;
										SelectedSkillIcon->Blackboard.SetBlackboardData(EIconState::COOL, true);
									}


									if (role == ECharacterRoleEnum::VE_Crew1)
									{
										float IncreaseCoolTime = skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Blackboard.CurrentCoolTime;
										skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Blackboard.ClearBlackboardData();
										skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Renderer.Prepare(*RestSlot, ESkillCategoryTypeEnum::VE_ActiveSkill03);
										skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->IncreaseCoolTime(IncreaseCoolTime);
									}
									else
									{
										float IncreaseCoolTime = skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Blackboard.CurrentCoolTime;
										skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Blackboard.ClearBlackboardData();
										skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Renderer.Prepare(*RestSlot, ESkillCategoryTypeEnum::VE_ActiveSkill03);
										skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->IncreaseCoolTime(IncreaseCoolTime);
									}
								}

								//play anim
								URCampaignHUD_ThreeControl* ThreeGameHUD = Cast<URCampaignHUD_ThreeControl>(UUIFunctionLibrary::GetInGameHUD());
								if (IsValid(ThreeGameHUD))
								{
									FString aniNameString = (role == ECharacterRoleEnum::VE_Crew1) ? TEXT("Crew1Change") : TEXT("Crew2Change");
									ThreeGameHUD->PlayAni(aniNameString);
									for (size_t i = 0; i < ThreeGameHUD->HeroIconArray.Num(); ++i)
									{
										FHeroSlotInfo slotinfo = ThreeGameHUD->HeroIconArray[i]->GetHeroSlotInfo();
										if (slotinfo.CharacterRole == role)
											ThreeGameHUD->HeroIconArray[i]->PlayAni(TEXT("CrewChange"));
									}
								}
							}
						}


						

						CrewChangeProcess = ESkillOrigin::VE_Leader;
					}
				}
			}
			else
			{
				UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_CHANGE_CREW Actor Is Not Vaild >> ActorUK[%d]"), _actor_update->actorUK);
			}
		}
	}
}

void URealTimeModManager::OnACK_BT_DROP_CREW(OVERLORD::RTS::S2C::ACK_BT_DROP_CREWT* Packet)
{
	if (Packet->error->type == RTSCE::RTSCE_SUCCESS)
	{
		TWeakObjectPtr<AActor> Actor = RaidGameMode->ObjectHelper.GetObjectByPlayerUK(Packet->actorUK);
		if (!Actor.IsValid())
		{
			//Update_RTS_Actor(_actor_update->);
			FRTS_ACTOR_UPDATE crewInfo;
			crewInfo.actorUK = Packet->actorUK;
			crewInfo.direction = FVector(Packet->direction->X , Packet->direction->Y , Packet->direction->Z);
			crewInfo.location = FVector(Packet->location->X, Packet->location->Y, Packet->location->Z);
			crewInfo.isStop = false;

			RaidGameMode->OnACK_BT_CHANGE_CREW(&crewInfo);
			Actor = RaidGameMode->ObjectHelper.GetObjectByPlayerUK(crewInfo.actorUK);
			IEntityBaseProperty* BaseProperty = Cast< IEntityBaseProperty >(Actor.Get());
			if (BaseProperty && RGAMEINSTANCE(GEngine)->HttpClient->IsValidKID(BaseProperty->GetUserKID())) {
				if (CrewChangeProcess != ESkillOrigin::VE_Leader && RaidGameMode != nullptr)
				{
					//CamSettings
					//AStagePlayerController* PlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RaidGameMode->GetWorld()));
					//if (PlayerController)
					//{
					//	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(this)->GetWorld(), 0);
					//	ABaseCameraActor* BaseCam = Cast<ABaseCameraActor>(PCM->GetViewTarget());
					//	if (IsValid(BaseCam))
					//	{
					//		BaseCam->MoveStageCamera(Actor->GetActorLocation(), 0.f);
					//		BaseCam->PingPongCameraForward(Actor->GetActorLocation(), 1.f);

					//		BaseCam->AutoZoomInOutCameraByPercent(
					//			true,//bStartZoomIn, 
					//			0.5f,//ZoomPercent, 
					//			0.15f,//StartTime, 
					//			0.0f,//DurationTime, 
					//			0.5f,//EndTime, 
					//			nullptr, nullptr);
					//	}
					//}

					auto SkillEntComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Actor.Get());
					auto BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Actor.Get());

					if (IsValid(SkillEntComp) && IsValid(BattleEntComp))
					{
						UISkill_V2* TempSkill = SkillEntComp->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill02);
						if (IsValid(TempSkill))
						{
							AActor* CurTarget = nullptr;
							if (TempSkill != nullptr && TempSkill->GetSkillInfo()->TargetType != ESkillTargetTypeEnum::VE_Target)
								CurTarget = Actor.Get();
							else
								CurTarget = BattleEntComp->GetCurrentAttackTarget();

							if (!IsValid(CurTarget))
							{
								UCharacterBattleInfo* BattleInfo = BattleEntComp->GetBattleInfo();
								float AttackRange = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);

								BattleEntComp->SetCurrentAttackTarget(URenewal_BattleFunctionLibrary::FindEnemy(Actor.Get(), Actor->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, AttackRange, EEnemySearchTargetTypeEnum::VE_Close));
								CurTarget = BattleEntComp->GetCurrentAttackTarget();
							}

							if (CurTarget)
							{
								FVector Location = UMapFunctionLibrary::GetNavAgentLocation(CurTarget);
								SkillEntComp->CastInputSkill(TempSkill->GetSkillID(), Actor.Get(), CurTarget, Location);
							}
							
							ECharacterRoleEnum role = CrewChangeProcess == ESkillOrigin::VE_Crew1 ? ECharacterRoleEnum::VE_Crew1 : ECharacterRoleEnum::VE_Crew2;
							FHeroSlotInfo* BattleSlot = RGAMEINSTANCE(this)->PlayStateInfo->GetBattleSlot(role);
							FHeroSlotInfo* RestSlot = RGAMEINSTANCE(this)->PlayStateInfo->GetRestSlot(role);
							

							/*Set SkillIcon blackboard Data*/
							URSkillSet_New* skillset = UUIFunctionLibrary::GetSkillSet(RGAMEINSTANCE(this));

							USkillIcon_New* SelectedSkillIcon = nullptr;
							if (role == ECharacterRoleEnum::VE_Crew1)
								SelectedSkillIcon = skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE2];
							else
								SelectedSkillIcon = skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE2];

							if (RestSlot)
							{
								SelectedSkillIcon->Renderer.Prepare(*RestSlot, ESkillCategoryTypeEnum::VE_ActiveSkill02);
								SelectedSkillIcon->Blackboard.MaxCoolTime = TempSkill->GetSkillInfo()->CoolTime;
								SelectedSkillIcon->Blackboard.CurrentCoolTime = SelectedSkillIcon->Blackboard.MaxCoolTime;
								SelectedSkillIcon->Blackboard.SetBlackboardData(EIconState::COOL, true);
							}
							else
								SelectedSkillIcon->Blackboard.SetBlackboardData(EIconState::LOCK, true);

							if (BattleSlot)
							{
								if (role == ECharacterRoleEnum::VE_Crew1)
								{
									skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
									skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW1_ACTIVE3]->Renderer.Prepare(*BattleSlot, ESkillCategoryTypeEnum::VE_ActiveSkill03);
								}
								else
								{
									skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
									skillset->Variables.SkillIconArray[(int32)ESkillIconSeq::CREW2_ACTIVE3]->Renderer.Prepare(*BattleSlot, ESkillCategoryTypeEnum::VE_ActiveSkill03);
								}

								skillset->Variables.BG_3Combine->SetVisibility(ESlateVisibility::Collapsed);
							}

							URCampaignHUD_ThreeControl* ThreeGameHUD = Cast<URCampaignHUD_ThreeControl>(UUIFunctionLibrary::GetInGameHUD());
							if (IsValid(ThreeGameHUD))
							{
								if (role == ECharacterRoleEnum::VE_Crew1)
									ThreeGameHUD->HeroIconArray[ROLE_CREW1_BATTLE_INDEX]->SetRTSHeroSlot(*BattleSlot);
								else
									ThreeGameHUD->HeroIconArray[ROLE_CREW2_BATTLE_INDEX]->SetRTSHeroSlot(*BattleSlot);
							}
						}
					}
					CrewChangeProcess = ESkillOrigin::VE_Leader;
				}
			}
		}
	}
}

void URealTimeModManager::OnACK_BT_CHANGE_SPIRIT(OVERLORD::RTS::S2C::ACK_BT_CHANGE_SPIRITT* Packet)
{
// 	if (Packet->error->type == RTSCE::RTSCE_SUCCESS)
// 	{
// 		if (OnChangeSpirit.IsBound())
// 			OnChangeSpirit.Broadcast(Packet->actorUK, Packet->spiritIndex);
// 		SpiritChangeProcess = -1;
// 	}
}

void URealTimeModManager::SetGameMode(ARaidRGameMode* inGameMode)
{
	RaidGameMode = inGameMode;
}

void URealTimeModManager::ClearData()
{
	TeamList.Empty();
	RTSPlayerList.Empty();
	ActorList.Empty();
	RaidStageTableInfo = nullptr;
//	WorldRaidStageInfo = nullptr;
	bIsControlable = false;
	GameTimeSec = 0;
}

void URealTimeModManager::EnterWorldRaid(FName StageKey)
{
	PlayStageID = StageKey;
	RaidStageTableInfo = nullptr;
// 	WorldRaidStageInfo = UTableManager::GetInstancePtr()->GetWorldBossStage(PlayStageID);
// 	checkf(WorldRaidStageInfo != nullptr, TEXT("Map Table Empty"));
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] EnterWorldRaid >> %d, %s, %d, %d"), (int16)EPVP::SR, *(PlayStageID.ToString()), (int16)true, (int16)false);
	UE_LOG(LogRTS, Log, TEXT("RTS>> [SEND] REQ_MATCH ModType[%d], StageID[%s], Create[%d], Public[%d]"), (int16)EPVP::SR, *(PlayStageID.ToString()), true, false);
	URTSManager::GetInstancePtr()->REQ_MATCH(EPVP::SR, PlayStageID.ToString(), false, false);
}

void URealTimeModManager::EnterColosseum(FName StageKey)
{
	PlayStageID = StageKey;
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] EnterColosseum >> %d, %s, %d, %d"), (int16)EPVP::CS, *(PlayStageID.ToString()), (int16)true, (int16)false);
	UE_LOG(LogRTS, Log, TEXT("RTS>> [SEND] REQ_MATCH ModType[%d], StageID[%s], Create[%d], Public[%d]"), (int16)EPVP::CS, *(PlayStageID.ToString()), true, false);
	URTSManager::GetInstancePtr()->REQ_MATCH(EPVP::CS, PlayStageID.ToString(), true, false);
}


void URealTimeModManager::EnterTowerRaid(FName StageKey)
{
	PlayStageID = StageKey;
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	RaidStageTableInfo = UTableManager::GetInstancePtr()->GetRaidStageTableRow(PlayStageID);
	if (RaidStageTableInfo->PlayMode == FName("M"))
	{
		FBossStage* BossTable = UTableManager::GetInstancePtr()->GetBossStage(PlayStageID);

		BaseStageInfo->StageID = PlayStageID;
		BaseStageInfo->MapTableKey = RaidStageTableInfo->MapTableKey;
		BaseStageInfo->ClearRewardTableKey = RaidStageTableInfo->RewardPreviewKey;

		if (JoinNumberString.IsEmpty())
			URTSManager::GetInstancePtr()->REQ_MATCH(EPVP::MR, PlayStageID.ToString(), true, false);
		else if (JoinNumberString.Contains(TEXT("FastEnterance")))
		{
			URTSManager::GetInstancePtr()->REQ_MATCH(EPVP::MR, PlayStageID.ToString(), false, false);
			JoinNumberString.Empty();
		}
		else
		{
			URTSManager::GetInstancePtr()->REQ_MATCH_JOIN(EPVP::MR, PlayStageID.ToString(), FCString::Atoi(*JoinNumberString), TEXT(""));
			JoinNumberString.Empty();
		}
	}
	else
	{
//		WorldRaidStageInfo = nullptr;
		BaseStageInfo->StageID = PlayStageID;
		BaseStageInfo->MapTableKey = RaidStageTableInfo->MapTableKey;
		BaseStageInfo->ClearRewardTableKey = RaidStageTableInfo->RewardPreviewKey;
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] REQ_MATCH >> %d, %s, %d, %d"), (int16)EPVP::SR, *(PlayStageID.ToString()), (int16)true, (int16)false);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [SEND] REQ_MATCH ModType[%d], StageID[%s], Create[%d], Public[%d]"), (int16)EPVP::SR, *(PlayStageID.ToString()), true, false);
		URTSManager::GetInstancePtr()->REQ_MATCH(EPVP::SR, PlayStageID.ToString(), true, false);
	}
}


void URealTimeModManager::OnACK_AUTH(OVERLORD::RTS::S2C::ACK_AUTHT* Packet)
{
	ClearData();

	if (Packet->error->type == RTSCE::RTSCE_SUCCESS)
	{
		RTSInfo.kId = Packet->player->kId;
		FRTS_PLAYER newPlayerInfo;
		newPlayerInfo.SetData(Packet->player.get());
		RTSPlayerList.Emplace(RTSInfo.kId, newPlayerInfo);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_AUTH FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_MATCHED(OVERLORD::RTS::S2C::ACK_MATCHEDT* Packet)
{
	if (Packet->error->type == RTSCE::RTSCE_SUCCESS)
	{
		RTSInfo.modId = Packet->modId.c_str();
		RTSInfo.modType = (EPVP)Packet->modType;
		RTSInfo.partyNo = Packet->partyNo;
		RTSInfo.bPublic = Packet->bPublic;
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		//GameInstance->RWidgetManager->ClearUIPage();
		RaidStageTableInfo = UTableManager::GetInstancePtr()->GetRaidStageTableRow(FName(Packet->modId.c_str()));

		FString str = RaidStageTableInfo->MapTableKey.ToString();
		UE_LOG(LogRTS, Log, TEXT("SR_FloorStageDetail->MapTableKey %s"), *str);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_MATCHED SUCCESS[%d], ModID[%s], ModType[%d], PartyNo[%d], Public[%d]"), Packet->error->type, Packet->modId.c_str(), Packet->modType, Packet->partyNo, Packet->bPublic);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [SEND] REQ_PARTY_UI_READY PartyNo[%d]"), RTSInfo.partyNo);
		if (RTSInfo.modType == EPVP::SR)
		{
			
			BaseStageInfo->StageID = PlayStageID;
			BaseStageInfo->MapTableKey = FName(*str);
			BaseStageInfo->ClearRewardTableKey = RaidStageTableInfo->RewardPreviewKey;
			GameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_DeckSetting);
			URTSManager::GetInstancePtr()->REQ_PARTY_UI_READY(RTSInfo.partyNo);
		}
		if (RTSInfo.modType == EPVP::MR)
		{
			//BaseStageInfo->StageID = PlayStageID;
			BaseStageInfo->MapTableKey = FName(*str);
			BaseStageInfo->ClearRewardTableKey = RaidStageTableInfo->RewardPreviewKey;
			GameInstance->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_PartyRaidUI);
			URTSManager::GetInstancePtr()->REQ_PARTY_UI_READY(RTSInfo.partyNo);
		}
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_MATCHED FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_PARTY_UPDATE_PUBLIC(OVERLORD::RTS::S2C::ACK_PARTY_UPDATE_PUBLICT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		if (RTSInfo.partyNo == Packet->partyNo)
		{
			RTSInfo.bPublic = Packet->bPublic;
			if (OnUpdatePartyPublic.IsBound())
				OnUpdatePartyPublic.Broadcast();
		}
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PARTY_UPDATE_PUBLIC SUCCESSED[%d]"), Packet->error->type);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PARTY_UPDATE_PUBLIC FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_DECK_SELECT(OVERLORD::RTS::S2C::ACK_DECK_SELECTT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		FDeckSelected deckSelected;
		deckSelected.SetData(Packet);
		if (OnReceiveDeckSelect.IsBound())
			OnReceiveDeckSelect.Broadcast(deckSelected);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_DECK_SELECT SUCCESS Slot[%d], crewNo[%d] heroUD[%s], heroId[%s], level[%d]"), Packet->slot, Packet->crewNo, Packet->heroUD.c_str(), Packet->heroId.c_str(), Packet->level);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_DECK_SELECT FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_PARTY_PLAYER(OVERLORD::RTS::S2C::ACK_PARTY_PLAYERT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		FRTS_PLAYER NewPlayer;
		NewPlayer.SetData(Packet->player.get());
		NewPlayer.slot = Packet->slot;
		if (RTSPlayerList.Contains(Packet->player->kId))
			RTSPlayerList[Packet->player->kId] = NewPlayer;
		else
			RTSPlayerList.Emplace(Packet->player->kId, NewPlayer);
		
		if (NewPlayer.kId == GameInstance->UserInfo->kId)
		{
			RTSInfo.mSlotNo = Packet->slot;
		}
		if (OnUpdatePartPlayer.IsBound())
			OnUpdatePartPlayer.Broadcast(NewPlayer.kId);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PARTY_PLAYER SUCCESS[%d] kId[%d] Slot[%d] ModType[%d]"), Packet->error->type, Packet->player->kId, Packet->slot, int32(RTSInfo.modType));
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PARTY_PLAYER FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_PLAYER_EXIT(OVERLORD::RTS::S2C::ACK_PARTY_PLAYER_EXITT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
		{
			if (OnPartyPlayerExit.IsBound())
				OnPartyPlayerExit.Broadcast(Packet->slot);
			if (Packet->slot == RTSInfo.mSlotNo)
			{
				ClearRealTimeModData();
				ClearData();
			}
		}
		if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR))
		{
			ClearRealTimeModData();
			ClearData();
		}
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PLAYER_EXIT SUCCESS[%d] Slot[%d]"), Packet->error->type, Packet->slot);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PLAYER_EXIT FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_DECK_SELECT_READY(OVERLORD::RTS::S2C::ACK_DECK_SELECT_READYT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_DECK_SELECT_READY >> PartyNo[%d]"), RTSInfo.partyNo);
		if(URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR))
			URTSManager::GetInstancePtr()->REQ_PARTY_GO(RTSInfo.partyNo);
		if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
		{
			if (OnDeckReady.IsBound())
				OnDeckReady.Broadcast(Packet->slot, Packet->ready);
		}
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_DECK_SELECT_READY SUCCESS[%d] Slot[%d], Ready[%d]"), Packet->error->type, Packet->slot, Packet->ready);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_DECK_SELECT_READY FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_PARTY_GO(OVERLORD::RTS::S2C::ACK_PARTY_GOT* Packet)
{
	UUIFunctionLibrary::ShowWaitingPopup(false);
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		if (Packet->partyNo == RTSInfo.partyNo) 
		{
			//레디 완료함
			UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PARTY_GO >> PartyNo[%d]"), RTSInfo.partyNo);
		}
		else
		{
			//다른 파티가 레디를 누름
			UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PARTY_GO >> PartyNo[%d]"), RTSInfo.partyNo);
		}
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PARTY_GO FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_MATCH_ROUND(OVERLORD::RTS::S2C::ACK_MATCH_ROUNDT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		mRoomKey = Packet->roomkey.c_str();
		TeamList.Empty();
		RTSPlayerList.Empty();
		ActorList.Empty();
		GameInstance->PartyManager->ClearPartyList();
		// RTS_PlayerMap
		for (auto& playerData : Packet->players)
		{
			Update_RTS_Player(playerData.get());
		}

		for (auto& teamData : Packet->teams)
		{
			Update_RTS_Team(teamData.get());
		}

		for (auto& actor : Packet->actors)
		{
			Update_RTS_Actor(actor.get());
			UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_MATCH_ROUND Actor >>  %d"), actor->actorUK);
		}

		GameInstance->PlayModeManager->PlayModeRaidStart();
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_MATCH_ROUND SUCCESS[%d] ModID[%s] ModType[%d] RoomKey[%s]"), Packet->error->type, Packet->modId.c_str(), Packet->modType, Packet->roomkey.c_str());
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_MATCH_ROUND FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::Update_RTS_Actor(OVERLORD::RTS::STRUCT::ACTORT* InData)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] Update_RTS_Actor >> party %d kId %d actorId %s actorUK %d slot %d"), InData->party, InData->kId, InData->nick.c_str(), InData->actorUK, InData->slot);
	FRTS_ACTOR* FindInfo = nullptr;

	GameInstance->PartyManager->AddPartyMember(InData->party, InData->actorUK);
	if (ActorList.Contains(InData->actorUK))
	{
		// Rewriting data for re-create, re-gen...
		FindInfo = &ActorList[InData->actorUK];
		if (FindInfo)
		{
			FindInfo->SetData(InData);

			// FindInfo->heroUD = FString::FromInt(FindInfo->actorUK);
			// GameInstance->PartyManager->AddPartyMember(FindInfo->party, FindInfo->actorUK);
			if (GameInstance->HttpClient->IsValidKID(InData->kId))
			{
				UObjectFunctionLibrary::SetMyPartyID(InData->party);
				UObjectFunctionLibrary::SetMyTeamKey(InData->team);
			}
			else
			{
				FEQUIPED_HERO EquipedHeroInfo;
				EquipedHeroInfo.heroUD = FindInfo->heroUD;
				EquipedHeroInfo.heroId = FindInfo->actorId;
				EquipedHeroInfo.level = FindInfo->level;

				int16 SkillCount = FindInfo->skills.Num();
				EquipedHeroInfo.skills.Init(1, SkillCount);
				for (int16 n = 0; n < SkillCount; ++n)
				{
					EquipedHeroInfo.skills[n] = FindInfo->skills[n];
				}

				EquipedHeroInfo.equipedItems.Empty();
				if (ECharacterCategoryTypeEnum::VE_Hero == (ECharacterCategoryTypeEnum)InData->actorCategory)
				{
					int16 ItemCount = FindInfo->items.Num();
					if (0 < ItemCount)
					{
						int16 OptionCount = 0;
						int16 JewelCount = 0;
						EquipedHeroInfo.equipedItems.Reserve(ItemCount);
						for (int16 n = 0; n < ItemCount; ++n)
						{
							FITEM ItemInfo;

							ItemInfo.itemUD = "";
							ItemInfo.itemId = FName(*FindInfo->items[n].itemId);
							ItemInfo.enchant = FindInfo->items[n].enchant;

							ItemInfo.equipedHeroUD = FindInfo->heroUD;

							OptionCount = FindInfo->items[n].options.Num();
							if (0 < OptionCount)
							{
								ItemInfo.options.Reserve(OptionCount);
								for (int16 p = 0; p < OptionCount; ++p)
								{
									FITEM_OPTION ItemOptionInfo;

									ItemInfo.options.Emplace(ItemOptionInfo);
								}
							}

							EquipedHeroInfo.equipedItems.Emplace(ItemInfo);
						}
					}
				}
				GameInstance->OtherHeroManager->InsertOtherHeroData(FindInfo->kId, EquipedHeroInfo);
			}
		}
	} 
	else 
	{
		FindInfo = Get_RTS_Actor(InData->actorUK);
		if (FindInfo == nullptr)
		{
			FRTS_ACTOR newActorInfo;
			newActorInfo.SetData(InData);
			ActorList.Emplace(InData->actorUK, newActorInfo);
			UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager]RTS_ActorList Add %d Count %d"), InData->actorUK, ActorList.Num());
		}
	}	
}

FRTS_PLAYER* URealTimeModManager::Get_RTS_Player(int16 InKID)
{
	// UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] Get_RTS_Player >> KID %d"), InKID);
	return RTSPlayerList.Find(InKID);
}

FRTS_ACTOR* URealTimeModManager::Get_RTS_Actor(uint16 InActorUK)
{
//	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] Get_RTS_Actor >> actorUK %d"), InActorUK);
	FRTS_ACTOR* FindInfo = nullptr;
	if (ActorList.Contains(InActorUK))
		return &ActorList[InActorUK];
	return FindInfo;
}

void URealTimeModManager::OnACK_BT_ACTOR_UPDATE(OVERLORD::RTS::S2C::ACK_BT_ACTOR_UPDATET* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		TWeakObjectPtr<AActor> Actor = RaidGameMode->ObjectHelper.GetObjectByPlayerUK(Packet->update->actorUK);
		if (Actor.IsValid())
		{
			FRTS_ACTOR_UPDATE update;
			update.SetData(Packet->update.get());
			URemoteMoveComponent* RemoteMoveComponent = UCharacterHelpFunctionLibrary::GetComponent<URemoteMoveComponent>(Actor.Get());
			if (false == IsValid(RemoteMoveComponent))
				return;
			UMapFunctionLibrary::AdjustActorLocation(Actor.Get(), update.location);
			RemoteMoveComponent->Update(update);
			UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_ACTOR_UPDATE >> ActorUK[%d] IsStop[%d] location [%f, %f, %f] direction [%f, %f, %f]"),
				update.actorUK, update.isStop, update.location.X, update.location.Y, update.location.Z, update.direction.X, update.direction.Y, update.direction.Z);
		}
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_ACTOR_UPDATE >> actorUK %d"), Packet->update->actorUK);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_UPDATE FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_BT_SYNC_ACTOR_UPDATE(OVERLORD::RTS::S2C::ACK_BT_SYNC_ACTOR_UPDATET* Packet)
{
	if (Packet->error->type == RTSCE::RTSCE_SUCCESS)
	{
		if (!IsValid(GameInstance))
			return;
		if (!IsValid(RaidGameMode))
			return;
		for (auto& updateData : Packet->updates)
		{
			OVERLORD::RTS::STRUCT::ACTOR_UPDATET* _actor_update = updateData.get();
			TWeakObjectPtr<AActor> Actor = RaidGameMode->ObjectHelper.GetObjectByPlayerUK(_actor_update->actorUK);
			if (Actor.IsValid())
			{
				FRTS_ACTOR_UPDATE fActorUpdate;
				fActorUpdate.SetData(_actor_update);
				URemoteMoveComponent* RemoteMoveComponent = UCharacterHelpFunctionLibrary::GetComponent<URemoteMoveComponent>(Actor.Get());
				if (IsValid(RemoteMoveComponent))
				{
					UMapFunctionLibrary::AdjustActorLocation(Actor.Get(), fActorUpdate.location);
					RemoteMoveComponent->Update(fActorUpdate);
//					UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_SYNC_ACTOR_UPDATE >> ActorUK[%d] IsStop[%d] location [%f, %f, %f] direction [%f, %f, %f]"),
//						fActorUpdate.actorUK, fActorUpdate.isStop, fActorUpdate.location.X, fActorUpdate.location.Y, fActorUpdate.location.Z, fActorUpdate.direction.X, fActorUpdate.direction.Y, fActorUpdate.direction.Z);
				}
// 				else
// 				{
// 					UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_SYNC_ACTOR_UPDATE RemoteMoveComponent Is Not Vaild >> ActorUK[%d]"), _actor_update->actorUK);
// 				}
			}
			else
			{
				UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_SYNC_ACTOR_UPDATE Actor Is Not Vaild >> ActorUK[%d]"), _actor_update->actorUK);
			}
		}
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_SYNC_ACTOR_UPDATE (FAIL) >> force[%d], frame[%d]"), Packet->force, Packet->frame);
	}
}

void URealTimeModManager::OnMapLoadingComplete()
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnMapLoadingComplete >> %d"), RTSInfo.partyNo);
	UE_LOG(LogRTS, Log, TEXT("RTS>> [SEND] REQ_PLAY_READY >> %d"), RTSInfo.partyNo);
	URTSManager::GetInstancePtr()->REQ_PLAY_READY();
}

void URealTimeModManager::OnREQ_CINEMATIC_PLAY(OVERLORD::RTS::S2C::REQ_CINEMATIC_PLAYT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnREQ_CINEMATIC_PLAY >> %d"), Packet->sequence);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [SEND] ACK_CINEMATIC_DONE >> %d"), Packet->sequence);
		EPVP ConnectedMode = URTSManager::GetInstancePtr()->getConnectedMode();

		FString str = TEXT("MyTeamKey : ") + FString::FromInt(UObjectFunctionLibrary::GetMyTeamKey());
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, str);

		CinematicSequence = Packet->sequence;
		switch (ConnectedMode)
		{
			case EPVP::CS:
			{
				if (Packet->sequence == 1)
				{
					UUIFunctionLibrary::ReadyToStartUI_PVP();
					RGAMEINSTANCE(GEngine)->EventManager->OnUI_ReadyToStart.Clear();
				}
				else if (Packet->sequence == 2)
				{
					FName MyLeaderId = TEXT("");
					FName EnemyLeaderId = TEXT("");
					FName MyCrew1Id = TEXT("");
					FName EnemyCrew1Id = TEXT("");
					FName MyCrew2Id = TEXT("");
					FName EnemyCrew2Id = TEXT("");

					for (auto& Element : ActorList)
					{
						if (Element.Value.role == (uint8)ECharacterRoleEnum::VE_Leader)
						{
							if (Element.Value.team == UObjectFunctionLibrary::GetMyTeamKey())
								MyLeaderId = Element.Value.actorId;
							else
								EnemyLeaderId = Element.Value.actorId;
						}
						else if (Element.Value.role == (uint8)ECharacterRoleEnum::VE_Crew1 && Element.Value.slot == ROLE_CREW1_BATTLE_INDEX + 1)
						{
							if (Element.Value.team == UObjectFunctionLibrary::GetMyTeamKey())
								MyCrew1Id = Element.Value.actorId;
							else
								EnemyCrew1Id = Element.Value.actorId;
						}
						else if (Element.Value.role == (uint8)ECharacterRoleEnum::VE_Crew2  && Element.Value.slot == ROLE_CREW2_BATTLE_INDEX + 1)
						{
							if (Element.Value.team == UObjectFunctionLibrary::GetMyTeamKey())
								MyCrew2Id = Element.Value.actorId;
							else
								EnemyCrew2Id = Element.Value.actorId;
						}
					}


					FGameModeSequenceTableInfo* GameModeSequenceInfo = UTableManager::GetInstancePtr()->GetGameModeSequenceRow(FName(*(FString(TEXT("Colosseum")))));
					if (GameModeSequenceInfo)
					{
						if (false == GameModeSequenceInfo->StartSequence.IsNone())
						{
							ABaseSequenceActor* Sequence = RGAMEINSTANCE(this)->SequenceManager->SpawnSequence(GameModeSequenceInfo->StartSequence);
							if (Sequence)
							{
								TArray<AActor*> Actors;
								UGameplayStatics::GetAllActorsOfClass(RGAMEINSTANCE(this)->GetWorld(), ACharacter::StaticClass(), Actors);

								for (size_t i = 0; i < Actors.Num(); ++i)
								{
									UEntityRenderComponent* RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Actors[i]);
									if (RenderComponent)
									{
										RenderComponent->SetVisible(false);
										UWidgetDecoratorComponent* HpBarWidgetComponent = RenderComponent->GetHpBarWidgetComponent();
										if (IsValid(HpBarWidgetComponent))
											HpBarWidgetComponent->SetActiveWidgetDecorator(false);
									}
								}
								
								for (size_t i = 0; i < Actors.Num(); ++i)
								{
									if (Cast<ABattleBaseCharacter>(Actors[i]) == nullptr)
									{
										if (Actors[i]->GetActorLocation().X < 0.f)
										{
											RaidGameMode->LeftPosActor = Actors[i];
											continue;
										}
										else
										{
											RaidGameMode->RightPosActor = Actors[i];
											continue;
										}
									}
									
									IEntityBaseProperty* Property = Cast<IEntityBaseProperty>(Actors[i]);
									if (Property)
									{
										FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
										rules.LocationRule = EAttachmentRule::KeepRelative;
										rules.RotationRule = EAttachmentRule::KeepRelative;
										rules.ScaleRule = EAttachmentRule::SnapToTarget;
										if (Property->GetCharacterID() == MyLeaderId && Property->GetGroupKey() == UObjectFunctionLibrary::GetMyTeamKey())
										{
											RaidGameMode->MyLeader = Actors[i];
											RaidGameMode->MyLeader->SetActorRelativeLocation(FVector(0.f, 0.f, RaidGameMode->MyLeader->GetActorLocation().Z));
											//if (UObjectFunctionLibrary::GetMyTeamKey() == 2) // me - right
											//	Actors[i]->AddActorWorldRotation(FRotator(0.f, 180.f, 0.f));

											Cast<ACharacter>(Actors[i])->GetCharacterMovement()->GravityScale = 0.0f;
											Actors[i]->AttachToActor(UObjectFunctionLibrary::GetMyTeamKey() == 1 ? RaidGameMode->LeftPosActor : RaidGameMode->RightPosActor, rules);
											UEntityRenderComponent* RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Actors[i]);
											if (RenderComponent)
												RenderComponent->SetVisible(true);

											UAnimStateComponent* AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(Actors[i]);
											if (AnimStateComponent)
												AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StartMove);
										}
										else if (Property->GetCharacterID() == EnemyLeaderId)
										{
											RaidGameMode->EnemyLeader = Actors[i];

											RaidGameMode->EnemyLeader->SetActorRelativeLocation(FVector(0.f, 0.f, RaidGameMode->EnemyLeader->GetActorLocation().Z));
											//if (UObjectFunctionLibrary::GetMyTeamKey() == 1) // me - left
											//	Actors[i]->AddActorWorldRotation(FRotator(0.f, 180.f, 0.f));

											Cast<ACharacter>(Actors[i])->GetCharacterMovement()->GravityScale = 0.0f;
											Actors[i]->AttachToActor(UObjectFunctionLibrary::GetMyTeamKey() == 1 ? RaidGameMode->RightPosActor : RaidGameMode->LeftPosActor, rules);
											UEntityRenderComponent* RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Actors[i]);
											if (RenderComponent)
												RenderComponent->SetVisible(true);

											UAnimStateComponent* AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(Actors[i]);
											if (AnimStateComponent)
												AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_StartMove);
										}
										else if (Property->GetCharacterID() == MyCrew1Id)
											RaidGameMode->MyCrew1 = Actors[i];
										else if (Property->GetCharacterID() == MyCrew2Id)
											RaidGameMode->MyCrew2 = Actors[i];
										else if (Property->GetCharacterID() == EnemyCrew1Id)
											RaidGameMode->EnemyCrew1 = Actors[i];
										else if (Property->GetCharacterID() == EnemyCrew2Id)
											RaidGameMode->EnemyCrew2 = Actors[i];
									}
								}

								Sequence->OnSequenceEndEvent.Clear();
								Sequence->OnSequenceEndEvent.AddDynamic(this, &URealTimeModManager::OnSequenceEndColosseumStart);
							}
						}
					}
				}
				break;
			}

			case EPVP::SR:
			case EPVP::MR:
			{
				URTSManager::GetInstancePtr()->ACK_CINEMATIC_DONE(Packet->sequence);
				break;
			}
		}
		

		

		
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnREQ_CINEMATIC_PLAY FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_CINEMATIC_DONE(OVERLORD::RTS::S2C::ACK_CINEMATIC_DONET* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_CINEMATIC_DONE >> %d"), Packet->sequence);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_CINEMATIC_DONE FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_PLAY_READY(OVERLORD::RTS::S2C::ACK_PLAY_READYT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PLAY_READY >> actorUK"));
		AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
		RGameMode->NextGameModeState = EGameModeStateEnum::VE_Ready;
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_PLAY_READY FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnREQ_STEP_READY(OVERLORD::RTS::S2C::REQ_STEP_READYT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnREQ_STEP_READY >> %d"), Packet->sequence);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [SEND] ACK_STEP_AND_GO >> %d"), Packet->sequence);
		if(URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
		{
			if (OnCSStepAndGo.IsBound())
				OnCSStepAndGo.Broadcast(Packet->sequence);
		} 
		else 
		{
			// defaults for HealthCheck
			URTSManager::GetInstancePtr()->ACK_STEP_AND_GO(Packet->sequence);
		}
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnREQ_STEP_READY FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_STEP_GO(OVERLORD::RTS::S2C::ACK_STEP_GOT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_STEP_GO >> %d"), Packet->sequence);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_STEP_GO FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_BT_ACTOR_CREATE(OVERLORD::RTS::S2C::ACK_BT_ACTOR_CREATET* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_ACTOR_CREATE >> %d"), Packet->error->type);
		for (auto& actorData : Packet->actors)
		{
			Update_RTS_Actor(actorData.get());
			UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_CREATE >>  %d"), actorData->actorUK);
		}

	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_CREATE FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_DECK_CACHED_DONE(OVERLORD::RTS::S2C::ACK_DECK_CACHED_DONET* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_DECK_CACHED_DONE >> %d: %d %d"), Packet->error->type, Packet->deck.size(), OnReceiveDeckSelect.IsBound());
		if (OnReceiveDeckSelect.IsBound())
		{
			for (int i = 0; i < Packet->deck.size(); i++)
			{
				FDeckSelected deckSelected;
				deckSelected.SetData(Packet->deck[i].get());
				OnReceiveDeckSelect.Broadcast(deckSelected);
			}		
		}
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_DECK_CACHED_DONE FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_BT_ACTOR_SPAWN(OVERLORD::RTS::S2C::ACK_BT_ACTOR_SPAWNT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_ACTOR_SPAWN >> %d"), Packet->error->type);
		RaidGameMode->OnACK_BT_ACTOR_SPAWN(Packet);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_SPAWN FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_BT_ACTOR_RESPAWN(OVERLORD::RTS::S2C::ACK_BT_ACTOR_RESPAWNT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_BT_ACTOR_RESPAWN >> %d"), Packet->error->type);
		FRTS_ACTOR* actor = GameInstance->RealTimeModManager->Get_RTS_Actor(Packet->actorUK);
		if (actor == nullptr)
			return;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FVector(Packet->location->X, Packet->location->Y, Packet->location->Z));
		SpawnTransform.SetRotation(FVector(Packet->direction->X, Packet->direction->Y, Packet->direction->Z).Rotation().Quaternion());

		bool bRemote = true;

		if (GameInstance->HttpClient->IsValidKID(actor->kId))
			bRemote = false;
		int32 CommandSlotIndex = actor->slot - 1;
		AActor *SpawnedCharacter = UObjectFunctionLibrary::SpawnCharacterWithRTS(SpawnTransform, actor->actorId, (ECharacterCategoryTypeEnum)actor->actorCategory,
			actor->kId, actor->heroUD, actor->team, actor->party, actor->actorUK, actor->level, actor->grade, true, CommandSlotIndex, bRemote, true);

		FString CategoryType = UUtilFunctionLibrary::GetEnumToString(TEXT("ECharacterCategoryTypeEnum"), actor->actorCategory);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_SPAWN ECharacterCategoryTypeEnum[%s]"), *CategoryType);

		if (!IsValid(SpawnedCharacter))
			return;

		GameInstance->BattleRecordManager->SetHeroBattleInfoDelegate(SpawnedCharacter);
		if (actor->isSummon)
		{
			TWeakObjectPtr<AActor> PrimaryActor = RaidGameMode->ObjectHelper.GetObjectByPlayerUK(actor->primaryUK);
			if (PrimaryActor.IsValid())
			{
				UBattleEntityComponent *PrimaryBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(PrimaryActor.Get());
				UBattleEntityComponent *SummonBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedCharacter);
				if (IsValid(PrimaryBattleEntityComponent) && IsValid(SummonBattleEntityComponent))
				{
					SummonBattleEntityComponent->SetSummonOwner(PrimaryActor.Get());
					SummonBattleEntityComponent->SetCurrentAttackTarget(PrimaryBattleEntityComponent->GetCurrentAttackTarget());
					PrimaryBattleEntityComponent->GetBattleInfo()->AddSummonedMinion(SpawnedCharacter);
				}
			}
		}

		FRTS_PLAYER *OwnerPlayer = GameInstance->RealTimeModManager->Get_RTS_Player(actor->kId);
		if (OwnerPlayer)
			GameInstance->BattleRecordManager->SetUserNickName(OwnerPlayer->kId, OwnerPlayer->nick, actor->subNick);
		if (GameInstance->HttpClient->IsValidKID(actor->kId))
		{
			IEntityBaseProperty *BaseProp = Cast<IEntityBaseProperty>(SpawnedCharacter);
			RaidGameMode->MyGroupKey = BaseProp->GetGroupKey();

			GameInstance->PlayStateInfo->SetSpawnedCharacter(CommandSlotIndex, SpawnedCharacter);

			if (CommandSlotIndex == ROLE_LEADER_INDEX)
			{
				UWorld *GameWorld = GEngine->GetWorldFromContextObject(GameInstance, EGetWorldErrorMode::Assert);
				ARaidPlayerController *BasePlayerController = Cast<ARaidPlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameWorld));
				BasePlayerController->SetSelectedCharacter(SpawnedCharacter);
				BasePlayerController->SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);
				BasePlayerController->ActorSettingOnTheCamera(SpawnedCharacter, 0.1f, 0.1f);
			}
		}
		else
		{
			IEntityBaseProperty *BaseProp = Cast<IEntityBaseProperty>(SpawnedCharacter);
			RaidGameMode->OtherGroupKey = BaseProp->GetGroupKey();
		}

		SpawnedCharacter->GetWorldTimerManager().SetTimerForNextTick(this, &URealTimeModManager::Deferred_ShowIngameHUD);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_RESPAWN FAILED[%d]"), Packet->error->type);
	}

	
}

void URealTimeModManager::OnACK_PLAY_START(OVERLORD::RTS::S2C::ACK_PLAY_STARTT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		bIsControlable = true;
		AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
		RGameMode->NextGameModeState = EGameModeStateEnum::VE_Start;
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PLAY_START >> %d"), Packet->error->type);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PLAY_START FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_BT_ACTOR_DESPAWN(OVERLORD::RTS::S2C::ACK_BT_ACTOR_DESPAWNT* Packet)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_DESPAWN >> error %d"), Packet->error->type);
	if (Packet->error->type != RTSCE::RTSCE_SUCCESS)
		return;
	
	/*Not For Crew Change*/
	if (CrewChangeProcess != ESkillOrigin::VE_Leader)
		return;

	// Check RTS Actor
	FRTS_ACTOR* ActorInfo = Get_RTS_Actor(Packet->actorUK);
	if (ActorInfo == nullptr)
		return;

	// Check GameMode
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!pGameMode)
		return;
	
	// Check Actor Object
	TWeakObjectPtr<AActor> TargetActor = nullptr;
	TargetActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(Packet->actorUK);
	if (!TargetActor.IsValid())
		return;

	// Check Battle Entity
	UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor.Get());
	if (!IsValid(BattleEntityComp))
		return;
	
	// BattleEntityComp->Dead();
	// BattleEntityComp->ForceDead(nullptr);
	pGameMode->ObjectHelper.RemoveObject(Packet->actorUK, true);

	/**
	 * disable GuildRaid's Select Hero
	if (GameInstance->HttpClient->IsValidKID(ActorInfo->kId))
	{
		if ((int16)ECharacterCategoryTypeEnum::VE_Hero == ActorInfo->actorCategory)
		{
			if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR))
			{
				auto UserWidget = GameInstance->RWidgetManager->GetUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_RealTimeGroup);
				if (IsValid(UserWidget))
				{
					auto RealTimeGroupWidget = Cast<UUC_GuildRaid_Group>(UserWidget);
					if (IsValid(RealTimeGroupWidget))
					{
						RealTimeGroupWidget->SetGroupStateType(EGroupStateType::VE_ReSpawn);
					}
					GameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_RealTimeGroup);
				}
			}
			MyKillPlayerUK = Packet->actorUK;
		}
	}
	*/
}

void URealTimeModManager::OnACK_BT_ACTOR_DISAPPEAR(OVERLORD::RTS::S2C::ACK_BT_ACTOR_DISAPPEART* Packet)
{
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_DISAPPEAR >> error %d: %d > %d"), Packet->error->type, Packet->actorUK, Packet->killerUK );
	if (Packet->error->type != RTSCE::RTSCE_SUCCESS)
		return;

	// Check GameMode
	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (!pGameMode)
		return;

	// Check RTS Actor
	FRTS_ACTOR* ActorInfo = Get_RTS_Actor(Packet->actorUK);
	if (ActorInfo == nullptr)
		return;

	/*
	this code needs to be fixed later..
	From now on, Client has a responsibility for deleting NewtralNPC objects , not Server.
	if we get a packet related to despawning netralnpc, we just ignore that packets.
	*/
	if ((ECharacterCategoryTypeEnum)ActorInfo->actorCategory == ECharacterCategoryTypeEnum::VE_NeutralNpc)
		return;

	/*
	So, When do we have to delete neutral objects?
	Only if when leader is slained by someone else.
	*/
	if (ActorInfo->role == (uint8)ECharacterRoleEnum::VE_Leader)
	{
		TArray<int16> DeleteUKArray;
		for (auto& Element : ActorList)
		{
			if (Element.Value.actorCategory == (uint8)ECharacterCategoryTypeEnum::VE_NeutralNpc)
			{
				if (Element.Value.nick.Contains(TEXT("Hidden")))
					continue;

				DeleteUKArray.AddUnique(Element.Key);
			}
		}

		for (size_t i = 0; i < DeleteUKArray.Num(); ++i)
		{
			TWeakObjectPtr<AActor> TargetActor = nullptr;
			TargetActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(DeleteUKArray[i]);
			if (!TargetActor.IsValid())
				continue;

			UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor.Get());
			if (!IsValid(BattleEntityComp))
				continue;

			BattleEntityComp->Dead();
			BattleEntityComp->ForceDead(nullptr);
		}
	}

	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_DISAPPEAR >>  %s"), *(ActorInfo->nick) );
	
	// Check Actor Object
	TWeakObjectPtr<AActor> TargetActor = nullptr;
	TargetActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(Packet->actorUK);
	if (!TargetActor.IsValid())
		return;

	FRTS_ACTOR* KillerInfo = Get_RTS_Actor(Packet->killerUK);
	if (KillerInfo)
		RTSKillMsg(KillerInfo, ActorInfo);
	

	FHeroSlotInfo slotinfo = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetSelectedCharacterSlot(ROLE_LEADER_INDEX);
	if(IsValid(slotinfo.SpawnedTargetActor) && TargetActor == slotinfo.SpawnedTargetActor)
	{
		//UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Stage_FailEvent);
		auto ForceDead = [](ECharacterRoleEnum InRole) {
			AActor* ActiveCrew = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetBattleActiveCrewActor(InRole);
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

				int32 Idx = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetSelectedCharacterSlotIndex(ActiveCrew);
				RGAMEINSTANCE(GEngine)->PlayStateInfo->SetSpawnState(Idx, EHeroSpawnStateEnum::VE_Dead);
			}
		};

		ForceDead(ECharacterRoleEnum::VE_Leader);

		if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
		{
			UUserWidget* IngameHUD = UUIFunctionLibrary::GetInGameHUD();
			if (IsValid(IngameHUD) && IngameHUD->IsInViewport())
				IngameHUD->RemoveFromViewport();
		}
	}
	else
	{
		// Check Battle Entity
		UBattleEntityComponent* BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor.Get());
		if (!IsValid(BattleEntityComp))
			return;

		BattleEntityComp->Dead();
		BattleEntityComp->ForceDead(nullptr);
	}
	
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URTSReceiveHandler] OnACK_BT_ACTOR_DISAPPEAR DONE >>  %d"), Packet->actorUK);
	
	// pGameMode->ObjectHelper.RemoveObject(Packet->actorUK, true);

	/**
	 * disable GuildRaid's Select Hero
	if (GameInstance->HttpClient->IsValidKID(ActorInfo->kId))
	{
		if ((int16)ECharacterCategoryTypeEnum::VE_Hero == ActorInfo->actorCategory)
		{
			if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR))
			{
				auto UserWidget = GameInstance->RWidgetManager->GetUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_RealTimeGroup);
				if (IsValid(UserWidget))
				{
					auto RealTimeGroupWidget = Cast<UUC_GuildRaid_Group>(UserWidget);
					if (IsValid(RealTimeGroupWidget))
					{
						RealTimeGroupWidget->SetGroupStateType(EGroupStateType::VE_ReSpawn);
					}
					GameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_RealTimeGroup);
				}
			}
			MyKillPlayerUK = Packet->actorUK;
		}
	}
	*/
}

void URealTimeModManager::OnACK_CS_MATCHED(OVERLORD::RTS::S2C::ACK_CS_MATCHEDT* Packet)
{
	if (Packet->error->type == RTSCE::RTSCE_SUCCESS)
	{
		RTSInfo.modId = Packet->modId.c_str();
		RTSInfo.modType = (EPVP)Packet->modType;
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		FColosseumModes* StageTableInfo = UTableManager::GetInstancePtr()->GetColosseumModes(FName(Packet->modId.c_str()));
		BaseStageInfo->StageID = FName(Packet->modId.c_str());
		BaseStageInfo->MapTableKey = StageTableInfo->MapTableKey;

		for (auto& playerData : Packet->players)
		{
			Update_RTS_Player(playerData.get());
		}

		for (auto& teamData : Packet->teams)
		{
			Update_RTS_Team(teamData.get());
		}
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_CS_MATCHED >> %d ModID[%s], ModType[%d]"), Packet->error->type, Packet->modId.c_str(), Packet->modType);
		RGAMEINSTANCE(GEngine)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ColosseumSetting);
		URTSManager::GetInstancePtr()->REQ_CS_PARTY_UI_READY();
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_CS_MATCHED FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_CS_DECK_SELECT(OVERLORD::RTS::S2C::ACK_CS_DECK_SELECTT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		FCSDeckSelected deckSelected;
		deckSelected.SetData(Packet);

		if (OnReceiveCSDeckSelect.IsBound())
			OnReceiveCSDeckSelect.Broadcast(deckSelected);

		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_CS_DECK_SELECT SUCCESS kId[%d], CrewNo[%d], heroUD[%s], heroId[%s], level[%d]"), Packet->kId, Packet->crewNo, Packet->heroUD.c_str(), Packet->heroId.c_str(), Packet->level);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_CS_DECK_SELECT FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_CS_DECK_SELECT_READY(OVERLORD::RTS::S2C::ACK_CS_DECK_SELECT_READYT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		if (OnReceiveCSReady.IsBound())
			OnReceiveCSReady.Broadcast(Packet->kId, Packet->ready);

		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_CS_DECK_SELECT_READY >> SUCCESS[%d]"), Packet->error->type);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_CS_DECK_SELECT_READY FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnACK_MR_PLAYER_SCORE(OVERLORD::RTS::S2C::ACK_BT_SYNC_PLAYER_SCORET* Packet)
{
	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
	{
		UHUD_RaidThreeControl *RaidHUD = Cast<UHUD_RaidThreeControl>(UUIFunctionLibrary::GetInGameHUD());
		if (RaidHUD)
		{
			int32 MyPoint = 0;
			int32 EnemyPoint = 0;
			for (auto& scoreData : Packet->scores)
			{
				if (scoreData == nullptr)
					return;

				if (scoreData->kId == RGAMEINSTANCE(this)->HttpClient->GetKID())
					MyPoint = scoreData->medal;
				else
					EnemyPoint = scoreData->medal;
			}
			RaidHUD->OnColosseum_PointChanged.ExecuteIfBound(MyPoint, EnemyPoint);
		}
	}
	else
	{
		MS_Score_NickList.Reset();
		MS_Score_List.Reset();

		for (auto& scoreData : Packet->scores)
		{
			MS_Score_NickList.Emplace(UTF8_TO_TCHAR(scoreData->nick.c_str()));
			MS_Score_List.Emplace(scoreData->damage);
		}
		OnReceiveStatCount.Broadcast(MS_Score_NickList, MS_Score_List);
	}
}

void URealTimeModManager::OnACK_PLAY_TIMEOUT(OVERLORD::RTS::S2C::ACK_PLAY_TIMEOUTT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PLAY_TIMEOUT SUCCESS[%d]"), Packet->error->type);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_PLAY_TIMEOUT FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnExitGame()
{
	UPacketFunctionLibrary::TOWER_BOSS_PLAY_RESULT_RQ(this->mRoomKey);
	AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)->GetWorld()));
	GoToLobby();
	UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnExitGame"));
}

void URealTimeModManager::GoToLobby()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->EventManager->ClearEvent();
		RGameInstance->ChangeState(EGameStateEnum::VE_Lobby);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] GoToLobby"));
	}
}

void URealTimeModManager::ChangeCrew(ESkillOrigin CrewNo, FVector _Location, FVector _Direction)
{
	if (!URealTimeModManager::IsControlWithRTS())
		return;

	ECharacterRoleEnum CharacterRole = ECharacterRoleEnum::VE_None;
	if (CrewNo == ESkillOrigin::VE_Crew1)
		CharacterRole = ECharacterRoleEnum::VE_Crew1;
	if (CrewNo == ESkillOrigin::VE_Crew2)
		CharacterRole = ECharacterRoleEnum::VE_Crew2;
	CrewChangeProcess = CrewNo;
	int16 BattleUK = -1;
	int16 RestUK = -1;
	auto& SelectedCharacterArray = GameInstance->PlayStateInfo->GetSelectedCharacterArray();
	for (int32 index = 0; index < SelectedCharacterArray.Num(); index++)
	{
		if (SelectedCharacterArray[index].CharacterRole != CharacterRole)
			continue;
		if (SelectedCharacterArray[index].CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
		{
			BattleUK = SelectedCharacterArray[index].PlayerUK;
		}
		else if (SelectedCharacterArray[index].CharacterCrewState == ECharacterCrewStateEnum::VE_Rest)
		{
			RestUK = SelectedCharacterArray[index].PlayerUK;
		}		
	}

	if (BattleUK != -1 && RestUK != -1)
	{
		URTSManager::GetInstancePtr()->REQ_BT_CHANGE_CREW(BattleUK, RestUK, _Location, _Direction);
	}
}


void URealTimeModManager::DropCrew(ESkillOrigin CrewNo, FVector _Location, FVector _Direction)
{
	if (!URealTimeModManager::IsControlWithRTS())
		return;

	ECharacterRoleEnum CharacterRole = ECharacterRoleEnum::VE_None;
	if (CrewNo == ESkillOrigin::VE_Crew1)
		CharacterRole = ECharacterRoleEnum::VE_Crew1;
	if (CrewNo == ESkillOrigin::VE_Crew2)
		CharacterRole = ECharacterRoleEnum::VE_Crew2;

	CrewChangeProcess = CrewNo;
	
	int16 BattleUK = -1;
	auto& SelectedCharacterArray = GameInstance->PlayStateInfo->GetSelectedCharacterArray();
	for (int32 index = 0; index < SelectedCharacterArray.Num(); index++)
	{
		if (SelectedCharacterArray[index].CharacterRole != CharacterRole)
			continue;
		if (SelectedCharacterArray[index].CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
		{
			BattleUK = SelectedCharacterArray[index].PlayerUK;
		}
	}

	if (BattleUK != -1)
	{
		URTSManager::GetInstancePtr()->REQ_BT_DROP_CREW(BattleUK, _Location, _Direction);
	}
}

void URealTimeModManager::Deferred_ShowIngameHUD()
{
	UHUD_RaidThreeControl* IngameHUD = Cast<UHUD_RaidThreeControl>(UUIFunctionLibrary::GetInGameHUD());
	if (IsValid(IngameHUD) && !IngameHUD->IsInViewport())
	{
		TArray<FHeroSlotInfo>& SlotInfoArray = GameInstance->PlayStateInfo->GetSelectedCharacterArray();
		for (size_t i = 0; i < SlotInfoArray.Num(); ++i)
		{
			if (i == ROLE_LEADER_INDEX)
				continue;

			if (IsValid(SlotInfoArray[i].SpawnedTargetActor))
			{
				SlotInfoArray[i].SpawnedTargetActor->GetWorldTimerManager().SetTimerForNextTick(this, &URealTimeModManager::Deferred_ShowIngameHUD);
				return;
			}

			SlotInfoArray[i].SlotSpawnState = EHeroSpawnStateEnum::VE_Ready;
			if (i == ROLE_CREW1_BATTLE_INDEX || i == ROLE_CREW2_BATTLE_INDEX)
				SlotInfoArray[i].CharacterCrewState = ECharacterCrewStateEnum::VE_Battle;
			else if(i == ROLE_CREW1_REST_INDEX || i == ROLE_CREW2_REST_INDEX)
				SlotInfoArray[i].CharacterCrewState = ECharacterCrewStateEnum::VE_Rest;
		}
		IngameHUD->AddToViewport();
	}
}

void URealTimeModManager::ChangeSPIRIT(int32 nSpiritIndex)
{
	int32 ActorUK = -1;
	auto& SelectedCharacterArray = GameInstance->PlayStateInfo->GetSelectedCharacterArray();
	for (int32 index = 0; index < SelectedCharacterArray.Num(); index++)
	{
		if (SelectedCharacterArray[index].SpawnedTargetActor != nullptr)
		{
			SpiritChangeProcess = nSpiritIndex;
			ActorUK = SelectedCharacterArray[index].PlayerUK;
			break;
		}
	}
	if (ActorUK != -1)
		URTSManager::GetInstancePtr()->REQ_BT_CHANGE_SPIRIT(ActorUK, SpiritChangeProcess);
}

void URealTimeModManager::OnACK_LEADER_SLOT(OVERLORD::RTS::S2C::ACK_PARTY_LEADER_SLOTT* Packet)
{
	if (Packet->error->type == OVERLORD::RTS::ENUM::RTSCE::RTSCE_SUCCESS)
	{	
		if (OnLeaderSlot.IsBound())
			OnLeaderSlot.Broadcast(Packet->slot, Packet->leader);
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_LEADER_SLOT SUCCESS leader[%d], slot[%d]"), Packet->leader, Packet->slot);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("RTS>> [URealTimeModManager] OnACK_LEADER_SLOT FAILED[%d]"), Packet->error->type);
	}
}

void URealTimeModManager::OnSequenceEndColosseumStart(FName InSequenceKey)
{
	URTSManager::GetInstancePtr()->ACK_CINEMATIC_DONE(CinematicSequence);

	UUtilFunctionLibrary::ViewTargetBaseCamera(RGAMEINSTANCE(this)->GetWorld());
}

void URealTimeModManager::SetRTSResults(const TArray<FREWARD>& Reward)
{
	GameInstance = RGAMEINSTANCE(this);
	m_ResultAvatar = GameInstance->UserInfo->GetAvatarData();
	m_ResultReward = Reward;
}

void URealTimeModManager::RTSKillMsg(FRTS_ACTOR* InKiller, FRTS_ACTOR* InDie)
{
	/*	const FLinearColor TextColor(0.760784f, 0.819608f, 0.831373f, 1.f);*/

	if (InDie->isSummon)
		return;

	FColor KillerColor = FColor::Red;
	FColor DieColor = FColor::Red;

	if (GameInstance->HttpClient->IsValidKID(InKiller->kId))
	{
		KillerColor = FColor::Cyan;
		DieColor = FColor::Red;
	}
	else
	{
		KillerColor = FColor::Red;
		DieColor = FColor::Cyan;
	}

	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) || IsKillMsg(InKiller, InDie))
	{		
		FText KillerNick		=	UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Npc, FName(*InKiller->nick));
		FText DieNick			=	UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Npc, FName(*InDie->nick));

		KillerNick				=	FText::FromString(FString(TEXT("<span color=\"#")) + KillerColor.ToHex() + TEXT("\">") + KillerNick.ToString() + TEXT("</>"));
		DieNick					=	FText::FromString(FString(TEXT("<span color=\"#")) + DieColor.ToHex() + TEXT("\">") + DieNick.ToString() + TEXT("</>"));

		FText KillMsg = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Message_Kill"));
		FText DisplayText		=	FText::Format(KillMsg, KillerNick, DieNick);

		UUtilFunctionLibrary::ShowIndicator(DisplayText, 1.0f, true, false);
	}
}

bool URealTimeModManager::IsKillMsg(FRTS_ACTOR* InKiller, FRTS_ACTOR* InDie)
{
	if ((ECharacterCategoryTypeEnum)InKiller->actorCategory == ECharacterCategoryTypeEnum::VE_Hero)
	{
		if ((ECharacterCategoryTypeEnum)InDie->actorCategory == ECharacterCategoryTypeEnum::VE_Hero)
			return true;
		if (InDie->role == (uint8)ECharacterRoleEnum::VE_Boss)
			return true;

		return false;
	}

	return false;
}

void URealTimeModManager::OnACK_BT_DOOR_EVENT(OVERLORD::RTS::S2C::ACK_BT_DOOR_EVENTT* data)
{
	if (RaidGameMode)
		RaidGameMode->OnACK_BT_DOOR_EVENT(data->doorId, data->event);
}
