// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "HeroSlotInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"


FHeroSlotInfo::FHeroSlotInfo()
{	
	HeroUD.Empty();

	CharacterCategoryType = ECharacterCategoryTypeEnum::VE_None;
// 	SpawnCount = MaxSpawnCount;
	SlotSpawnState = EHeroSpawnStateEnum::VE_Ready;
	SpawnedTargetActor = nullptr;
	HeroHP = 1.0f;
}

void FHeroSlotInfo::SetSpawnedCharacter(AActor* SpawnedTarget)
{
	if (CharacterCategoryType == ECharacterCategoryTypeEnum::VE_Hero)
	{
		SpawnedTargetActor = SpawnedTarget;
		SetSpawnState(EHeroSpawnStateEnum::VE_Spawned);
// 		SpawnCount = 1;
	}
	else if (CharacterCategoryType == ECharacterCategoryTypeEnum::VE_Minion )
	{
		SpawnedMinions.Emplace(SpawnedTarget);
// 		SpawnCount--;

		SetSpawnState(EHeroSpawnStateEnum::VE_Spawned);
	}
}

void FHeroSlotInfo::SetSpawnState(EHeroSpawnStateEnum SpawnState)
{
	auto SpawnedTargetActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SpawnedTargetActor);
	auto SpawnedTargetActor_RenderEntityComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(SpawnedTargetActor);
	if (IsValid(SpawnedTargetActor_SkillEntityComponent) && IsValid(SpawnedTargetActor_RenderEntityComponent))
	{
		SlotSpawnState = SpawnState;
		switch (SpawnState)
		{		
		case EHeroSpawnStateEnum::VE_Spawned:
			if (SpawnedTargetActor)
			{
				SpawnedTargetActor_RenderEntityComponent->SetVisible(true);
			}
			break;
		case EHeroSpawnStateEnum::VE_Dead:
		{
			//SpawnedTargetActor_SkillEntityComponent->SetVisibleSkillIcon();
		}
		break;
		}
	}
}

void FHeroSlotInfo::DespawnAll()
{
	if (CharacterCategoryType == ECharacterCategoryTypeEnum::VE_Hero)
	{		
		auto SpawnedTargetActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedTargetActor);
		if (IsValid(SpawnedTargetActor_BattleEntityComponent))
		{
			if (IsValid(SpawnedTargetActor) && SpawnedTargetActor_BattleEntityComponent->IsDead() == false)
			{
				SpawnedTargetActor->K2_DestroyActor();
				SpawnedTargetActor = nullptr;
				SetSpawnState(EHeroSpawnStateEnum::VE_Despawned);
			}
		}
	}
	else if (CharacterCategoryType == ECharacterCategoryTypeEnum::VE_Minion)
	{
		for (auto SpawnedMinionItor = SpawnedMinions.CreateConstIterator(); SpawnedMinionItor; ++SpawnedMinionItor)
		{
			AActor* SpawnedMinion = *SpawnedMinionItor;
			if (IsValid(SpawnedMinion))
				SpawnedMinion->Destroy();
		}

		SetSpawnState(EHeroSpawnStateEnum::VE_Ready);		
		SpawnedMinions.Empty();
	}
}

void FHeroSlotInfo::Save()
{	
	if (SpawnedTargetActor != nullptr)
	{
		auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedTargetActor);
		if (IsValid(BattleEntityComponent))
		{
			int32 hp = BattleEntityComponent->GetHP();
			int32 maxhp = BattleEntityComponent->GetMaxHP();
			HeroHP = (float)hp / (float)maxhp;
		}
	}
}

void FHeroSlotInfo::Load()
{
	if (SpawnedTargetActor != nullptr)
	{
		auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SpawnedTargetActor);
		if (IsValid(BattleEntityComponent))
		{
			float fHp = (float)BattleEntityComponent->GetMaxHP() * HeroHP;
			BattleEntityComponent->SetHP(fHp);
		}
	}
}

void FHeroSlotInfo::Dead()
{
	HeroHP = 0;
}

ECharacterCategoryTypeEnum FHeroSlotInfo::GetCharacterCategoryType()
{
	return CharacterCategoryType;
}
 
ESpawnResultTypeEnum FHeroSlotInfo::CanSpawn()
{
	if (SlotSpawnState == EHeroSpawnStateEnum::VE_None)
		return ESpawnResultTypeEnum::VE_Fail;

// 	if ( SpawnCount <= 0)
// 		return ESpawnResultTypeEnum::VE_CountLess;

	if (SlotSpawnState != EHeroSpawnStateEnum::VE_Ready && CharacterCategoryType == ECharacterCategoryTypeEnum::VE_Hero)
		return ESpawnResultTypeEnum::VE_NoReady;

	if( CharacterCrewState != ECharacterCrewStateEnum::VE_Battle)
		return ESpawnResultTypeEnum::VE_Fail;

	if (SlotSpawnState == EHeroSpawnStateEnum::VE_Spawned)
		return ESpawnResultTypeEnum::VE_Fail;
	
	return ESpawnResultTypeEnum::VE_Success;
}