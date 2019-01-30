// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BattleInfoBlackboard.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionLibrary.h"
#include "CharacterHelpFunctionLibrary.h"

/*
Getter
*/
template<>
EBattleDamageResultEnum UBattleInfoBlackboard::GetInternalValue<EBattleDamageResultEnum>(EBattleInfoPropertyType PropertyType)
{
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_LastDamageResultType:
		return LastDamageResultType;
	}
	return EBattleDamageResultEnum::VE_None;
}

template<>
EAttackTypeEnum UBattleInfoBlackboard::GetInternalValue<EAttackTypeEnum>(EBattleInfoPropertyType PropertyType)
{
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_LastAttackType:
		return LastAttackType;
	}
	return EAttackTypeEnum::VE_None;
}

template<>
float UBattleInfoBlackboard::GetInternalValue<float>(EBattleInfoPropertyType PropertyType)
{
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_FinalDamage:
		return FinalDamage;
	}
	return 0.0f;
}

template<>
bool UBattleInfoBlackboard::GetInternalValue<bool>(EBattleInfoPropertyType PropertyType)
{
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_SkillAppliedCritically:
		return SkillAppliedCritically;
	}
	return false;
}

template<>
FName UBattleInfoBlackboard::GetInternalValue<FName>(EBattleInfoPropertyType PropertyType)
{
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_LastAppliedSkill:
		return LastAppliedSkill;
		break;
	}
	return FName();
}


/*
Setter
*/


template<>
void UBattleInfoBlackboard::SetInternalValue<EBattleDamageResultEnum>(EBattleInfoPropertyType PropertyType, EBattleDamageResultEnum Value)
{
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_LastDamageResultType:
		LastDamageResultType = Value;
		break;
	}

}

template<>
void UBattleInfoBlackboard::SetInternalValue<EAttackTypeEnum>(EBattleInfoPropertyType PropertyType, EAttackTypeEnum Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("normalattack: Class - %s"), Value == EAttackTypeEnum::VE_Normal));
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_LastAttackType:
		LastAttackType = Value;
		break;
	}

}

template<>
void UBattleInfoBlackboard::SetInternalValue<float>(EBattleInfoPropertyType PropertyType, float Value)
{
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_FinalDamage:
		FinalDamage = Value;
		break;
	}
}

template<>
void UBattleInfoBlackboard::SetInternalValue<bool>(EBattleInfoPropertyType PropertyType, bool Value)
{
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_SkillAppliedCritically:
		SkillAppliedCritically = Value;
		break;
	}
}

template<>
void UBattleInfoBlackboard::SetInternalValue<FName>(EBattleInfoPropertyType PropertyType, FName Value)
{
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_LastAppliedSkill:
		LastAppliedSkill = Value;
		break;
	}
}

template<>
void UBattleInfoBlackboard::SetInternalValue<AActor*>(EBattleInfoPropertyType PropertyType, AActor* Value)
{
// 	switch (PropertyType)
// 	{
// 	
// 	}
}



// Add..

template<>
void UBattleInfoBlackboard::AddInternalValue<float>(EBattleInfoPropertyType PropertyType, float Value)
{
	switch (PropertyType)
	{
	case EBattleInfoPropertyType::VE_FinalDamage:
		FinalDamage += Value;
		break;
	}
}





void UBattleInfoBlackboard::AddDamageCalcState(EBattleDamageResultEnum InDamageCalcState)
{
	DamageCalcStateInfos.Emplace(InDamageCalcState);
}

TArray<EBattleDamageResultEnum>  UBattleInfoBlackboard::GetDamageCalcStateInfos()
{
	return DamageCalcStateInfos;
}


/*
Instant skill attack applier
*/
FSkillApplierBase* UBattleInfoBlackboard::GetInstantAttackSkillApplier()
{
	if (InstantAttackSkillApplier.SkillApplierType == ESkillApplierTypeEnum::VE_None)
		return nullptr;
	return &InstantAttackSkillApplier;
}

void UBattleInfoBlackboard::SetInstantAttackSkillApplier(FSkillApplierBase& InSkillApplier)
{
	InstantAttackSkillApplier = InSkillApplier;
}

void UBattleInfoBlackboard::ClearInstantAttackSkillApplier()
{
	InstantAttackSkillApplier.SkillApplierType = ESkillApplierTypeEnum::VE_None;
}






/*
Clear
*/
// This function is called every tick.
void UBattleInfoBlackboard::ClearBlackBoardData()
{
	ClearLastDamageInformations();
	ClearLastAppliedSkillInformations();
	ClearDamageCalcStateInfos();

	AdditionalDamage = 0.0f;
	AdditionalDamageReduce = 0.0f;
	LastAttackCharacter = nullptr;
	LastAttackResultType = EBattleDamageResultEnum::VE_None;
	bSkillAttack = false;
}

void UBattleInfoBlackboard::ClearLastDamageInformations()
{
	FinalDamage = 0.0f;
	LastDamageResultType	= EBattleDamageResultEnum::VE_None;
	LastAttackType			= EAttackTypeEnum::VE_None;
	LastAttackPropertyType	= ECharacterPropertyTypeEnum::VE_None;
}

void UBattleInfoBlackboard::ClearLastAppliedSkillInformations()
{
	SkillAppliedCritically = false;
	LastAppliedSkill = FName();
}

void UBattleInfoBlackboard::ClearDamageCalcStateInfos()
{
	DamageCalcStateInfos.Empty();
}

void UBattleInfoBlackboard::CounterInit()
{
	WeakProtectMat.M[0][0] = 1;
	WeakProtectMat.M[0][1] = 0;
	WeakProtectMat.M[0][2] = 0;
	WeakProtectMat.M[0][3] = 0;

	WeakProtectMat.M[1][0] = 0;
	WeakProtectMat.M[1][1] = 1;
	WeakProtectMat.M[1][2] = 0;
	WeakProtectMat.M[1][3] = 0;

	WeakProtectMat.M[2][0] = 0;
	WeakProtectMat.M[2][1] = 0;
	WeakProtectMat.M[2][2] = 1;
	WeakProtectMat.M[2][3] = 0;

	WeakProtectMat.M[3][0] = 0;
	WeakProtectMat.M[3][1] = 0;
	WeakProtectMat.M[3][2] = 0;
	WeakProtectMat.M[3][3] = 1;

	WeakProtectMat.M[4][0] = 1;
	WeakProtectMat.M[4][1] = 1;
	WeakProtectMat.M[4][2] = 0;
	WeakProtectMat.M[4][3] = 0;

	WeakProtectMat.M[5][0] = 0;
	WeakProtectMat.M[5][1] = 0;
	WeakProtectMat.M[5][2] = 1;
	WeakProtectMat.M[5][3] = 1;

	BlessWeakMat.M[0][0] = 1;
	BlessWeakMat.M[0][1] = 0;
	BlessWeakMat.M[0][2] = 0;
	BlessWeakMat.M[0][3] = 0;
	BlessWeakMat.M[0][4] = 1;
	BlessWeakMat.M[0][5] = 0;

	BlessWeakMat.M[1][0] = 0;
	BlessWeakMat.M[1][1] = 1;
	BlessWeakMat.M[1][2] = 0;
	BlessWeakMat.M[1][3] = 0;
	BlessWeakMat.M[1][4] = 1;
	BlessWeakMat.M[1][5] = 0;

	BlessWeakMat.M[2][0] = 0;
	BlessWeakMat.M[2][1] = 0;
	BlessWeakMat.M[2][2] = 1;
	BlessWeakMat.M[2][3] = 0;
	BlessWeakMat.M[2][4] = 0;
	BlessWeakMat.M[2][5] = 1;

	BlessWeakMat.M[3][0] = 0;
	BlessWeakMat.M[3][1] = 0;
	BlessWeakMat.M[3][2] = 0;
	BlessWeakMat.M[3][3] = 1;
	BlessWeakMat.M[3][4] = 0;
	BlessWeakMat.M[3][5] = 1;


	FPlane Row1 = FPlane(0.f, 0.f, 1.f, -1.f);
	FPlane Row2 = FPlane(0.f, 0.f, -1.f, 1.f);
	FPlane Row3 = FPlane(-1.f, 1.f, 0.f, 0.f);
	FPlane Row4 = FPlane(1.f, -1.f, 0.f, 0.f);
	BlessProtectMat = FMatrix(Row1, Row2, Row3, Row4);
	
	Row1 = FVector4(1.f, 1.f, 1.f, -1.f);
	Row2 = FVector4(1.f, 1.f, -1.f, 1.f);
	Row3 = FVector4(-1.f, 1.f, 1.f, 1.f);
	Row4 = FVector4(1.f, -1.f, 1.f, 1.f);
	SpiritPlusMinusMat = FMatrix(Row1, Row2, Row3, Row4);
}

void UBattleInfoBlackboard::SetBlessApplier(FSkillApplierBase InApplier)
{
	BlessApplier = InApplier;

	if (BlessApplier.SkillApplierType != ESkillApplierTypeEnum::VE_None)
	{
		UParticleSystem* PS = nullptr;
		if(BlessApplier.SkillApplierType == ESkillApplierTypeEnum::VE_p_bless_solar)
			PS= UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("RedPS"));
		else if (BlessApplier.SkillApplierType == ESkillApplierTypeEnum::VE_p_bless_light)
			PS = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("YelloPS"));
		else if (BlessApplier.SkillApplierType == ESkillApplierTypeEnum::VE_p_bless_dark)
			PS = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("PurplePS"));
		else if (BlessApplier.SkillApplierType == ESkillApplierTypeEnum::VE_p_bless_moon)
			PS = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("BluePS"));



		if (PS)
		{
			if (IsValid(BlessParticleComponent))
			{
				BlessParticleComponent->Deactivate();
				BlessParticleComponent = nullptr;
			}

			BlessParticleComponent = UUtilFunctionLibrary::SpawnEmitterAttached(PS, OwnerCharacter.Get(), TEXT("Bip001-L-Finger0"));
			static float StandardRadius = 96.0f;

			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
			if (IsValid(BlessParticleComponent) && IsValid(BattleEntityComponent))
			{
				UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
				if (BattleInfo)
				{
					float UnitRadius = BattleInfo->CharacterProperty.UnitRadius;
					float EffectScale = UnitRadius / StandardRadius;
					EffectScale *= 0.4f;

					BlessParticleComponent->SetRelativeScale3D(FVector(EffectScale, EffectScale, EffectScale));
				}
			}
		}
	}
}

void UBattleInfoBlackboard::SetProtectApplier(FSkillApplierBase InApplier)
{
	ProtectApplier = InApplier;

	if (ProtectApplier.SkillApplierType != ESkillApplierTypeEnum::VE_None)
	{
		UParticleSystem* PS = nullptr;
		if (ProtectApplier.SkillApplierType == ESkillApplierTypeEnum::VE_p_protect_solar)
			PS = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("RedPS_Protect"));
		else if (ProtectApplier.SkillApplierType == ESkillApplierTypeEnum::VE_p_protect_light)
			PS = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("YelloPS_Protect"));
		else if (ProtectApplier.SkillApplierType == ESkillApplierTypeEnum::VE_p_protect_dark)
			PS = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("PurplePS_Protect"));
		else if (ProtectApplier.SkillApplierType == ESkillApplierTypeEnum::VE_p_protect_moon)
			PS = UUtilFunctionLibrary::GetParticleSystemFromTable(TEXT("BluePS_Protect"));

		if (PS)
		{
			if (IsValid(ProtectParticleComponent))
			{
				ProtectParticleComponent->SetVisibility(false);
				ProtectParticleComponent->Deactivate();
				ProtectParticleComponent->DeactivateSystem();
				ProtectParticleComponent = nullptr;
			}

			ProtectParticleComponent = UUtilFunctionLibrary::SpawnEmitterAttached(PS, OwnerCharacter.Get(), SOCKET_ROOT);
			static float StandardRadius = 96.0f;

			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerCharacter.Get());
			if (IsValid(ProtectParticleComponent) && IsValid(BattleEntityComponent))
			{
				UCharacterBattleInfo* BattleInfo = BattleEntityComponent->GetBattleInfo();
				if (BattleInfo)
				{
					float UnitRadius = BattleInfo->CharacterProperty.UnitRadius;
					float EffectScale = UnitRadius / StandardRadius;
					EffectScale *= 0.3f;

					ProtectParticleComponent->SetRelativeScale3D(FVector(EffectScale, EffectScale, EffectScale));
				}
			}
		}
	}
}

void UBattleInfoBlackboard::SetWeakApplier(FSkillApplierBase InApplier, bool InIsAttach)
{
	if (InIsAttach)
	{
		bool bExe = false;
		if (ProtectApplier.SkillApplierType == ESkillApplierTypeEnum::VE_None)
			bExe = true;
		else
		{
			int32 iRow = (int32)ProtectApplier.SkillApplierType - (int32)ESkillApplierTypeEnum::VE_p_Counter_MID2 - 1;
			int32 iCol = (int32)InApplier.SkillApplierType - (int32)ESkillApplierTypeEnum::VE_p_Counter_MID1 - 1;
			float Weakfloat[(uint32)EWeakType::END] = {};
			Weakfloat[iCol] = 1;
			/*Simple Math*/

			float ResultFloat = 0;
			for (uint32 i = 0; i < (uint32)EWeakType::END; ++i)
			{
				ResultFloat += Weakfloat[i] * WeakProtectMat.M[i][iRow];
			}

			if (ResultFloat == 0)
				bExe = true;
		}

		if (bExe)
		{
			FSkillApplierBase* FoundApplier = WeakApplierArray.FindByPredicate([InApplier](const FSkillApplierBase& Source) {
				if (InApplier.SkillApplierType == Source.SkillApplierType)
					return true;

				return false;
			});

			if (!FoundApplier)
			{
				WeakApplierArray.Emplace(InApplier);
			}
		}

	}
	else
	{
		for (size_t i = 0; i < WeakApplierArray.Num(); ++i)
		{
			if (WeakApplierArray[i].SkillApplierType == InApplier.SkillApplierType)
			{
				WeakApplierArray.RemoveAt(i);
				break;
			}
		}
	}
}

void UBattleInfoBlackboard::SetSpiritPlusApplier(FSkillApplierBase InApplier, bool InIsAttach)
{
	if (!SpiritPlusApplierArray.Num())
	{
		SpiritPlusApplierArray.Init(FSkillApplierBase() , (int32)ECounterType::END);
	}

	if (InIsAttach)
	{
		FSkillApplierBase* Foundapplier = SpiritPlusApplierArray.FindByPredicate([InApplier](const FSkillApplierBase& Source) {
			return Source.SkillApplierType == InApplier.SkillApplierType;
		});

		if (Foundapplier)
		{
			Foundapplier->TargetPercent += InApplier.TargetPercent;
		}
		else
		{
			int32 Targetidx = (int32)InApplier.SkillApplierType - (int32)ESkillApplierTypeEnum::VE_p_spirit_plus_solar;
			if (SpiritPlusApplierArray.IsValidIndex(Targetidx))
				SpiritPlusApplierArray[Targetidx] = InApplier;
		}
			
	}
	else
	{
		for (size_t i = 0; i < SpiritPlusApplierArray.Num(); ++i)
		{
			if (SpiritPlusApplierArray[i].SkillApplierType == InApplier.SkillApplierType)
			{
				SpiritPlusApplierArray.RemoveAt(i);
				break;
			}
		}
	}
}

void UBattleInfoBlackboard::SetSpiritMinusApplier(FSkillApplierBase InApplier, bool InIsAttach)
{
	if (!SpiritMinusApplierArray.Num())
	{
		SpiritMinusApplierArray.Init(FSkillApplierBase(), (int32)ECounterType::END);
	}

	if (InIsAttach)
	{
		FSkillApplierBase* Foundapplier = SpiritMinusApplierArray.FindByPredicate([InApplier](const FSkillApplierBase& Source) {
			return Source.SkillApplierType == InApplier.SkillApplierType;
		});

		if (Foundapplier)
		{
			Foundapplier->TargetPercent += InApplier.TargetPercent;
		}
		else
		{
			int32 Targetidx = (int32)InApplier.SkillApplierType - (int32)ESkillApplierTypeEnum::VE_p_spirit_minus_solar;
			if (SpiritMinusApplierArray.IsValidIndex(Targetidx))
				SpiritMinusApplierArray[Targetidx] = InApplier;
		}
			
	}
	else
	{
		for (size_t i = 0; i < SpiritMinusApplierArray.Num(); ++i)
		{
			if (SpiritMinusApplierArray[i].SkillApplierType == InApplier.SkillApplierType)
			{
				SpiritMinusApplierArray.RemoveAt(i);
				break;
			}
		}
	}
}

void UBattleInfoBlackboard::SetBlackBoardAsAttackType(EAttackTypeEnum AttackType)
{
	LastAttackType = AttackType;
}

EAttackTypeEnum UBattleInfoBlackboard::GetBlackBoardAsAttackType()
{
	return LastAttackType;
}

void UBattleInfoBlackboard::SetBlackBoardSkillAttack(bool InbSkillAttack)
{
	bSkillAttack = InbSkillAttack;
}

bool UBattleInfoBlackboard::GetBlackBoardSkillAttack()
{
	return bSkillAttack;
}

void UBattleInfoBlackboard::SetAttachedPileupSlot(bool bAttached, int32 InMaxCount /*= 0*/, float InPercent /*= 0.0f*/)
{
	bAttachedPileupSlot = bAttached;
	MaxAttackCount = InMaxCount;
	PileupAddDamagePercent = InPercent;
}

bool UBattleInfoBlackboard::IncreaseAttackCount(AActor* InFromActor)
{
	++CurrentAttackCount;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->EventManager->OnUpdateAttackCount.Broadcast(InFromActor);
	return (MaxAttackCount <= CurrentAttackCount);
}

void UBattleInfoBlackboard::ResetAttackCountStack(AActor* InFromActor)
{
	CurrentAttackCount = 0;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->EventManager->OnUpdateAttackCount.Broadcast(InFromActor);
}
