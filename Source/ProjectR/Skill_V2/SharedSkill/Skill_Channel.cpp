// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "Skill_Channel.h"

#include "GlobalIntegrated.h"
#include "Skill_V2/ContinuousSkillApplierActor.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSpriteEmitter.h"
#include "Character/BattleBaseAnimInstance.h"

#include "UtilFunctionIntegrated.h"


const float CHANNELING_MINIMUM_CASTING_TIME = 0.1f;


void USkill_Channel::InitSkill(FName InSkillID, FName InSkillTableID, AActor* InCaster, int32 InLevel, FSkillTableInfo_V2* SkillInfo)
{
	checkf(InCaster != nullptr, TEXT("USkill_Channel::InitSkill[InCaster Is Empty]"));
// 	if (InCaster == nullptr)
// 		return;

	if (nullptr == SkillInfo)
		return;

	UISkill_V2::InitSkill(InSkillID, InSkillTableID, InCaster, InLevel, SkillInfo);

	ChannelingElapsedTime = 0.0f;
	ChannelingDurationTime = SkillInfo->CastingTime;
	IntervalElapsedTime = 0.0f;
	IntervalTime = SkillInfo->Interval;
}


void USkill_Channel::Update(float DeltaTime)
{
	if (IsValidCaster())
	{
		if (CasterSkillEntityComponent->GetCurrentSkill_V2() == this)
		{
			if (ESkillStateEnum::VE_Casting == SkillState || ESkillStateEnum::VE_DoAction == SkillState)
			{
				ChannelingElapsedTime += DeltaTime;

				if (ESkillStateEnum::VE_Casting == SkillState &&
					CHANNELING_MINIMUM_CASTING_TIME < ChannelingElapsedTime)
				{
					SkillState = ESkillStateEnum::VE_DoAction;
				}

				if (false == bRealTimeMode)
				{
					UpdateCasterDirection();
					if (CheckAimTarget())
					{
						if (ChannelingElapsedTime < ChannelingDurationTime)
						{
							IntervalElapsedTime += DeltaTime;
							if (IntervalElapsedTime >= IntervalTime)
							{
								IntervalElapsedTime = IntervalElapsedTime - IntervalTime;
								Fire();
							}

							if (false == CheckCasterHoldOn())
							{
								CasterSkillEntityComponent->EndSkillActionForced_V2();
							}
						}
						else
						{
							CasterSkillEntityComponent->EndSkillAction_V2();
						}
					}
					else
					{
						CasterSkillEntityComponent->EndSkillActionForced_V2();
					}
				}
				else
				{
					if (CheckCasterHoldOn())
					{
						UpdateCasterDirection();
					}

					if (ChannelingElapsedTime >= ChannelingDurationTime || ReservedSkillEnd)
					{
						CasterSkillEntityComponent->EndSkillAction_V2();
					}
				}
			}
		}

		UpdateCooling(DeltaTime);

		if (CasterSkillEntityComponent->IsSkillCasting() && CasterSkillEntityComponent->GetCurrentSkill_V2() == this)
		{
			if (false == bRealTimeMode)
			{
				UpdateCasterDirection();
				if (CheckAimTarget())
				{
					ChannelingElapsedTime += DeltaTime;
					if (ChannelingElapsedTime < ChannelingDurationTime)
					{
						IntervalElapsedTime += DeltaTime;
						if (IntervalElapsedTime >= IntervalTime)
						{
							IntervalElapsedTime = IntervalElapsedTime - IntervalTime;
							Fire();
						}

						if (false == CheckCasterHoldOn())
						{
							CasterSkillEntityComponent->EndSkillActionForced_V2();
						}
					}
					else
					{
						CasterSkillEntityComponent->EndSkillAction_V2();
					}
				}
				else
				{
					CasterSkillEntityComponent->EndSkillActionForced_V2();
				}
			}
			else
			{
				if (CheckCasterHoldOn())
				{
					UpdateCasterDirection();
				}

				ChannelingElapsedTime += DeltaTime;
				if (ChannelingElapsedTime >= ChannelingDurationTime || ReservedSkillEnd)
				{
					CasterSkillEntityComponent->EndSkillAction_V2();
				}
			}
		}

		UpdateCooling(DeltaTime);
	}
}


bool USkill_Channel::Casting(AActor* InTarget, const FVector& InTargetLocation)
{
	if (Super::Casting(InTarget, InTargetLocation))
	{
		//UE_LOG(LogBattle, Log, TEXT("=====>> USkill_Channel : Casting"));

		CasterCastingPos = Caster->GetActorLocation();

		DoAction();

		CreateAreaApplierActor();

		if (EAbnormalSlotTypeEnum::VE_None != SkillInfo_V2->CasterSlotType)
		{
			CasterSkillEntityComponent->ApplySkill(this, Caster.Get(), true);
		}
		return true;
	}
	return false;
}

void USkill_Channel::DoAction()
{
	//SkillState = ESkillStateEnum::VE_Channeling;

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

	auto OwnerActor_AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(Caster.Get());
	if (IsValid(OwnerActor_AnimStateComponent))
	{
		FVariantData SkillData = FVariantData((int32)SkillResourceInfo->SkillAniType);
		OwnerActor_AnimStateComponent->DoCommand(ECharacterCommandEnum::VE_ActionSkill, SkillData);
	}

	//UE_LOG(LogBattle, Log, TEXT("=====>> USkill_Channel : DoAction"));
}

bool USkill_Channel::Fire()
{
	if (IsValidCaster() == false)
		return false;

	GetApplyTarget();

	if (0 < ApplyTargetActors.Num() )
	{
		Apply();
	}

	AttachActionEffect(Caster.Get(), ESkillStateEnum::VE_Fire);
	AttachScreenEffect(Caster.Get(), ESkillStateEnum::VE_Fire);

	// Not needed target skill apply.. ex) summon
	USkillEntityComponent* SkillComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Caster.Get());
	if (!IsValid(SkillComponent))
		return false;
	SkillComponent->ApplyInstanceApplierGroup(Caster.Get(), Target.Get(), SkillInfo_V2, false, this, true);
	
	//UE_LOG(LogBattle, Log, TEXT("=====>> USkill_Channel : Fire"));

	return true;
}

void USkill_Channel::End()
{
	StartCoolTime();

	UISkill_V2::End();

	DeleteContinuousActor();

	ChannelingElapsedTime = 0.0f;
	IntervalElapsedTime = 0.0f;

	//UE_LOG(LogBattle, Log, TEXT("=====>> USkill_Channel : End"));
}

bool USkill_Channel::CheckCasterHoldOn()
{
	if (IsValidCaster())
	{
		FVector MoveVec = Caster->GetActorLocation() - CasterCastingPos;
		if (MoveVec.IsNearlyZero(100.0f))
		{
			return true;
		}
	}
	return false;
}

//void USkill_Channel::StartCoolTime()
//{
//	if (IsValidCaster())
//	{
//		if (UUtilFunctionLibrary::IsCheatCoolTimeZero(Caster->GetWorld()))
//		{
//			return;
//		}
//
//		if (ESkillAniTypeEnum::VE_None != SkillResourceInfo->SkillAniType || 0.0f < SkillInfo_V2->CoolTime)
//		{
//			if (SkillInfo_V2->CoolTime > CurrentCoolTime)
//			{
//				if (ChannelingElapsedTime >= ChannelingDurationTime)
//				{
//					ChannelingElapsedTime = ChannelingDurationTime;
//				}
//
//				float SkillCoolTime = SkillInfo_V2->CoolTime * (ChannelingElapsedTime / ChannelingDurationTime);			
//				float ReduceCoolTime = CasterBattleEntityComponent->GetBattleInfo()->GetAvatarAbilityProperty(EAvatarAbilityType::VE_ReduceCoolTime);
//
//				SkillCoolTime = SkillCoolTime - ReduceCoolTime;
//				if (0.0f < SkillCoolTime)
//				{
//					CurrentCoolTime = SkillCoolTime;
//				}
//			}
//		}
//		else
//		{
//			if (SKILL_FIRECOOLTIME > CurrentCoolTime)
//			{
//				CurrentCoolTime = SKILL_FIRECOOLTIME;
//			}
//		}
//	}
//}

void USkill_Channel::UpdateCasterDirection()
{
	checkf(SkillInfo_V2 != nullptr, TEXT("USkill_Channel::UpdateCasterDirection[SkillInfo_V2 Is Empty]"));

	if (SkillInfo_V2 && Target.IsValid())
	{
		if (ESkillTargetTypeEnum::VE_Target == SkillInfo_V2->TargetType &&
			ESearchShapeTypeEnum::VE_None == SkillInfo_V2->EffectShape)
		{
			FVector TargetLoc = Target->GetActorLocation();
			FVector CasterLoc = Caster->GetActorLocation();

			FVector Dir = TargetLoc - CasterLoc;
			Dir.Z = 0;
			Dir.Normalize();

			FRotator Rot = FRotationMatrix::MakeFromX(Dir).Rotator();

			Caster->SetActorRotation(Rot);
		}
	}
}

bool USkill_Channel::CheckAimTarget()
{
	checkf(SkillInfo_V2 != nullptr, TEXT("USkill_Channel::CheckAimTarget[SkillInfo_V2 Is Empty]"));
	if (SkillInfo_V2 && Target.IsValid())
	{
		if (ESkillTargetTypeEnum::VE_Target == SkillInfo_V2->TargetType &&
			ESearchShapeTypeEnum::VE_None == SkillInfo_V2->EffectShape)
		{
			if (false == CasterBattleEntityComponent->IsTargetInRange(Target.Get(), SkillInfo_V2->CastingRange))
			{
				return false;
			}
		}
	}
	return true;
}

void USkill_Channel::CreateAreaApplierActor()
{
	checkf(SkillInfo_V2 != nullptr, TEXT("USkill_Channel::CreateAreaApplierActor[SkillInfo_V2 Is Empty]"));
	if (SkillInfo_V2)
	{
		if (ESearchShapeTypeEnum::VE_None == SkillInfo_V2->EffectShape)
			return;

		AActor* AttachedTarget = nullptr;
		FVector SpawnLoc = FVector::ZeroVector;
		if (ESkillTargetTypeEnum::VE_TargetLocation == SkillInfo_V2->TargetType)
		{
			SpawnLoc = TargetLocation;
		}
		else if (ESkillTargetTypeEnum::VE_Target == SkillInfo_V2->TargetType)
		{
			if (Target.IsValid())
			{
				AttachedTarget = Target.Get();
				SpawnLoc = Target->GetActorLocation();
			}
		}
		else if (ESkillTargetTypeEnum::VE_Me == SkillInfo_V2->TargetType)
		{
			if (Caster.IsValid())
			{
				AttachedTarget = Caster.Get();
				SpawnLoc = Caster->GetActorLocation();
			}
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Caster.Get();
		SpawnParams.Instigator = Cast< APawn >(Caster.Get());

		AContinuousSkillApplierActor *ApplierActor = Caster->GetWorld()->SpawnActor< AContinuousSkillApplierActor >(AContinuousSkillApplierActor::StaticClass(), SpawnLoc, FRotator::ZeroRotator, SpawnParams);
		if (ApplierActor)
		{
			ApplierActor->SetAttachedTarget(AttachedTarget);
			ApplierActor->SpawnSkill(this, SpawnLoc);
			ApplierActor->SetUsingOnlyEffect(true);
			ApplierActor->SetEspecialDurationTime(ChannelingDurationTime);

			ContinuousApplierActor = ApplierActor;

			//UE_LOG(LogBattle, Log, TEXT("=====>> USkill_Channel : CreateAreaApplierActor"));
		}
	}
}
