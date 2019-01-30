// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ISkill_V2.h"
#include "Skill_V2/ContinuousSkillApplierActor.h"

#include "GlobalIntegrated.h"




#include "CustomStructures/GameEnvironmentVar.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/Bullets/BulletBase.h"





#include "UtilFunctionIntegrated.h"

#include "Network/HttpRequestClient.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSpriteEmitter.h"
#include "Character/BattleBaseAnimInstance.h"	
#include "GameInfo/PlayStateInfo.h"
#include "LeaderControlHelper.h"



#define SUMMON_SPAWN_SEARCH_DEPTH_DIST 50.0f
#define GLOBAL_ACTIVE3_COOLTIME 3.f



void UISkill_V2::InitSkill(FName InSkillID, FName InSkillTableID, AActor* InCaster, int32 InLevel, FSkillTableInfo_V2* SkillInfo)
{
	checkf(InCaster != nullptr, TEXT("UISkill_V2::InitSkill[InCaster Is Empty]"));
	checkf(SkillInfo != nullptr, TEXT("UISkill_V2::InitSkill[SkillInfo Is Empty]"));
// 	if (InCaster == nullptr)
// 		return;
// 
// 	if (nullptr == SkillInfo)
// 		return;

	SkillLevel = InLevel;
	SkillID = InSkillID;
	SkillTableID = InSkillTableID;
	SkillInfo_V2 = SkillInfo;
	Caster = InCaster;
	Target = nullptr;

	CacheCasterComponent();
		
	CurrentCoolTime = SkillInfo_V2->StartCoolTime;
	CurrentCrewSkillCoolTime = 0;

	IEntityBaseProperty* CasterEntityBaseProperty = Cast< IEntityBaseProperty >(Caster.Get());
	if (CasterEntityBaseProperty == nullptr)
		return;

	if (SkillInfo_V2->SkillResourceID.IsNone() == false)
	{
		FSkillResourceTableInfo* CurSkillResourceInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(SkillInfo_V2->SkillResourceID);
		if (CurSkillResourceInfo != nullptr)
		{
			SkillResourceInfo = CurSkillResourceInfo;
		}
	}

	SkillSyncData.Clear();

	bRealTimeMode = URealTimeModManager::IsRealTimeGameMode();
}

void UISkill_V2::BeginDestroy()
{
	if (SkillState != ESkillStateEnum::VE_None)
	{
		if (SkillState != ESkillStateEnum::VE_End)
		{
#if LOG_SKILL
			UE_LOG(LogBattle, Log, TEXT("Blessing of Garbage %s Skill  "), *(GetSkillID().ToString()) );
#endif
			End();
		}
	}

	Super::BeginDestroy();
}

void UISkill_V2::Update(float DeltaTime)
{
	if (IsValidCaster())
	{
		if (CasterSkillEntityComponent->IsSkillCasting())
		{
			if (CasterSkillEntityComponent->GetCurrentSkill_V2() == this)
			{
				ElapsedCastingTime += DeltaTime;

				if (false == bRealTimeMode)
				{
					if (ElapsedCastingTime > SkillInfo_V2->CastingTime)
					{
						DoAction();
						ElapsedCastingTime = 0.0f;
					}
				}
				else
				{
					if (SkillSyncData.IsValid() )
					{
						DoAction();
						ElapsedCastingTime = 0.0f;
					}
				}
			}
		}

		UpdateCooling(DeltaTime);
		UpdateCrewSkillCooling(DeltaTime);
	}
}

void UISkill_V2::SetTarget(AActor* InTarget, const FVector& InTargetLocation)
{
	Target = InTarget;
	TargetLocation = InTargetLocation;
	
	if (Caster.Get() == InTarget)
	{
		if (TargetLocation.IsNearlyZero())
		{
			TargetLocation = Caster->GetActorLocation();
		}
	}
	else
	{
		if ( Target.IsValid() )
		{
			TargetLocation = Target->GetActorLocation();
		}
	}

	IEntityBaseProperty* CasterBaseProperty = Cast<IEntityBaseProperty>(Caster.Get());
	if (CasterBaseProperty != nullptr)
	{
		if (CasterBaseProperty->GetCharacterCategory() != ECharacterCategoryTypeEnum::VE_NeutralNpc)
		{
			if (false == TargetLocation.IsNearlyZero())
			{
				CasterAIEntityComponent->ForceDirection(TargetLocation);
			}
		}
	}

// 
// #if WITH_EDITOR
// 	if (GEngine)
// 	{
// 		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 		if (RGameInstance )
// 		{
// 			if (RGameInstance->GameEnvironmentVar.EnableSkillLog)
// 			{
// 				FString logMsg = FString::Printf(TEXT("[ %s ] Skill SetTarget - %s, %s, %.2f, %.2f, %.2f"),
// 					*Caster->GetName(),
// 					*SkillID.ToString(),
// 					(Target != nullptr) ? *Target->GetName() : TEXT("None"),
// 					TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
// 				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, logMsg);
// 			}
// 		}
// 	}
// #endif

}


bool UISkill_V2::Casting(AActor* InTarget, const FVector& InTargetLocation)
{
	if (IsValidCaster() == false)
		return false;

	if (CanCastSkill())
	{
		SetTarget(InTarget, InTargetLocation);

		if (0.0f < SkillInfo_V2->CoolTime)
		{
			StartCoolTime();
		}
		

		IEntityBaseProperty* CasterBaseProperty = Cast<IEntityBaseProperty>(Caster.Get());
		if (CasterBaseProperty == nullptr)
			return false;

		if (CasterBaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_NeutralNpc)
		{
			Fire();
			End();
			return true;
		}
		//if (SkillInfo_V2->CastingType == ESkillCastingTypeEnum::VE_Immediate)
		//{
		//	Fire();
		//	End();
		//	
		//	if (CasterBattleEntityComponent.IsValid() && IsValid(CasterBattleEntityComponent.Get()->GetPhaseComponent()))
		//		CasterBattleEntityComponent.Get()->GetPhaseComponent()->SkillEnd(SkillID);

		//	return true;
		//}

		if (SkillInfo_V2->CastingType == ESkillCastingTypeEnum::VE_Channeling)
		{
			ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(Caster->GetWorld());
			if (BasePlayerController)
			{
				BasePlayerController->ResetJoystickForChannelSkillMovingCast();
			}
		}

		CasterSkillEntityComponent->SetCurrentSkill_V2(this);

		if(SkillResourceInfo)
			CasterSkillEntityComponent->SetSkillAniType(SkillResourceInfo->SkillAniType);
		else
			CasterSkillEntityComponent->SetSkillAniType(ESkillAniTypeEnum::VE_None);
		
		auto CharacterMovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(Caster.Get());
		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->StopMovementImmediately();
		}

		CasterBattleInfoBlackboard->ClearLastAppliedSkillInformations();

		ElapsedCastingTime = 0.0f;
		if (SkillInfo_V2->CastingTime > 0) //Casting
		{
			SkillState = ESkillStateEnum::VE_Casting;			

			CasterSkillEntityComponent->SetSkillCasting(true);

			AttachCastingEffect(Caster.Get());
			AttachScreenEffect(Caster.Get(), ESkillStateEnum::VE_Casting);
		}
		else
		{
			DoAction();
		}

		ShowUltimateSkillEffect();

		CasterSkillEntityComponent->OnNotifySkillCasting.Broadcast(Caster.Get(), this);

		if (SkillInfo_V2->VisibleDecal)
		{
			if (CasterEntityRenderComponent.IsValid())
			{
				FVector CastingLocation = FVector::ZeroVector;
				if (Caster.Get() != InTarget)
				{
					CastingLocation = TargetLocation;
				}

				CasterEntityRenderComponent->ShowSkillCastingDecal(SkillInfo_V2->EffectShape, SkillInfo_V2->ShapeScope, SkillInfo_V2->CastingRange, SkillInfo_V2->CastingTime, CastingLocation);
			}
		}

#if LOG_SKILL
		UE_LOG(LogBattle, Log, TEXT("Skill Casting"));
#endif

		return true;
	}
	else
	{
#if LOG_SKILL
		UE_LOG(LogBattle, Log, TEXT("Casting fail - CanCastSkill fail"));
#endif		

#if WITH_EDITOR
		if (GEngine)
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if (RGameInstance )
			{
				if (RGameInstance->GameEnvironmentVar.EnableSkillLog)
				{					
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Casting fail - CanCastSkill fail") );
				}
			}
		}
#endif
	}

	return false;
}

void UISkill_V2::DoAction()
{
	if (IsValidCaster() == false)
		return;

	SkillState = ESkillStateEnum::VE_DoAction;

	CasterSkillEntityComponent->SetSkillCasting(false);
	CasterSkillEntityComponent->SetSkillActing(true);

	if (CastingEffect.IsValid())
	{
		CastingEffect->Deactivate();
		CastingEffect = nullptr;
	}
	if (CastingBeamEffect.IsValid())
	{
		CastingBeamEffect->Deactivate();
		CastingBeamEffect = nullptr;
	}

	AttachActionEffect(Caster.Get(), ESkillStateEnum::VE_DoAction);
	AttachScreenEffect(Caster.Get(), ESkillStateEnum::VE_DoAction);
}

bool UISkill_V2::Fire()
{
	SkillState = ESkillStateEnum::VE_Fire;

	IEntityBaseProperty* CasterBaseProperty = Cast<IEntityBaseProperty>(Caster.Get());
	if (CasterBaseProperty == nullptr)
		return false;

	if (CasterBaseProperty->GetCharacterCategory() != ECharacterCategoryTypeEnum::VE_NeutralNpc)
	{
		if (SkillResourceInfo == nullptr)
			return false;
	}

	AttachActionEffect(Caster.Get(), ESkillStateEnum::VE_Fire);
	AttachScreenEffect(Caster.Get(), ESkillStateEnum::VE_Fire);	

	/*
	Apply Caster Applier
	*/	
	// Apply caster effect after appling target effect
	if( SkillInfo_V2->ApplyProcedure == 0)
		CasterSkillEntityComponent->ApplySkill(this, Caster.Get(), true);
	
	/*
	Check Area Skill
	*/
	if (SkillInfo_V2->GroundDuration > 0.0f)
	{
		AContinuousSkillApplierActor* ApplierActor = nullptr;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Caster.Get();
		SpawnParams.Instigator = Cast< APawn >(Caster.Get());

		ApplierActor = Caster->GetWorld()->SpawnActor< AContinuousSkillApplierActor >(AContinuousSkillApplierActor::StaticClass(), TargetLocation, FRotator::ZeroRotator, SpawnParams);		
		if (ApplierActor)
		{
			ApplierActor->SetAttachedTarget(Caster.Get());
			ApplierActor->SpawnSkill( this, TargetLocation);

			if (bRealTimeMode)
			{
				ApplierActor->SetUsingOnlyEffect(true);
			}
			ContinuousApplierActor = ApplierActor;
		}
	}
	else
	{
		GetApplyTarget();

		bool bApplyToTarget = (0 < ApplyTargetActors.Num());

		if ((SkillResourceInfo != nullptr) && (false == SkillResourceInfo->BulletID.IsNone()))
		{
			FBulletTableInfo* BulletTableInfo = UTableManager::GetInstancePtr()->GetBulletRow(SkillResourceInfo->BulletID);
			if (nullptr == BulletTableInfo)
				return false;

			if (ESkillBulletTypeEnum::VE_Bullet == BulletTableInfo->BulletType && Target.IsValid())
			{
				CasterSkillEntityComponent->FireTargetBullet(this, Caster.Get(), Target.Get(), bApplyToTarget);
			}
			else if (ESkillBulletTypeEnum::VE_MultiBullet == BulletTableInfo->BulletType  && Target.IsValid())
			{
				CasterSkillEntityComponent->FireMultiBullet(this, Caster.Get(), Target.Get(), TargetLocation, bApplyToTarget);
			}
			else if (ESkillBulletTypeEnum::VE_LocationBullet == BulletTableInfo->BulletType)
			{
				CasterSkillEntityComponent->FireLocationBullet(this, Caster.Get(), TargetLocation, bApplyToTarget);
			}
		}
		else
		{
			if (bApplyToTarget)
			{
				Apply();
			}
		}
	}

	USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster.Get());
	// Not needed target skill apply.. ex) summon
	if(IsValid(SkillComponent))
		SkillComponent->ApplyInstanceApplierGroup(Caster.Get(), Target.Get(), SkillInfo_V2, false, this, true);

	// Apply caster effect after appling target effect
	if (SkillInfo_V2->ApplyProcedure != 0)
		CasterSkillEntityComponent->ApplySkill(this, Caster.Get(), true);

	return true;
}

bool UISkill_V2::DummyFire()
{
	if (SkillResourceInfo == nullptr)
		return false;

	AttachActionEffect(Caster.Get(), ESkillStateEnum::VE_Fire);
	AttachScreenEffect(Caster.Get(), ESkillStateEnum::VE_Fire);

	FBulletTableInfo* BulletTableInfo = UTableManager::GetInstancePtr()->GetBulletRow(SkillResourceInfo->BulletID);
	if (BulletTableInfo)
	{
		if (ESkillBulletTypeEnum::VE_Bullet == BulletTableInfo->BulletType)
		{
			CasterSkillEntityComponent->FireTargetBullet(this, Caster.Get(), Target.Get(), false);
		}
		else if (ESkillBulletTypeEnum::VE_MultiBullet == BulletTableInfo->BulletType)
		{
			CasterSkillEntityComponent->FireMultiBullet(this, Caster.Get(), Target.Get(), TargetLocation, false);
		}
		else if (ESkillBulletTypeEnum::VE_LocationBullet == BulletTableInfo->BulletType)
		{
			CasterSkillEntityComponent->FireLocationBullet(this, Caster.Get(), TargetLocation, false);
		}
	}

	return true;
}

bool UISkill_V2::Apply()
{
	if (IsValidCaster() == false)
		return false;

	AActor* ApplyTarget = nullptr;
	USkillEntityComponent* Target_SkillEntityComponent = nullptr;

	int32 TargetCount = ApplyTargetActors.Num();
	for (int32 n = 0; n < TargetCount; ++n)
	{
		if (ApplyTargetActors.IsValidIndex(n) && ApplyTargetActors[n].IsValid())
		{
			ApplyTarget = ApplyTargetActors[n].Get();

			Target_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(ApplyTarget);
			if (IsValid(Target_SkillEntityComponent))
			{
				Target_SkillEntityComponent->ApplySkill(this, Caster.Get());
			}

			AttachHitEffect(ApplyTarget);
		}
	}

	//if (SkillSyncData.IsValid() && bRealTimeMode)
	//{
	//	if (EBattleDamageResultEnum::VE_Heal == SkillSyncData.CasterApplyData.DamageResult ||
	//		EBattleDamageResultEnum::VE_NoneHitDamage == SkillSyncData.CasterApplyData.DamageResult)
	//	{
	//		FCSNormalAttackSyncData ChangedHPData;
	//		ChangedHPData.Target = Caster.Get();
	//		ChangedHPData.HitResult = SkillSyncData.CasterApplyData.DamageResult;
	//		ChangedHPData.HitDamage = SkillSyncData.CasterApplyData.Damage;
	//		ChangedHPData.SyncHP = SkillSyncData.CasterApplyData.SyncHP;
	//		CasterBattleEntityComponent->CSOnDamage(nullptr, ChangedHPData);
	//	}
	//}

#if WITH_EDITOR
	if (GEngine)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		if (RGameInstance )
		{
			if (RGameInstance->GameEnvironmentVar.EnableSkillLog)
			{
				FString logMsg = FString::Printf(TEXT("[ %s ] Apply - %s, %s, TargetCount:%d"),
					*Caster->GetName(),
					*SkillID.ToString(),
					(Target != nullptr) ? *Target->GetName() : TEXT("None"),
					ApplyTargetActors.Num() );
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, logMsg );
			}
		}
	}
#endif

	return true;
}

void UISkill_V2::ApplySkillBullet()
{
	Apply();
}

void UISkill_V2::End()
{
	SkillState = ESkillStateEnum::VE_End;
	auto RGameInstance = RGAMEINSTANCE(this);
	bool bMySkill = UUtilFunctionLibrary::IsMyPlayCharacter(Caster.Get());

	if (CastingEffect.IsValid())
	{
		// lkk1989 akhan CastingEffect
		// TODO : Check effect option..
		if ((CasterBattleEntityComponent.IsValid() && CasterBattleEntityComponent->IsDead())
			|| SkillID == "NPC_Akhan_Roar"
			|| SkillID == "NPC_Akhan_WindAttack"
			|| SkillID == "NPC_Akhan_Breath")
		{
			CastingEffect->DestroyComponent();
		}
		else
		{
			CastingEffect->Deactivate();
		}

		CastingEffect = nullptr;
	}

	if (CastingBeamEffect.IsValid())
	{
		if (CasterBattleEntityComponent.IsValid() && CasterBattleEntityComponent->IsDead())
		{
			CastingBeamEffect->DestroyComponent();
		}
		else
		{
			CastingBeamEffect->Deactivate();
		}
		CastingBeamEffect = nullptr;
	}

	if (ActionEffect.IsValid())
	{
		if (CasterBattleEntityComponent.IsValid() && CasterBattleEntityComponent->IsDead())
		{
			ActionEffect->DestroyComponent();
		}
		else
		{
			ActionEffect->Deactivate();
		}
		ActionEffect = nullptr;
	}

	if (IsValidCaster())
	{
		CasterSkillEntityComponent->SetSkillCasting(false);
		CasterSkillEntityComponent->SetSkillActing(false);
		CasterSkillEntityComponent->SetSkillAniType(ESkillAniTypeEnum::VE_None);

		CasterBattleInfoBlackboard->LastAppliedSkill = SkillID;
	}

	if (SkillSyncData.IsValid())
	{
		SkillSyncData.Clear();
	}
	ReservedSkillEnd = false;

//	DeleteContinuousActor();

	if (bMySkill && IsValid(RGameInstance))
	{
		if (SkillCategoryType == ESkillCategoryTypeEnum::VE_ActiveSkill03)
		{
			//float JustInputTime = 2.0f;
			//RGameInstance->GetWorld()->GetTimerManager().SetTimer(ComboDelayTimeHandler, this, &UISkill_V2::CheckNextComboSkill, JustInputTime);
			//UGameplayStatics::SetGamePaused(RGameInstance, true);
			
		}
// 		float JustInputTime = 2.0f;
// 		RGameInstance->GetWorld()->GetTimerManager().SetTimer(ComboDelayTimeHandler, this, &UISkill_V2::CheckNextComboSkill, JustInputTime);
// 		UGameplayStatics::SetGamePaused(RGameInstance, true);
	}
	//CheckNextComboSkill();


#if LOG_SKILL
	UE_LOG(LogBattle, Log, TEXT("Skill End"));
#endif
}

void UISkill_V2::CastingAction()
{
	if (SkillInfo_V2 == nullptr)
		return;

	if (SkillInfo_V2->CastingEffect != ESkillApplierTypeEnum::VE_None)
	{
		USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster.Get());
		if (IsValid(SkillComponent))
		{
			ESkillApplierTypeEnum ApplierType = SkillInfo_V2->CastingEffect;
			SkillComponent->ApplyCastingActionApplier(Caster.Get(), Target.Get(), SkillInfo_V2->CastingEffect , SkillInfo_V2->CastingTime);
		}
	}
}

bool UISkill_V2::IsIncludeApplierTypeEnum(ESkillApplierTypeEnum InType)
{
	if (SkillInfo_V2 == nullptr)
		return false;
	else if (SkillInfo_V2->CastingEffect == InType)
		return true;
	else
		return false;
}

// void UISkill_V2::CheckNextComboSkill()
// {
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	UGameplayStatics::SetGamePaused(RGameInstance, false);
// 	if ( IsValid(RGameInstance->PlayStateInfo))
// 	{
// 		//int32 index = 0;
// 		TPair<int32, int32> item;
// 		if (RGameInstance->PlayStateInfo->ComboSkillQueue.Dequeue(item))
// 		{
// 			FHeroSlotInfo& HeroSlot = RGameInstance->PlayStateInfo->GetSelectedCharacterSlot(item.Key);
// 			auto SkillEntComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(HeroSlot.SpawnedTargetActor);
// 			auto BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(HeroSlot.SpawnedTargetActor);
// 
// 			if (IsValid(SkillEntComp) && IsValid(BattleEntComp))
// 			{
// 				UCharacterBattleInfo* BattleInfo = BattleEntComp->GetBattleInfo();
// 				float AttackRange = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);
// 
// 				UISkill_V2* ActiveSkill = SkillEntComp->GetSkillByCategory_V2((ESkillCategoryTypeEnum)item.Value);
// 				if (ActiveSkill)
// 				{
// 					AActor* CurTarget = BattleEntComp->GetCurrentAttackTarget();
// 					if (!IsValid(CurTarget))
// 					{
// 						BattleEntComp->SetCurrentAttackTarget(URenewal_BattleFunctionLibrary::FindEnemy(HeroSlot.SpawnedTargetActor, HeroSlot.SpawnedTargetActor->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, AttackRange, EEnemySearchTargetTypeEnum::VE_Close));
// 						CurTarget = BattleEntComp->GetCurrentAttackTarget();
// 					}
// 
// 					if (CurTarget)
// 					{
// 						FVector Location = UMapFunctionLibrary::GetNavAgentLocation(CurTarget);
// 						SkillEntComp->CastInputSkill(ActiveSkill->GetSkillID(), HeroSlot.SpawnedTargetActor, CurTarget, Location);
// 					}
// 					else
// 					{
// 						UE_LOG(LogGamePlay, Log, TEXT("target empty"));
// // 						GameInstance->PlayStateInfo->SetVisibleAllCharacter(true);
// 					}
// 				}
// 			}
// 		}
// // 		else
// // 		{
// // 			GameInstance->PlayStateInfo->SetVisibleAllCharacter(true);
// // 		}
// 	}
// }

void UISkill_V2::DeleteContinuousActor()
{
	if (ContinuousApplierActor.IsValid())
	{
		ContinuousApplierActor->ClearSkillEffectForced();
		ContinuousApplierActor->DespawnSkill();
		ContinuousApplierActor.Reset();
	}
}

//int32 UISkill_V2::GetSkillUseCost()
//{
//	if (SkillInfo_V2)
//	{
//		return SkillInfo_V2->UseCost;
//	}
//	return 0;
//}

bool UISkill_V2::CanCastSkill(const bool bCheckCoolling)
{
	if (true == bCheckCoolling && IsSkillCooling())
		return false;

	if (ESkillStateEnum::VE_End != SkillState &&
		ESkillStateEnum::VE_None != SkillState)
		return false;

	/*if (CasterBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_state_change))
		return false;*/

	if (nullptr == SkillInfo_V2)
		return false;
	
	return true;
}

bool UISkill_V2::RTS_CanCastSkill()
{
	if (false == URealTimeModManager::IsRealTimeGameMode())
		return false;

	if (ESkillStateEnum::VE_End != SkillState &&
		ESkillStateEnum::VE_None != SkillState)
		return false;

	if (nullptr == SkillInfo_V2)
		return false;

	if (SkillSyncData.isCooling)
		return false;

	return true;
}

void UISkill_V2::GetApplyTarget()
{
	if (false == Caster.IsValid())
		return;

	if (false == bRealTimeMode)
	{
		auto SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster.Get());
		if (IsValid(SkillComp))
			SkillComp->GetSkillTarget(ApplyTargetActors, this);
	}
	else
	{
		if(SkillSyncData.IsValid())
		{
			int32 count = SkillSyncData.TargetApplyDatas.Num();
			ApplyTargetActors.Reset(count);
			for (int32 n = 0; n < count; ++n)
			{
				ApplyTargetActors.Emplace(SkillSyncData.TargetApplyDatas[n].Target);
			}
		}
	}
}

bool UISkill_V2::AttachCastingEffect(AActor* InTargetActor)
{
//	checkf(SkillResourceInfo != nullptr, TEXT("UISkill_V2::AttachCastingEffect[SkillResourceInfo Is Empty]"));
	if ( SkillResourceInfo != nullptr && SkillResourceInfo->CastingEffect != nullptr)
	{
		if (IsValid(InTargetActor) == false)
		{
			return false;
		}

		if (SkillResourceInfo->CastingEffectSocket.IsNone())
		{
			CastingEffect = URenewal_BattleFunctionLibrary::ParametersUsingParticle(UUtilFunctionLibrary::GetParticleSystemFromAssetID(SkillResourceInfo->CastingEffect), InTargetActor);
		}
		else if (SkillResourceInfo->CastingEffectSocket == SOCKET_FLOOR )
		{
			FVector FloorPos;
			if ( !UMapFunctionLibrary::GetProjectionToFloorHitBlockingVolume(InTargetActor, InTargetActor->GetActorLocation(), FloorPos))
			{
#if LOG_SKILL
				UE_LOG(LogBattle, Log, TEXT(" GetProjectionToFloorHitBlockingVolume() is false in Skill Casting Effect  Type Socket_Floor"));
#endif
				return false;
			}						
			
			CastingEffect = UUtilFunctionLibrary::CustomSpawnEmitterAtLocation(InTargetActor->GetWorld(), UUtilFunctionLibrary::GetParticleSystemFromAssetID(SkillResourceInfo->CastingEffect),FloorPos, InTargetActor->GetActorRotation());
		}
		else
		{
			UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InTargetActor);
			if (IsValid(TargetBattleEntityComponent) == false)
			{
				return false;
			}

			FName SocketName = SkillResourceInfo->CastingEffectSocket;
			if (!SocketName.IsNone())
				CastingEffect = UUtilFunctionLibrary::SpawnEmitterAttached(UUtilFunctionLibrary::GetParticleSystemFromAssetID(SkillResourceInfo->CastingEffect), InTargetActor, SocketName);
			else
				CastingEffect = UGameplayStatics::SpawnEmitterAtLocation(Caster->GetWorld(), UUtilFunctionLibrary::GetParticleSystemFromAssetID(SkillResourceInfo->CastingEffect), Caster->GetActorTransform());
		}
	}
	if (SkillResourceInfo != nullptr && SkillResourceInfo->CastingBeamEffect.IsValid() && Target.IsValid())
	{
		switch (SkillInfo_V2->TargetType)
		{
			case ESkillTargetTypeEnum::VE_Me:
			{
				ESearchShapeTypeEnum Shape = SkillInfo_V2->EffectShape;
				if (Shape == ESearchShapeTypeEnum::VE_Circle)
				{
					GetApplyTarget();

					for (size_t i = 0; i < ApplyTargetActors.Num(); ++i)
					{
						if (ApplyTargetActors.IsValidIndex(i) && ApplyTargetActors[i].IsValid())
						{
							FName SocketName = SkillResourceInfo->CastingEffectSocket;
							FName TargetSocketName = SkillResourceInfo->CastingBeamSocket;
							CastingBeamEffect = UUtilFunctionLibrary::SpawnEmitterAttached(UUtilFunctionLibrary::GetParticleSystemFromAssetID(SkillResourceInfo->CastingBeamEffect), ApplyTargetActors[i].Get(), TargetSocketName);
							if (CastingBeamEffect.IsValid())
							{
								FVector CasterSocketLocation = InTargetActor->GetActorLocation();
								TArray<UMeshComponent*> MeshComponentArray;
								InTargetActor->GetComponents(MeshComponentArray);
								for (auto Element : MeshComponentArray)
								{
									if (IsValid(Element))
									{
										if (Element->DoesSocketExist(SocketName))
										{
											CasterSocketLocation = Element->GetSocketLocation(SocketName);
											break;
										}
									}
								}
								CastingBeamEffect->SetBeamTargetPoint(0, CasterSocketLocation, 0);
							}
						}
					}
				}
			}
			break;

			case ESkillTargetTypeEnum::VE_Target:
			{
				FName SocketName = SkillResourceInfo->CastingEffectSocket;
				FName TargetSocketName = SkillResourceInfo->CastingBeamSocket;
				CastingBeamEffect = UUtilFunctionLibrary::SpawnEmitterAttached(UUtilFunctionLibrary::GetParticleSystemFromAssetID(SkillResourceInfo->CastingBeamEffect), Target.Get(), TargetSocketName);
				if (CastingBeamEffect.IsValid())
				{
					FVector CasterSocketLocation = InTargetActor->GetActorLocation();
					TArray<UMeshComponent*> MeshComponentArray;
					InTargetActor->GetComponents(MeshComponentArray);
					for (auto Element : MeshComponentArray)
					{
						if (IsValid(Element))
						{
							if (Element->DoesSocketExist(SocketName))
							{
								CasterSocketLocation = Element->GetSocketLocation(SocketName);
								break;
							}
						}
					}
					CastingBeamEffect->SetBeamTargetPoint(0, CasterSocketLocation, 0);
				}
			}
			break;
		}
	}
	return true;
}

bool UISkill_V2::AttachActionEffect(AActor* InTargetActor, ESkillStateEnum InActionEffectTiming)
{
//	checkf(SkillResourceInfo != nullptr, TEXT("UISkill_V2::AttachActionEffect[SkillResourceInfo Is Empty]"));
	if (SkillResourceInfo == nullptr)
		return false;

	bool IsFireActionEffect = ( SkillResourceInfo->ActionEffectTiming == InActionEffectTiming && !ActionEffect.IsValid());
	if (IsFireActionEffect)
	{
		FName SocketName = SkillResourceInfo->ActionEffectSocket;
// 		UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InTargetActor);
// 		if (IsValid(TargetBattleEntityComponent)
// 			&& TargetBattleEntityComponent->IsInSituationState(ECharacterSituationState::VE_Air)
// 			&& SocketName == SOCKET_ROOT)
// 		{
// 			USkillManager::ReNameSocketFly(SocketName);
// 		}

		ActionEffect = UUtilFunctionLibrary::SpawnEmitterAttached(UUtilFunctionLibrary::GetParticleSystemFromAssetID(SkillResourceInfo->ActionEffect), InTargetActor, SocketName);
		if (ActionEffect.IsValid())
		{
			ActionEffect->SetWorldScale3D(FVector(1, 1, 1));
		}

		

		return true;
	}
	return false;
}


bool UISkill_V2::AttachHitEffect(AActor* InTargetActor)
{
//	checkf(SkillResourceInfo != nullptr, TEXT("UISkill_V2::AttachHitEffect[SkillResourceInfo Is Empty]"));
	if (SkillResourceInfo == nullptr)
		return false;

	if (!SkillResourceInfo->HitEffect.IsNone())
	{
		if (Caster.IsValid())
		{
			/*Pointlight Settings*/
			
			/**/

			// BeamType HitEffect
			if (SkillResourceInfo->BeamTypeHitEffect)
			{
				FResourceFXTableInfo* info = UTableManager::GetInstancePtr()->GetResourceFXRow(SkillResourceInfo->HitEffect);
				if (info)
				{
					USkeletalMeshComponent* TargetActor_SkeletalMeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(InTargetActor);
					if (IsValid(TargetActor_SkeletalMeshComp))
					{
						HitEffect = UGameplayStatics::SpawnEmitterAttached(info->Particle.Get(), TargetActor_SkeletalMeshComp, SkillResourceInfo->HitEffectSocket, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
						if (HitEffect.IsValid())
						{
							//UE_LOG(LogBattle, Log, TEXT("SpawnFX %s"), FXKey);
							if (info->UseParentScale)
							{
								FVector SocketScale = TargetActor_SkeletalMeshComp->GetSocketTransform(SkillResourceInfo->HitEffectSocket).GetScale3D();
								HitEffect->SetWorldScale3D(SocketScale);
							}
							else
							{
								HitEffect->SetWorldScale3D(FVector(1, 1, 1));
							}
						}
					}

					if (info->Sound.IsValid())
					{
						USoundManager::GetInstancePtr()->PlayComputeSoundAtLocation(RGAMEINSTANCE(GEngine)->GetWorld(), 
							info->Sound.Get(), InTargetActor->GetActorLocation());
						//UGameplayStatics::PlaySoundAtLocation(RGAMEINSTANCE(GEngine)->GetWorld(), info->Sound.Get(), InTargetActor->GetActorLocation());
					}
					
				}

				if (HitEffect.IsValid())
				{
					FVector CasterSocketLocation = Caster->GetActorLocation();
					if (!SkillResourceInfo->BeamCasterSocket.IsNone())
					{
						TArray<UMeshComponent*> MeshComponentArray;
						Caster->GetComponents(MeshComponentArray);
						for (auto Element : MeshComponentArray)
						{
							if (IsValid(Element))
							{
								if (Element->DoesSocketExist(SkillResourceInfo->BeamCasterSocket))
								{
									CasterSocketLocation = Element->GetSocketLocation(SkillResourceInfo->BeamCasterSocket);
									break;
								}
							}
						}
					}
					HitEffect->SetBeamTargetPoint(0, CasterSocketLocation, 0);
				}
			}
			else
				UUtilFunctionLibrary::SpawnFX(HitEffect, SkillResourceInfo->HitEffect, InTargetActor, FVector::ZeroVector, FRotator::ZeroRotator, SkillResourceInfo->HitEffectSocket);
		}
		
		/*
		//FVector HitPoint = URenewal_BattleFunctionLibrary::FindHitPoint(this->Caster.Get(), InTargetActor);
		ABasePlayerController* PlayerCtrl = UUtilFunctionLibrary::GetBasePlayerController(InTargetActor->GetWorld());
		AActor* PlayerCtrlCharacter = PlayerCtrl->GetSelectedCharacter();

		auto RGameInstance = RGAMEINSTANCE(this);

		if (Caster.IsValid() && PlayerCtrlCharacter == Caster.Get())
		{
			HitEffect = UUtilFunctionLibrary::SpawnFX(SkillResourceInfo->HitEffect, InTargetActor, FVector::ZeroVector, FRotator::ZeroRotator, SkillResourceInfo->HitEffectSocket);
		}
		else
		{
			
			{
				HitEffect = UUtilFunctionLibrary::SpawnFX(RGameInstance->GameEnvironmentVar.CommonHitEffect, InTargetActor, FVector::ZeroVector, FRotator::ZeroRotator, SkillResourceInfo->HitEffectSocket);
			}
		}

		if (HitEffect.IsValid())
		{
			HitEffect->SetBeamTargetPoint(0, Caster->GetActorLocation(), 0);
		}
		*/
		
		return true;
	}
	return false;
}

bool UISkill_V2::AttachHitEffectLocation(AActor* InTargetActor, FVector InHitLocation)
{
//	checkf(SkillResourceInfo != nullptr, TEXT("UISkill_V2::AttachHitEffectLocation[SkillResourceInfo Is Empty]"));
	if (SkillResourceInfo == nullptr)
		return false;

	if (!SkillResourceInfo->HitEffect.IsNone())
	{
		ABasePlayerController* PlayerCtrl = UUtilFunctionLibrary::GetBasePlayerController(InTargetActor->GetWorld());
		AActor* PlayerCtrlCharacter = PlayerCtrl->GetSelectedCharacter();

		auto RGameInstance = RGAMEINSTANCE(this);

		if (Caster.IsValid() && PlayerCtrlCharacter == Caster.Get())
		{
			UUtilFunctionLibrary::SpawnFX(HitEffect, SkillResourceInfo->HitEffect, InTargetActor, InHitLocation, FRotator::ZeroRotator, TEXT(""));
		}
		else
		{
			
			{
				UUtilFunctionLibrary::SpawnFX(HitEffect, RGameInstance->GameEnvironmentVar.CommonHitEffect, InTargetActor, InHitLocation, FRotator::ZeroRotator, TEXT(""));
			}
		}

		return true;
	}
	return false;
}

bool UISkill_V2::AttachBulletHitEffect(AActor* InTargetActor)
{
//	checkf(SkillResourceInfo != nullptr, TEXT("UISkill_V2::AttachBulletHitEffect[SkillResourceInfo Is Empty]"));

	FBulletTableInfo* BulletTableData = UTableManager::GetInstancePtr()->GetBulletRow(SkillResourceInfo->BulletID);
	if (BulletTableData)
	{
		FVector HitPoint = URenewal_BattleFunctionLibrary::FindHitPoint(this->Caster.Get(), InTargetActor);

		ABasePlayerController* PlayerCtrl = UUtilFunctionLibrary::GetBasePlayerController(InTargetActor->GetWorld());
		AActor* PlayerCtrlCharacter = PlayerCtrl->GetSelectedCharacter();

		auto RGameInstance = RGAMEINSTANCE(this);
		if (!HitPoint.IsNearlyZero())
		{
			if (Caster.IsValid() && PlayerCtrlCharacter == Caster.Get())
			{
				UUtilFunctionLibrary::SpawnFX(BulletHitEffect,BulletTableData->BulletHitEffect, InTargetActor, HitPoint, FRotator::ZeroRotator, TEXT(""));
			}
			else if (RGameInstance)
			{
				 UUtilFunctionLibrary::SpawnFX(BulletHitEffect,RGameInstance->GameEnvironmentVar.CommonHitEffect, InTargetActor, HitPoint, FRotator::ZeroRotator, TEXT(""));
			}
		}

		return true;
	}
	return false;
}

bool UISkill_V2::AttachScreenEffect(AActor* InTargetActor, ESkillStateEnum InScreenEffectTiming)
{
//	checkf(SkillResourceInfo != nullptr, TEXT("UISkill_V2::AttachScreenEffect[SkillResourceInfo Is Empty]"));
	if (SkillResourceInfo == nullptr)
		return false;

	// ScreenEffect
	if (  SkillResourceInfo != nullptr &&  (SkillResourceInfo->ScreenEffect != nullptr) && (SkillResourceInfo->ScreenEffectTiming == InScreenEffectTiming))
	{
		auto RGameInstance = RGAMEINSTANCE(this);
		if ( IsValid(RGameInstance->HttpClient))
		{
			IEntityBaseProperty* TargetEntityBaseProperty = Cast< IEntityBaseProperty >(InTargetActor);
			if (TargetEntityBaseProperty == nullptr)
				return false;

			if (RGameInstance->HttpClient->IsValidKID(TargetEntityBaseProperty->GetUserKID()))
			{
				if (IsValid(RGameInstance->CameraActor))
				{
					RGameInstance->CameraActor->SetScreenEffect(UUtilFunctionLibrary::GetParticleSystemFromAssetID(SkillResourceInfo->ScreenEffect));
					return true;
				}
			}
		}
	}

	return false;
}

UParticleSystemComponent* UISkill_V2::GetHitEffect()
{
	return HitEffect.Get();
}

void UISkill_V2::ClearSkillEffectForced()
{
	DeleteContinuousActor();

	if (CastingEffect.IsValid())
	{
		CastingEffect->ActivateSystem(false);
		CastingEffect->DeactivateSystem();
		CastingEffect->KillParticlesForced();
		CastingEffect->DestroyComponent();
	}

	if (CastingBeamEffect.IsValid())
	{
		CastingBeamEffect->ActivateSystem(false);
		CastingBeamEffect->DeactivateSystem();
		CastingBeamEffect->KillParticlesForced();
		CastingBeamEffect->DestroyComponent();
	}

	if (ActionEffect.IsValid())
	{
		ActionEffect->ActivateSystem(false);
		ActionEffect->DeactivateSystem();
		ActionEffect->KillParticlesForced();
		ActionEffect->DestroyComponent();
	}

	if (BulletHitEffect.IsValid())
	{
		BulletHitEffect->ActivateSystem(false);
		BulletHitEffect->DeactivateSystem();
		BulletHitEffect->KillParticlesForced();
		BulletHitEffect->DestroyComponent();
	}

	if (HitEffect.IsValid())
	{
		HitEffect->ActivateSystem(false);
		HitEffect->DeactivateSystem();
		HitEffect->KillParticlesForced();
		HitEffect->DestroyComponent();
	}
}


void UISkill_V2::ShowUltimateSkillEffect()
{
//	checkf(SkillResourceInfo != nullptr, TEXT("UISkill_V2::ShowUltimateSkillEffect[SkillResourceInfo Is Empty]"));
	if (SkillResourceInfo == nullptr)
		return;

	if (SkillCategoryType == ESkillCategoryTypeEnum::VE_ActiveSkill03)
	{
		bool bPlayMatinee = UUtilFunctionLibrary::IsLeaderCharacter(Caster.Get());

		CasterAIEntityComponent->SetForcePauseBySkill(
			SkillResourceInfo->UltimateSkillSlowTime,
			SkillResourceInfo->UltimateSkillSlowRate,
			SkillResourceInfo->UltimatePostprocessTime,
			false);
	}
}


void UISkill_V2::StartCoolTime()
{
	if (IsValidCaster())
	{
		if (UUtilFunctionLibrary::IsCheatCoolTimeZero(Caster->GetWorld()))
		{
			return;
		}

		float ReduceCoolTime = CasterBattleEntityComponent->GetBattleInfo()->GetAvatarAbilityProperty(EAvatarAbilityType::VE_ReduceCoolTime);
		if (SkillInfo_V2 == nullptr)
			return;
		float SkillCoolTime = SkillInfo_V2->CoolTime - ReduceCoolTime;
		if (0.0f < SkillCoolTime)
		{
			if (SkillCoolTime > CurrentCoolTime)
			{
				CurrentCoolTime = SkillCoolTime;
				/*SyncData*/
				if(bRealTimeMode)
					SkillSyncData.isCooling = true;

				/*if this skill is active3, apply global cooltime*/
				if (SkillCategoryType == ESkillCategoryTypeEnum::VE_ActiveSkill03)
				{
					IEntityBaseProperty* Prop = Cast<IEntityBaseProperty>(Caster.Get());
					if (Prop && Prop->GetPartyID() != PartyID_PC_MY)
						return;

					TArray<FHeroSlotInfo> HeroSlotArray = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetSelectedCharacterArray();
					for (size_t i = 0; i < HeroSlotArray.Num(); ++i)
					{
						if (!HeroSlotArray[i].SpawnedTargetActor)
							continue;

						if (HeroSlotArray[i].SpawnedTargetActor == Caster)
							continue;

						USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(HeroSlotArray[i].SpawnedTargetActor);
						if (!SkillComp)
							continue;

						UISkill_V2* Skill = SkillComp->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill03);
						if (!Skill)
							continue;

						if (Skill->GetCurrentCoolTime() <= 0)
							Skill->IncreaseCoolTime(GLOBAL_ACTIVE3_COOLTIME - Skill->GetCurrentCoolTime());
							
					}
				}
			}
		}
		else
		{
			if (SKILL_FIRECOOLTIME > CurrentCoolTime)
			{
				CurrentCoolTime = SKILL_FIRECOOLTIME;
			}
		}
	}
}

void UISkill_V2::StartCrewSkillCoolTime()
{
	if (IsValidCaster())
	{
		if (UUtilFunctionLibrary::IsCheatCoolTimeZero(Caster->GetWorld()))
		{
			return;
		}

		float SkillCoolTime = SkillInfo_V2->CrewCoolTime;
		if (0.0f < SkillCoolTime)
		{
			if (SkillCoolTime > CurrentCrewSkillCoolTime)
			{
				CurrentCrewSkillCoolTime = SkillCoolTime;
			}
		}
	}
}

void UISkill_V2::CancelCasting()
{
	if (IsValidCaster())
	{
		CasterSkillEntityComponent->SetSkillCasting(false);
		CasterSkillEntityComponent->SetSkillActing(false);
	}
	SkillState = ESkillStateEnum::VE_None;
}

void UISkill_V2::UpdateCooling(float DeltaTime)
{
	if (0.0f < CurrentCoolTime)
	{
		CurrentCoolTime -= DeltaTime;
		if (0.0f > CurrentCoolTime)
		{
			CurrentCoolTime = 0.0f;
		}
	}
}

void UISkill_V2::UpdateCrewSkillCooling(float DeltaTime)
{
	if (0.0f < CurrentCrewSkillCoolTime)
	{
		CurrentCrewSkillCoolTime -= DeltaTime;
		if (CurrentCrewSkillCoolTime <= 0.0f)
		{
			CurrentCrewSkillCoolTime = 0.0f;
		}
	}
}

bool UISkill_V2::IsValidCaster()
{
	return (true == (Caster.IsValid()
		&& CasterBattleEntityComponent.IsValid()
		&& CasterSkillEntityComponent.IsValid()
		&& CasterAIEntityComponent.IsValid()
		&& CasterBattleInfoBlackboard.IsValid()));
}

ESkillAttributeEnum UISkill_V2::GetSkillAttributeType()
{
	if (SkillInfo_V2)
	{
		return SkillInfo_V2->SkillAttribute;
	}
	return ESkillAttributeEnum::VE_None;
}

void UISkill_V2::DecreaseCoolTime(float InDecreaseCoolTime)
{
	CurrentCoolTime -= InDecreaseCoolTime;
	if (CurrentCoolTime < 0.0f)
		CurrentCoolTime = 0.0f;
}

void UISkill_V2::IncreaseCoolTime(float InIncreaseCoolTime)
{
	CurrentCoolTime += InIncreaseCoolTime;
}

void UISkill_V2::GetAppliedTargetSkillComponentList(TArray<USkillEntityComponent*> &TargetSkillComponentList)
{
	USkillEntityComponent* Target_SkillEntityComponent = nullptr;
	for (auto& ApplyTarget : ApplyTargetActors)
	{
		auto OwnerBattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(ApplyTarget.Get());
		if (IsValid(OwnerBattleEntityComp))
		{
			if (false == OwnerBattleEntityComp->IsDead())
			{
				Target_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(ApplyTarget.Get());
				if (IsValid(Target_SkillEntityComponent))
				{
					TargetSkillComponentList.Emplace(Target_SkillEntityComponent);
				}
			}
		}
	}
	ApplyTargetActors.Empty();
}


void UISkill_V2::CacheCasterComponent()
{
	CasterBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Caster.Get());
	CasterSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster.Get());
	CasterEntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Caster.Get());
	CasterAIEntityComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Caster.Get());

	if (CasterBattleEntityComponent.IsValid())
	{
		CasterBattleInfoBlackboard = CasterBattleEntityComponent->GetBattleInfoBlackboard();
	}
}

void UISkill_V2::SetSkillSyncData(const FSkillSyncApplyData& CasterData, const TArray<FSkillSyncApplyData>& TargetDataList)
{
	SkillSyncData.ValidData = true;
	SkillSyncData.CasterApplyData = CasterData;
	SkillSyncData.TargetApplyDatas = TargetDataList;
}

FSkillSyncApplyData* UISkill_V2::GetTargetSkillSyncData(AActor* TargetActor)
{
	if (!IsValid(TargetActor))
		return nullptr;

	if (false == SkillSyncData.IsValid())
		return nullptr;

	if (0 >= SkillSyncData.TargetApplyDatas.Num())
		return nullptr;

	for (auto& ApplyData : SkillSyncData.TargetApplyDatas)
	{
		if (ApplyData.Target.IsValid() && TargetActor == ApplyData.Target.Get())
		{
			return &ApplyData;
		}
	}
	return nullptr;
}

void UISkill_V2::ForceApplySkillSyncData()
{
	if (SkillSyncData.IsValid())
	{
	}

	SkillSyncData.Clear();
}


/*
Summon..
*/
bool UISkill_V2::SummonMinion(FSkillApplierBase& InSkillApplier)
{
	IEntityBaseProperty* CasterBaseProperty = Cast<IEntityBaseProperty>(Caster.Get());
	if (CasterBaseProperty == nullptr)
		return false;

	if (CasterBaseProperty->GetEntityType() == EEntityTypeEnum::VE_RemoteEntity)
		return false;

	
	FName TargetMinionID;
	int32 MaxSummonCount = 0;
	int32 SummonCount = 0;
	switch (InSkillApplier.SkillApplierType)
	{
	case ESkillApplierTypeEnum::VE_i_summon_1Max1:
		MaxSummonCount = 1;
		SummonCount = 1;
		break;
	case ESkillApplierTypeEnum::VE_i_summon_1Max2:
		MaxSummonCount = 2;
		SummonCount = 1;
		break;
	case ESkillApplierTypeEnum::VE_i_summon_1Max3:
		MaxSummonCount = 3;
		SummonCount = 1;
		break;
	case ESkillApplierTypeEnum::VE_i_summon_1Max4:
		MaxSummonCount = 4;
		SummonCount = 1;
		break;
	case ESkillApplierTypeEnum::VE_i_summon_1Max5:
		MaxSummonCount = 5;
		SummonCount = 1;
		break;
	case ESkillApplierTypeEnum::VE_i_summon_Once2:
		MaxSummonCount = 2;
		SummonCount = 2;
		break;
	case ESkillApplierTypeEnum::VE_i_summon_Once3:
		MaxSummonCount = 3;
		SummonCount = 3;
		break;
	case ESkillApplierTypeEnum::VE_i_summon_Once4:
		MaxSummonCount = 4;
		SummonCount = 4;
		break;
	case ESkillApplierTypeEnum::VE_i_summon_Once5:
		MaxSummonCount = 5;
		SummonCount = 5;
		break;
	}

	TargetMinionID = *(FString::Printf( TEXT("%d"), InSkillApplier.TargetValue));
	if (MaxSummonCount <= 0)
		return false;

	bool bResult = false;
	
	TArray< FVector >	OutCellIndics;
	GetSummonLocation( TargetLocation, OutCellIndics, SummonCount );

	FVector SpawnLocation;
	for (int32 i = 0; i < SummonCount; i++)
	{
		if (OutCellIndics.IsValidIndex(i))
			SpawnLocation = OutCellIndics[i];
		else
			SpawnLocation = Caster->GetActorLocation();

		AActor* SummonedMinion = SummonCharacter(TargetMinionID, SpawnLocation);

		// Set minion property
		ApplySummonedMinionProperty(SummonedMinion, InSkillApplier.TargetPercent);

		bResult = true;
	}

	// Already Summon Count Check
	TArray< TWeakObjectPtr< AActor > > SummonedMinions = CasterBattleEntityComponent->GetBattleInfo()->GetSummonedMinions();
	TArray< TWeakObjectPtr< AActor > > AlreadySpawnedMinions;
	for (auto Element : SummonedMinions)
	{
		if (Element.IsValid() == false)
			continue;

		IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(Element.Get());
		if (BaseProperty == nullptr)
			continue;

		if (TargetMinionID.IsEqual(BaseProperty->GetCharacterID()))
		{
			AlreadySpawnedMinions.Emplace(Element);
		}
	}

	// Delete Already Summoned Monion
	int32 DeleteCount = AlreadySpawnedMinions.Num() - MaxSummonCount;
	while (0 < DeleteCount)
	{
		auto Element = AlreadySpawnedMinions.Pop();
		if (Element.IsValid())
		{
			auto Summoned_EntityBattleComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Element.Get());
			if (Summoned_EntityBattleComponent)
			{
				Summoned_EntityBattleComponent->GetBattleInfo()->SetHP(0);
				Summoned_EntityBattleComponent->ForceDead(Caster.Get());
			}
		}

		--DeleteCount;
	}

	return bResult;
}

void UISkill_V2::GetSummonLocation(FVector InTargetLocation, TArray< FVector >& OutResult, int32 InRequestCount)
{
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) == false)
// 		return;

	if (IsValid(RGameInstance->GridMapManager) == false)
		return;

	bool bInTargetLocationValid = false;
	FVector SpawnLocation = Caster->GetActorLocation();
	FMapCellData CellData = RGameInstance->GridMapManager->GetMapInfoFromWorldPos(InTargetLocation);
	if (EMapCellStateEnum::VE_Movable == CellData.CellInfo)
	{
		SpawnLocation = InTargetLocation;
		if (1 >= InRequestCount)
		{
			OutResult.Emplace(SpawnLocation);
			return;
		}
	}

	FCellIndex TargetCellIndex;
	if (RGameInstance->GridMapManager->GetCellIndexFromWorldPos(InTargetLocation, TargetCellIndex))
	{
		FVector TargetCellWorldPos;
		RGameInstance->GridMapManager->GetWorldPosFromCellIndex(TargetCellIndex, TargetCellWorldPos);
		FVector PosOffSet = InTargetLocation - TargetCellWorldPos;

		TArray<FCellIndex> FindCells;
		int32 CellDepthForDistance = RGameInstance->GridMapManager->GetCellDepthFromDistance(SUMMON_SPAWN_SEARCH_DEPTH_DIST);
		int32 InSearchDepth = 0;
		for (int32 j = 0; j <= CellDepthForDistance; ++j)
		{
			InSearchDepth += ENUM_TO_BITFIELD(j);
		}

		RGameInstance->GridMapManager->GetAdjacentCells(FindCells, TargetCellIndex, InSearchDepth, ENUM_TO_BITFIELD((int32)EMapCellStateEnum::VE_Movable), true);
		int32 FindCellNum = FindCells.Num();
		if (0 < FindCellNum)
		{
			TArray<int32> CellDistCost;
			for (auto find_cell : FindCells)
			{
				CellDistCost.Emplace(find_cell.GetCellDistCost(TargetCellIndex));
			}

			int32 swap_dist = 0;
			FCellIndex swap_cell;
			for (int32 n = 0; n < FindCellNum; ++n)
			{
				for (int32 x = n + 1; x < FindCellNum; ++x)
				{
					if (CellDistCost[n] > CellDistCost[x])
					{
						swap_dist = CellDistCost[n];
						CellDistCost[n] = CellDistCost[x];
						CellDistCost[x] = swap_dist;

						swap_cell = FindCells[n];
						FindCells[n] = FindCells[x];
						FindCells[x] = swap_cell;
					}
				}
			}

			for (int32 i = 0; i < InRequestCount; ++i)
			{
				if (FindCells.IsValidIndex(i))
				{
					RGameInstance->GridMapManager->GetWorldPosFromCellIndex(FindCells[i], SpawnLocation);
					SpawnLocation = SpawnLocation + PosOffSet;

					if (UMapFunctionLibrary::CanSpawnPostion(RGameInstance, SpawnLocation) == false)
					{
						FVector AdjacentPos = Caster->GetActorLocation();
						if (UMapFunctionLibrary::GetAdjacentPosNearestWithTargetPositionOnNavyMesh(AdjacentPos, SpawnLocation, 1, 3))
						{
							SpawnLocation = AdjacentPos;
						}
					}
					OutResult.Emplace(SpawnLocation);
				}
			}
		}
		else
		{
			for (int32 i = 0; i < InRequestCount; ++i)
			{
				OutResult.Emplace(SpawnLocation);
			}
		}
	}
}

AActor* UISkill_V2::SummonCharacter(FName InSummonNPCID, FVector& SpawnLocation)
{
	IEntityBaseProperty* CasterBaseProperty = Cast<IEntityBaseProperty>(Caster.Get());
	if (CasterBaseProperty == nullptr)
		return nullptr;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);

	AActor* Minion = UObjectFunctionLibrary::SpawnCharacter(SpawnTransform, InSummonNPCID, ECharacterCategoryTypeEnum::VE_Minion,
		CasterBaseProperty->GetUserKID(), FString(""), CasterBaseProperty->GetGroupKey(), CasterBaseProperty->GetPartyID(), PLAYER_UK_NONE,
		CasterBaseProperty->GetLevel());

	if (!IsValid(Minion))
		return nullptr;

	UBattleEntityComponent* TargetBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Minion);
	if (IsValid(TargetBattleEntityComponent))
	{
		TargetBattleEntityComponent->SetSummonOwner(Caster.Get());
		TargetBattleEntityComponent->SetCurrentAttackTarget(CasterBattleEntityComponent->GetCurrentAttackTarget());
	}
		
	CasterBattleEntityComponent->GetBattleInfo()->AddSummonedMinion(Minion);

	return Minion;
}


void UISkill_V2::ApplySummonedMinionProperty(AActor* InMinion, float InPropertyPercent)
{
	TArray< ECharacterPropertyTypeEnum > ShouldBeModifiedValues =
	{ ECharacterPropertyTypeEnum::VE_MaxHP,
		ECharacterPropertyTypeEnum::VE_PhysicalAttackPower,
		ECharacterPropertyTypeEnum::VE_MagicAttackPower,
		ECharacterPropertyTypeEnum::VE_Armor,
		ECharacterPropertyTypeEnum::VE_MagicResistance };

	auto MinionBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InMinion);
	if (false == IsValid(MinionBattleEntityComponent))
		return;

	float ComputePercent = InPropertyPercent * 0.01f;
	for (auto PropertyEnum : ShouldBeModifiedValues)
	{
		int32 MinionProeprty = MinionBattleEntityComponent->GetBattleInfo()->GetCharacterProperty< int32 >(PropertyEnum);
		if (MinionProeprty < 0)
		{
			int32 OwnerProperty = CasterBattleEntityComponent->GetBattleInfo()->GetCharacterNativeTotalProperty< int32 >(PropertyEnum);
			//int32 ModifiedProperty = OwnerProperty * ((1.0f + InPropertyPercent) / 100.0f);
			//int32 ModifiedProperty = OwnerProperty * (InPropertyPercent / 100.0f);
			int32 ModifiedProperty = OwnerProperty * ComputePercent;

			switch (PropertyEnum)
			{
			case ECharacterPropertyTypeEnum::VE_MaxHP:
				MinionBattleEntityComponent->GetBattleInfo()->CharacterProperty.MaxHP = ModifiedProperty;
				MinionBattleEntityComponent->GetBattleInfo()->SetHP(ModifiedProperty);
				break;
			case ECharacterPropertyTypeEnum::VE_PhysicalAttackPower:
				MinionBattleEntityComponent->GetBattleInfo()->CharacterProperty.PhysicalAttackPower = ModifiedProperty;
				break;
			case ECharacterPropertyTypeEnum::VE_MagicAttackPower:
				MinionBattleEntityComponent->GetBattleInfo()->CharacterProperty.MagicAttackPower = ModifiedProperty;
				break;
			case ECharacterPropertyTypeEnum::VE_Armor:
				MinionBattleEntityComponent->GetBattleInfo()->CharacterProperty.Armor = ModifiedProperty;
				break;
			case ECharacterPropertyTypeEnum::VE_MagicResistance:
				MinionBattleEntityComponent->GetBattleInfo()->CharacterProperty.MagicResistance = ModifiedProperty;
				break;
			}
		}
	}
}