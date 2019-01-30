// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SharedConstants/GlobalConstants.h"
#include "BattleOperator.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/DamageFloater/SlateDamageFloaterComponentV2.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/SharedBattle/BattleInfoBlackboard.h"
#include "Character/BattleBaseCharacter.h"

#include "Skill_V2/SharedSkill/SkillSlotContainer.h"
#include "Skill_V2/SharedSkill/SkillPassiveSlotContainer.h"



void UBattleOperator::AttachProtect(FProtectInfo& ProtectInfo)
{	
	if (OwnerBattleEntityComponent == nullptr)
		return;

	if (ProtectInfo.ProtectType == EProtectType::VE_Immune )
	{
	}
	
	bool IsAdd = true;
	for (auto& Element : ProtectArray)
	{
		if (Element.ProtectType == ProtectInfo.ProtectType)
		{
			Element.AttackType = ProtectInfo.AttackType;
			Element.ProtectElapsedTime = ProtectInfo.ProtectElapsedTime;

			if (ProtectInfo.ProtectPower > Element.ProtectPower)
				Element.ProtectPower = ProtectInfo.ProtectPower;

			if (ProtectInfo.ProtectTime > Element.ProtectTime)
				Element.ProtectTime = ProtectInfo.ProtectTime;

			IsAdd = false;
			break;
		}
	}
	
	if (IsAdd)
	{
		ProtectInfo.ProtectPowerMax = ProtectInfo.ProtectPower;
		ProtectArray.Emplace(ProtectInfo);
		
		//order of priority
		ProtectArray.Sort([](const FProtectInfo& First, const FProtectInfo& Second){
			return First.ProtectType < Second.ProtectType;
		});
	}
	else
	{
		//buff delete
	}
}

void UBattleOperator::DetachProtect(EProtectType ProtectType, bool InIsTimeOut)
{	
	if (OwnerBattleEntityComponent == nullptr)
		return;

	for (auto& Element : ProtectArray)
	{
		if (Element.ProtectType == ProtectType)
		{
			if (Element.ProtectType == EProtectType::VE_HP)
			{
				if (IsValid(OwnerBattleEntityComponent->BossComponent))
				{
					URGameInstance* RGameInstance = RGAMEINSTANCE(this);
					if ( IsValid(RGameInstance->EventManager))
					{
						RGameInstance->EventManager->OnBossShieldNotify.Broadcast(0, InIsTimeOut);
					}
				}
			}
			break;
		}
	}
	
	ProtectArray.RemoveAll([&](const FProtectInfo ProtectElement){
		return ( ProtectElement.ProtectType == ProtectType);
	});
}

bool UBattleOperator::IsApplyProtect(EProtectType ProtectType)
{
	bool Result = false;
	for (auto& Element : ProtectArray)
	{
		if (Element.ProtectType == ProtectType)
		{
			Result = true;
			break;
		}
	}

	return Result;
}

void UBattleOperator::SetOwner(AActor* Owner)
{
	this->OwnerCharacter = Owner;

	OwnerBattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent( OwnerCharacter.Get() );
	OwnerSkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent( OwnerCharacter.Get() );
}



float UBattleOperator::CheckProtectState(float Damage, AActor* FromActor )
{	
	if (OwnerBattleEntityComponent.IsValid() == false )
		return Damage;

	if (IsValid(OwnerBattleEntityComponent->GetBattleInfoBlackboard()) == false)
		return Damage;

	bool OnBuffCheckPoint = false;

	TArray< ESkillApplierTypeEnum > RemoveProtectApplierType;
	RemoveProtectApplierType.Empty();

	for (auto& Element : ProtectArray)
	{
		if (Damage == 0)
			break;

		switch (Element.ProtectType)
		{
			case EProtectType::VE_HP:
			{
				if (Element.ProtectPower > Damage)
				{
					Element.ProtectPower -= Damage;
					Damage = 0;
				}
				else /* if (Element->ProtectPower <= Damage)*/
				{
					OnBuffCheckPoint = true;
					Damage -= Element.ProtectPower;				
					Element.ProtectType = EProtectType::VE_None;
					RemoveProtectApplierType.Emplace(Element.SkillApplierType);
				}

				if (IsValid(OwnerBattleEntityComponent->BossComponent))
				{
					URGameInstance* RGameInstance = RGAMEINSTANCE(this);
					if ( IsValid(RGameInstance->EventManager))
					{
						RGameInstance->EventManager->OnBossShieldNotify.Broadcast(GetProtectPowerPrecent(EProtectType::VE_HP), false);
					}
				}
			}
			break;
			case EProtectType::VE_Count:
			{
				if (Element.ProtectPower > 0)
				{
					--Element.ProtectPower;
					Damage = 0;
				}
				if (Element.ProtectPower <= 0)
				{
					OnBuffCheckPoint = true;
					Element.ProtectType = EProtectType::VE_None;

					RemoveProtectApplierType.Emplace(Element.SkillApplierType);
				}
			}
			break;
			case EProtectType::VE_ImmuneRange:
			{
				EAttackTypeEnum CheckType = OwnerBattleEntityComponent->GetBattleInfoBlackboard()->LastAttackType;//OwnerCharacter->GetBlackBoardData<EAttackTypeEnum>(EBattleInfoPropertyType::VE_LastAttackType);
				if (CheckType == Element.AttackType)
				{
					Damage = 0;
					//OwnerCharacter->SetBlackBoardData(EBattleInfoPropertyType::VE_LastDamageResultType, EBattleDamageResultEnum::VE_Immune);
					OwnerBattleEntityComponent->GetBattleInfoBlackboard()->LastDamageResultType = EBattleDamageResultEnum::VE_Immune;
				}				
			}
			break;
			case EProtectType::VE_Immune:
			{
				Damage = 0;
				OwnerBattleEntityComponent->GetBattleInfoBlackboard()->LastDamageResultType = EBattleDamageResultEnum::VE_Immune;				
			}
			break;
			case EProtectType::VE_ImmuneWithHP:
			{
				if (Element.ProtectPower > Damage)
				{
					Element.ProtectPower -= Damage;
					Damage = 0;
					OwnerBattleEntityComponent->GetBattleInfoBlackboard()->LastDamageResultType = EBattleDamageResultEnum::VE_Immune;					
				}
				else /* if (Element->ProtectPower <= Damage)*/
				{
					OnBuffCheckPoint = true;
					Damage -= Element.ProtectPower;
					Element.ProtectType = EProtectType::VE_None;

					RemoveProtectApplierType.Emplace(Element.SkillApplierType);
				}
			}
			break;
			case EProtectType::VE_ImmuneRangeWithHPAndReturnDamage:
			{				
				EAttackTypeEnum CheckType = OwnerBattleEntityComponent->GetBattleInfoBlackboard()->LastAttackType;//OwnerCharacter->GetBlackBoardData<EAttackTypeEnum>(EBattleInfoPropertyType::VE_LastAttackType);
				if (CheckType == Element.AttackType)
				{
					if (Element.ProtectPower > Damage)
					{
						Element.ProtectPower -= Damage;
						OwnerBattleEntityComponent->GetBattleInfoBlackboard()->ReturnDamage = Damage;
						Damage = 0;
						OwnerBattleEntityComponent->GetBattleInfoBlackboard()->LastDamageResultType = EBattleDamageResultEnum::VE_Immune;
					}
					else /* if (Element->ProtectPower <= Damage)*/
					{
						OnBuffCheckPoint = true;
						Damage -= Element.ProtectPower;
						OwnerBattleEntityComponent->GetBattleInfoBlackboard()->ReturnDamage = Element.ProtectPower;
						Element.ProtectType = EProtectType::VE_None;

						RemoveProtectApplierType.Emplace(Element.SkillApplierType);
					}
				}			
			}
			break;
			case EProtectType::VE_MissRange:
			{
				EAttackTypeEnum CheckType = OwnerBattleEntityComponent->GetBattleInfoBlackboard()->LastAttackType;//OwnerCharacter->GetBlackBoardData<EAttackTypeEnum>(EBattleInfoPropertyType::VE_LastAttackType);
				if (CheckType == Element.AttackType)
				{
					Damage = 0;
					OwnerBattleEntityComponent->GetBattleInfoBlackboard()->LastDamageResultType = EBattleDamageResultEnum::VE_Miss;
				}
			}
			break;
			case EProtectType::VE_ImmunePhysical:
			case EProtectType::VE_ImmuneMagic:
			{
				ECharacterPropertyTypeEnum CheckType = OwnerBattleEntityComponent->GetBattleInfoBlackboard()->LastAttackPropertyType;
				if (CheckType == Element.PropertyType)
				{
					Damage = 0;
					OwnerBattleEntityComponent->GetBattleInfoBlackboard()->LastDamageResultType = EBattleDamageResultEnum::VE_Immune;
				}
			}
			break;
		}
	}

	for (auto& RemoveSkillApplierElem : RemoveProtectApplierType)
	{
		CloseProtectSlot(RemoveSkillApplierElem);
	}

	ProtectArray.RemoveAll([](const FProtectInfo ProtectElement){
		return (ProtectElement.ProtectType == EProtectType::VE_None);
	});

	/*
	Check damage block applier
	*/
	IEntityBaseProperty* From_EntityBaseProperty = Cast<IEntityBaseProperty>( FromActor );
	if (From_EntityBaseProperty)
	{
		switch (From_EntityBaseProperty->GetAttachPropertyType())
		{
			case ECharacterPropertyTypeEnum::VE_PhysicalAttackPower:
			{
				if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_physical_damage))
				{
					OwnerBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_physical_damage);
					return 0;
				}
			}
			break;
			case ECharacterPropertyTypeEnum::VE_MagicAttackPower:
			{
				if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_magical_damage))
				{
					OwnerBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_magical_damage);
					return 0;
				}
			}
			break;
		}
	}

	UBattleEntityComponent* FromActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(FromActor);
	if (IsValid(FromActor_BattleEntityComponent))
	{
		EAttackTypeEnum AttackType = FromActor_BattleEntityComponent->GetBattleInfo()->CharacterProperty.AttackType;
		if (EAttackTypeEnum::VE_Melee == AttackType)
		{
			if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_melee_damage))
			{
				OwnerBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_melee_damage);
				return 0;
			}
		}

		if (EAttackTypeEnum::VE_Range == AttackType)
		{
			if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_range_damage))
			{
				OwnerBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_range_damage);
				return 0;
			}
		}
	}

	// Block instant damage
	if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_block_instant_damage))
	{
		OwnerBattleEntityComponent->OnSetBlockAbnormalMsg.Broadcast(ESkillApplierTypeEnum::VE_p_block_instant_damage);
		return 0;
	}
	
	/*
	Force damage
	*/	
	// passive
	USkillPassiveSlotContainer* PassiveSkillContainer = OwnerSkillEntityComponent->GetPassiveSkillSlotContainer();
	if (PassiveSkillContainer)
	{		
		FSkillApplierBase OutApplier;
		if (PassiveSkillContainer->GatherPersistentAppilerValue(ESkillApplierTypeEnum::VE_p_force_damage, OutApplier))
		{
			Damage = OutApplier.TargetValue;
		}
	}
	
	// active
	USkillSlotContainer*		ActiveSkillContainer = OwnerSkillEntityComponent->GetSkillSlotContainer();
	if (ActiveSkillContainer)
	{
		FSkillApplierBase OutApplier;
		if (ActiveSkillContainer->GatherPersistentAppilerValue(ESkillApplierTypeEnum::VE_p_force_damage, OutApplier))
		{
			Damage = OutApplier.TargetValue;
		}
	}
	
	
		

	return Damage;
}

void UBattleOperator::CloseProtectSlot(const ESkillApplierTypeEnum InSkillApplierType)
{
	USkillSlotContainer*		ActiveSkillContainer	= OwnerSkillEntityComponent->GetSkillSlotContainer();
	USkillPassiveSlotContainer* PassiveSkillContainer	= OwnerSkillEntityComponent->GetPassiveSkillSlotContainer();
	if (ActiveSkillContainer && PassiveSkillContainer)
	{
		USkillSlot* ActiveSlot = ActiveSkillContainer->GetSlotThatContainApplier(InSkillApplierType);
		if (ActiveSlot)
		{
			ActiveSlot->CloseSlot();
		}
	}
}

float UBattleOperator::ApplyAdditionalDamage(float InDamage, AActor* FromActor, EBattleDamageResultEnum& OutDamageResult)
{
	if (OwnerSkillEntityComponent == nullptr || OwnerBattleEntityComponent == nullptr )
		return InDamage;

	UBattleEntityComponent* FromActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent( FromActor );
	USkillEntityComponent* FromActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(FromActor);
	if (nullptr == FromActor_BattleEntityComponent || nullptr == FromActor_SkillEntityComponent)
		return InDamage;

	if (FromActor_BattleEntityComponent->GetBattleInfoBlackboard() == nullptr)
		return InDamage;

	auto OwnerActor_BaseProperty = Cast<IEntityBaseProperty>(FromActor);
	if (OwnerActor_BaseProperty == nullptr)
		return InDamage;

	float AppliedFinalDamage = InDamage;

	/*
	Additional damage
	*/
	int32 AdditionalPercent = 0;
	int32 AdditionalValue = 0;
	USkillSlotContainer* SkillSlotContainer = OwnerSkillEntityComponent->GetSkillSlotContainer();
	EItemEquipTypeEnum equiptype = EItemEquipTypeEnum::VE_None;
	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(OwnerActor_BaseProperty->GetCharacterCategory(), OwnerActor_BaseProperty->GetCharacterID());
	
	if (CharacterInfo)
	{
		equiptype = CharacterInfo->EquipType;
	}

	UBattleInfoBlackboard* OwnerBattleBB = OwnerBattleEntityComponent->GetBattleInfoBlackboard();
	UBattleInfoBlackboard* AttackerBattleBB = FromActor_BattleEntityComponent->GetBattleInfoBlackboard();
	if (!IsValid(OwnerBattleBB) || !IsValid(AttackerBattleBB))
		return InDamage;
	
	/*Bless VS Protect*/
	FSkillApplierBase AttackerBlessApplier = AttackerBattleBB->GetBlessApplier();
	FSkillApplierBase OnwerProtectApplier = OwnerBattleBB->GetProtectApplier();
	if (AttackerBlessApplier.SkillApplierType == ESkillApplierTypeEnum::VE_None)
	{
		if (OnwerProtectApplier.SkillApplierType != ESkillApplierTypeEnum::VE_None)
		{
			AdditionalPercent -= OnwerProtectApplier.TargetPercent;
		}
	}
	else
	{
		if (OnwerProtectApplier.SkillApplierType == ESkillApplierTypeEnum::VE_None)
		{
			AdditionalPercent += AttackerBlessApplier.TargetPercent;
		}
		else
		{
			FMatrix BlessProtectMat = AttackerBattleBB->GetBlessProtectMat();

			bool isIn1 = FMath::IsWithin<ESkillApplierTypeEnum>(AttackerBlessApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Counter_BEGIN, ESkillApplierTypeEnum::VE_p_Counter_MID1);
			bool isIn2 = FMath::IsWithin<ESkillApplierTypeEnum>(OnwerProtectApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Counter_MID2, ESkillApplierTypeEnum::VE_p_Counter_END);
			if (isIn1 && isIn2)
			{
				int32 iRow = (int32)AttackerBlessApplier.SkillApplierType - (int32)ESkillApplierTypeEnum::VE_p_Counter_BEGIN - 1;
				int32 iCol = (int32)OnwerProtectApplier.SkillApplierType - (int32)ESkillApplierTypeEnum::VE_p_Counter_MID2 - 1;

				float Coeff = BlessProtectMat.M[iRow][iCol];

				AdditionalPercent += Coeff > 0.f ? AttackerBlessApplier.TargetPercent * Coeff : OnwerProtectApplier.TargetPercent * Coeff;
			}
		}
	}
	/*Bless VS Protect End*/
	
	/*Bless VS Weak*/
	FMatrix4x6 BlessWeakMat = AttackerBattleBB->GetBlessWeakMat();
	TArray<FSkillApplierBase> WeakAppliers = OwnerBattleBB->GetWeakAppliers();
	bool bisIn = FMath::IsWithin<ESkillApplierTypeEnum>(AttackerBlessApplier.SkillApplierType, ESkillApplierTypeEnum::VE_p_Counter_BEGIN, ESkillApplierTypeEnum::VE_p_Counter_MID1);
	if (bisIn)
	{
		int32 iRow = (int32)AttackerBlessApplier.SkillApplierType - (int32)ESkillApplierTypeEnum::VE_p_Counter_BEGIN - 1;

		float Protect[(int32)ECounterType::END] = {};
		Protect[iRow] = 1.f;

		for (size_t i = 0; i < WeakAppliers.Num(); ++i)
		{
			int32 iCol = (int32)WeakAppliers[i].SkillApplierType - (int32)ESkillApplierTypeEnum::VE_p_Counter_MID1 - 1;

			float ResultFloat = 0;
			for (size_t j = 0; j < (uint32)EWeakType::END; ++j)
			{
				ResultFloat += Protect[j] * BlessWeakMat.M[j][iCol];
			}

			if (ResultFloat != 0)
			{
				AdditionalPercent += WeakAppliers[i].TargetPercent;
			}
		}
	}
	/*Bless VS Weak End*/
	

	/*Spirit Plus vs Minus Start*/
	TArray<FSkillApplierBase> SpiritPlusArray = AttackerBattleBB->GetSpiritPlusAppliers();
	TArray<FSkillApplierBase> SpiritMinusArray = OwnerBattleBB->GetSpiritMinusAppliers();
	FMatrix plusminusmat = AttackerBattleBB->GetSpiritPlusMinusMat();
	FMatrix CalculateMatrix = FMatrix(EForceInit::ForceInitToZero);
	for (size_t i = 0; i < SpiritPlusArray.Num(); ++i)
	{
		if (SpiritPlusArray[i].SkillApplierType != ESkillApplierTypeEnum::VE_None)
		{
			for (size_t j = 0; j < SpiritMinusArray.Num(); ++j)
			{
				if (SpiritMinusArray[j].SkillApplierType != ESkillApplierTypeEnum::VE_None)
				{
					CalculateMatrix.M[i][j] = plusminusmat.M[i][j];
				}
			}
		}
	}
	
	for (size_t i = 0; i < 4; ++i)
	{
		bool bBlockSucceeded = false;
		for (size_t j = 0; j < 4; ++j)
		{
			if (CalculateMatrix.M[i][j] < 0)
			{
				AdditionalPercent += SpiritPlusArray[i].GetPercent() - SpiritMinusArray[j].GetPercent();
				bBlockSucceeded = true;
			}
		}

		if (bBlockSucceeded == false)
		{
			if (SpiritPlusArray.IsValidIndex(i) && SpiritPlusArray[i].SkillApplierType != ESkillApplierTypeEnum::VE_None)
			{
				AdditionalPercent += SpiritPlusArray[i].GetPercent();
			}
		}
	}

	/*Spirit Plus vs Minus End*/
	AppliedFinalDamage = InDamage + InDamage * ((float)(1.0f + AdditionalPercent) / 100.0f) + AdditionalValue;


	/*
	Apply instant skill attack applier
	*/
	FSkillApplierBase* InstantSkillAttackApplier = FromActor_BattleEntityComponent->GetBattleInfoBlackboard()->GetInstantAttackSkillApplier();
	if (InstantSkillAttackApplier)
	{
		switch (InstantSkillAttackApplier->SkillApplierType)
		{
			case ESkillApplierTypeEnum::VE_i_p_attack:
			case ESkillApplierTypeEnum::VE_i_m_attack:			
				AppliedFinalDamage = AppliedFinalDamage + AppliedFinalDamage * ((float)(1.0f + InstantSkillAttackApplier->TargetPercent) / 100.0f) + InstantSkillAttackApplier->TargetValue;
				break;			

			case ESkillApplierTypeEnum::VE_i_hp_percent_attack:
				{
					int32 OwnerMaxHP = OwnerBattleEntityComponent->GetBattleInfo()->GetCharacterFinalProperty<int32>(ECharacterPropertyTypeEnum::VE_MaxHP);
					int32 PercentBonus = (float)OwnerMaxHP * ((float)InstantSkillAttackApplier->TargetPercent / 100.0f);
					AppliedFinalDamage = FMath::Min( PercentBonus, InstantSkillAttackApplier->TargetValue );
					OutDamageResult = EBattleDamageResultEnum::VE_Hit;
				}
				break;

			case ESkillApplierTypeEnum::VE_t_attack_pile_up:
				{
					AppliedFinalDamage = AppliedFinalDamage * (InstantSkillAttackApplier->TargetPercent * 0.01f);
					OutDamageResult = EBattleDamageResultEnum::VE_Counter;
				}
				break;
		}
	}

	/*
	Damage defence bonus
	*/

	/*
	Vamp attack
	*/	
	if (FromActor_BattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_vamp_attack))
	{
		FSkillApplierBase VampAttackApplier = FromActor_BattleEntityComponent->GetBattleInfoBlackboard()->VampAttackApplier;
		float PercentBonus = AppliedFinalDamage * ((1.0f + VampAttackApplier.TargetPercent) / 100.0f);
		float FinalBonus = PercentBonus + VampAttackApplier.TargetValue;

		//FromActor_SkillEntityComponent->CheckAccumulationSkillCost(ESkillApplierTypeEnum::VE_p_vamp_attack);
		FromActor_BattleEntityComponent->ApplyFinalDamage(-FinalBonus, FromActor, EBattleDamageResultEnum::VE_Heal );
		FromActor_BattleEntityComponent->NotifyReceiveHeal(FinalBonus, OwnerCharacter.Get());
	}

	/*
	Reflection damage
	*/
	if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_reflect_damage))
	{
		if (FromActor_BattleEntityComponent->GetBattleInfo())
		{
			EAttackTypeEnum FromAttackType = FromActor_BattleEntityComponent->GetBattleInfo()->GetCharacterProperty<EAttackTypeEnum>(ECharacterPropertyTypeEnum::VE_AttackType);
			if (FromAttackType == EAttackTypeEnum::VE_Melee)
			{
				FSkillApplierBase ReflectionDamageApplier = OwnerBattleEntityComponent->GetBattleInfoBlackboard()->ReflectionDamageApplier;
				float PercentBonus = AppliedFinalDamage * ((1.0f + ReflectionDamageApplier.TargetPercent) / 100.0f);
				float FinalBonus = FMath::Min((int32)PercentBonus, ReflectionDamageApplier.TargetValue);

				FromActor_BattleEntityComponent->ApplyFinalDamage(FinalBonus, OwnerCharacter.Get(), EBattleDamageResultEnum::VE_NoneHitDamage);
			}
		}
	}

	if (OwnerBattleEntityComponent->IsInAbnormalStateApplier(ESkillApplierTypeEnum::VE_p_reflect_damage_All))
	{
		FSkillApplierBase ReflectionDamageApplier = OwnerBattleEntityComponent->GetBattleInfoBlackboard()->ReflectionDamageApplier;
		float PercentBonus = AppliedFinalDamage * ((1.0f + ReflectionDamageApplier.TargetPercent) / 100.0f);
		float FinalBonus = FMath::Min((int32)PercentBonus, ReflectionDamageApplier.TargetValue);

		FromActor_BattleEntityComponent->ApplyFinalDamage(FinalBonus, OwnerCharacter.Get(), EBattleDamageResultEnum::VE_NoneHitDamage);
	}

	return AppliedFinalDamage;
}



float UBattleOperator::GetProtectPower(EProtectType ProtectType)
{
	float Result = 0.0f;
	for (auto& Element : ProtectArray)
	{
		if (Element.ProtectType == ProtectType)
		{
			Result = Element.ProtectPower;
			break;
		}
	}

	return Result;
}

float UBattleOperator::GetProtectPowerPrecent(EProtectType ProtectType)
{
	float Result = 0.0f;
	for (auto& Element : ProtectArray)
	{
		if (Element.ProtectType == ProtectType)
		{
			Result = (float)Element.ProtectPower / (float)Element.ProtectPowerMax;
			break;
		}
	}

	return Result;
}

bool UBattleOperator::CheckPassiveDamageOnDead(AActor* TargetActor)
{
	if (OwnerSkillEntityComponent.IsValid())
	{
		if (OwnerSkillEntityComponent->GetPassiveSkillSlotContainer())
		{
			auto SkillSlotContainer = OwnerSkillEntityComponent->GetPassiveSkillSlotContainer();
			if (SkillSlotContainer)
			{
				if (OwnerSkillEntityComponent->GetPassiveSkillSlotContainer()->ContainOneTimeValidApplier(ESkillApplierTypeEnum::VE_t_guts, true))
				{
					return true;
				}
				else
				{
					OwnerSkillEntityComponent->GetPassiveSkillSlotContainer()->ClearOneTimeValidApplier(ESkillApplierTypeEnum::VE_t_guts);
				}
			}
		}
	}

	return false;
}

