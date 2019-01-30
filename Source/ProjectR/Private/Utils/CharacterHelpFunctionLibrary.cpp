// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Character/BattleBaseCharacter.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/FunctionalComponents/PathFindingComponent.h"

#include "Character/Interfaces/EntityComponentGetterInterface.h"

#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/VibrationComponent.h"
#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Components/WidgetDecoratorComponent.h"
#include "Character/Components/EffectComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/BossComponents/GigantComponent.h"
#include "Character/Components/BossComponents/AkhanComponent.h"
#include "Character/Components/SpecialMovingComponent.h"
#include "Character/Components/GridMapInspectorComponent.h"
#include "Character/Components/CustomChangeMaterialComponent.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "CustomStructures/BattleObjectInfo.h"


#include "Table/CharacterTableInfo.h"
#include "Table/BattleObjectTableInfo.h"
#include "Table/ItemTableInfo.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"

#include "Skill_V2/SharedSkill/ISkill_V2.h"


#include "GlobalIntegrated.h"


#include "LeaderControlHelper.h"

#include "Network/PacketDataStructures.h"

#if WITH_EDITOR

#include "Editor/BaseEditor.h"
#include "Editor/SpawnerEditor.h"
#endif



#include "GameInfo/RInventory.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/PlayStateInfo.h"
#include "UtilFunctionIntegrated.h"

#include "CharacterHelpFunctionLibrary.h"


FName UCharacterHelpFunctionLibrary::GetHeroSkillIDByIndex(FName InHeroID, int32 InIndex)
{
	FName SkillID;
	checkf(UTableManager::GetInstancePtr() != nullptr, TEXT("[%s]UTableManager::GetInstancePtr() is Empty"), __FUNCTION__);
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(InHeroID);
	checkf(CharacterTableInfo != nullptr, TEXT("[%s]CharacterTableInfo is Empty"), __FUNCTION__);
	FName SuffixStr;
	switch (InIndex)
	{
	case 0:
		SkillID = CharacterTableInfo->ActiveSkill01;
		SuffixStr = TEXT("Active1");
		break;
	case 1:
		SkillID = CharacterTableInfo->ActiveSkill02;
		SuffixStr = TEXT("Active2");
		break;
	case 2:
		SkillID = CharacterTableInfo->ActiveSkill03;
		SuffixStr = TEXT("Active3");
		break;
	case 3:
		SkillID = CharacterTableInfo->PassiveSkill01;
		SuffixStr = TEXT("Passive1");
		break;
	}
	if (SkillID.IsNone())
	{
		SkillID = FName(*(CharacterTableInfo->DisplayName.ToString() + SuffixStr.ToString()));
	}
	return SkillID;
}

void UCharacterHelpFunctionLibrary::MakeHeroSkillStructure( FName InHeroID, int32 InLevel, int32 InIndex, FHERO_SKILL& OutHeroSkill )
{
	OutHeroSkill.level = InLevel;
	OutHeroSkill.skillId = UCharacterHelpFunctionLibrary::GetHeroSkillIDByIndex(InHeroID, InIndex).ToString();
	OutHeroSkill.activated = true;
	OutHeroSkill.slotNo = InIndex;
}

class URemoteMoveComponent* UCharacterHelpFunctionLibrary::GetRemoteMoveComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	IEntityComponentGetterInterface* ComponentGetter = Cast< IEntityComponentGetterInterface >(TargetActor);
	if (ComponentGetter)
		return ComponentGetter->GetRemoteMoveComponent();

	return Cast<URemoteMoveComponent>(TargetActor->GetComponentByClass(URemoteMoveComponent::StaticClass()));
}

class UBattleEntityComponent* UCharacterHelpFunctionLibrary::GetBattleEntityComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	IEntityComponentGetterInterface* ComponentGetter = Cast< IEntityComponentGetterInterface >(TargetActor);
	if (ComponentGetter)
		return ComponentGetter->GetBattleEntityComponent();

	return Cast<UBattleEntityComponent>( TargetActor->GetComponentByClass(UBattleEntityComponent::StaticClass()) );
}

class USkillEntityComponent* UCharacterHelpFunctionLibrary::GetSkillEntityComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	IEntityComponentGetterInterface* ComponentGetter = Cast< IEntityComponentGetterInterface >(TargetActor);
	if (ComponentGetter)
		return ComponentGetter->GetSkillEntityComponent();

	return  Cast<USkillEntityComponent>( TargetActor->GetComponentByClass(USkillEntityComponent::StaticClass()) );
}

class UEntityAIComponent* UCharacterHelpFunctionLibrary::GetEntityAIComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	IEntityComponentGetterInterface* ComponentGetter = Cast< IEntityComponentGetterInterface >(TargetActor);
	if (ComponentGetter)
		return ComponentGetter->GetEntityAIComponent();

	return  Cast<UEntityAIComponent>(TargetActor->GetComponentByClass(UEntityAIComponent::StaticClass()));
}

class UEntityRenderComponent* UCharacterHelpFunctionLibrary::GetEntityRenderComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr; 

	IEntityComponentGetterInterface* ComponentGetter = Cast< IEntityComponentGetterInterface >(TargetActor);
	if (ComponentGetter)
		return ComponentGetter->GetEntityRenderComponent();

	return  Cast<UEntityRenderComponent>(TargetActor->GetComponentByClass(UEntityRenderComponent::StaticClass()));
}

class UAnimStateComponent* UCharacterHelpFunctionLibrary::GetAnimStateComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	IEntityComponentGetterInterface* ComponentGetter = Cast< IEntityComponentGetterInterface >(TargetActor);
	if (ComponentGetter)
		return ComponentGetter->GetAnimStateComponent();

	return  Cast<UAnimStateComponent>(TargetActor->GetComponentByClass(UAnimStateComponent::StaticClass()));
}



class UCharacterMovementComponent* UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	auto CastCharacter = Cast<ACharacter>(TargetActor);
	if (CastCharacter)
	{
		return CastCharacter->GetCharacterMovement();
	}

	return Cast<UCharacterMovementComponent>(TargetActor->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
}

class UCustomCharacterMovementComponent* UCharacterHelpFunctionLibrary::GetCustomCharacterMovementComponent(AActor* TargetActor)
{
	auto CastBattleCharacter = Cast<ABattleBaseCharacter>(TargetActor);
	if (CastBattleCharacter)
	{
		return CastBattleCharacter->GetCustomCharacterMovementComponent();
	}

	return nullptr;
	//return Cast<UCharacterMovementComponent>(TargetActor->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
}

class USkeletalMeshComponent* UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	auto CastCharacter = Cast<ACharacter>(TargetActor);
	if (CastCharacter)
	{
		return CastCharacter->GetMesh();
	}

	//return nullptr; //todo anim editor...
	return Cast<USkeletalMeshComponent>(TargetActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
}


class UCapsuleComponent* UCharacterHelpFunctionLibrary::GetCapsuleComponent(AActor* InTargetActor)
{
	if (IsValid(InTargetActor) == false)
		return nullptr;

	auto CastCharacter = Cast<ACharacter>(InTargetActor);
	if (CastCharacter)
	{
		return CastCharacter->GetCapsuleComponent();
	}

	return Cast<UCapsuleComponent>(InTargetActor->GetComponentByClass(UCapsuleComponent::StaticClass()));
}

TArray< USkillAreaIndicatorMeshComponent* > UCharacterHelpFunctionLibrary::GetSkillAreaArray(AActor* InTargetActor)
{
	auto RenderComponent = GetEntityRenderComponent(InTargetActor);
	if (RenderComponent)
	{
		return RenderComponent->GetSkillAreaArray();
	}

	TArray< USkillAreaIndicatorMeshComponent* >	AreaIndicatorComponents;
	if(InTargetActor)
		InTargetActor->GetComponents< USkillAreaIndicatorMeshComponent >(AreaIndicatorComponents);
	return AreaIndicatorComponents;
}


UWidgetDecoratorComponent* UCharacterHelpFunctionLibrary::GetHpBarWidgetComponent(AActor* InTargetActor)
{
	auto RenderComponent = GetEntityRenderComponent(InTargetActor);
	if (RenderComponent)
	{
		return RenderComponent->GetHpBarWidgetComponent();
	}

	return nullptr;
}

class UBossBaseComponent* UCharacterHelpFunctionLibrary::GetCharacterBossBaseComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	IEntityComponentGetterInterface* ComponentGetter = Cast< IEntityComponentGetterInterface >(TargetActor);
	if (ComponentGetter)
	{
		auto BattleEntityComponent = ComponentGetter->GetBattleEntityComponent();
		if (BattleEntityComponent)
		{
			return BattleEntityComponent->GetBossComponent();
		}
	}

	return nullptr;
}

class UAkhanComponent* UCharacterHelpFunctionLibrary::GetAkhanComponent(AActor* TargetActor)
{
	auto BossComponent = GetCharacterBossBaseComponent(TargetActor);
	if (BossComponent)
	{
		return Cast<UAkhanComponent>(BossComponent);
	}

	return nullptr;
}

class UGigantComponent* UCharacterHelpFunctionLibrary::GetGigantComponent(AActor* TargetActor)
{
	auto BossComponent = GetCharacterBossBaseComponent(TargetActor);
	if (BossComponent)
	{
		return Cast<UGigantComponent>(BossComponent);
	}

	return nullptr;
}

class UVibrationComponent* UCharacterHelpFunctionLibrary::GetVibrationComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	auto CastBattleCharacter = Cast<ABattleBaseCharacter>(TargetActor);
	if (CastBattleCharacter)
	{
		return CastBattleCharacter->GetVibrationComponent();
	}

	return nullptr;
}

class USpecialMovingComponent* UCharacterHelpFunctionLibrary::GetSpecialMovingComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	auto CastBattleCharacter = Cast<ABattleBaseCharacter>(TargetActor);
	if (CastBattleCharacter)
	{
		return CastBattleCharacter->GetSpecialMovingComponent();
	}

	return nullptr;
}

UGridMapInspectorComponent* UCharacterHelpFunctionLibrary::GetGridMapInspectorComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	auto CastBattleCharacter = Cast<ABattleBaseCharacter>(TargetActor);
	if (CastBattleCharacter)
	{
		return CastBattleCharacter->GetGridMapInspectorComponent();
	}

	return nullptr;
}

UCustomChangeMaterialComponent* UCharacterHelpFunctionLibrary::GetCustomMaterialComponent(AActor* TargetActor)
{
	auto RenderComponent = GetEntityRenderComponent(TargetActor);
	if (RenderComponent)
	{
		return RenderComponent->GetCustomMaterialComponent();
	}

	return nullptr;
}


UEffectComponent* UCharacterHelpFunctionLibrary::GetEffectComponent(AActor* TargetActor)
{
	if (IsValid(TargetActor) == false)
		return nullptr;

	auto CastBattleCharacter = Cast<ABattleBaseCharacter>(TargetActor);
	if (CastBattleCharacter)
	{
		return CastBattleCharacter->GetEffectComponent();
	}

	return nullptr;
}


UBattleBaseAnimInstance* UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(AActor* TargetActor)
{
	ACharacter* CheckCharacter = Cast< ACharacter >(TargetActor);
	if (CheckCharacter)
	{
		UAnimInstance* AnimInstance = CheckCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			return Cast< UBattleBaseAnimInstance >(AnimInstance);
		}
	}

	return nullptr;
}

UAnimInstance* UCharacterHelpFunctionLibrary::GetCharacterAnimInstance(AActor* TargetActor)
{
	ACharacter* CheckCharacter = Cast< ACharacter >(TargetActor);
	if (CheckCharacter)
	{
		return CheckCharacter->GetMesh()->GetAnimInstance();
	}

	return nullptr;
}

ECharacterCategoryTypeEnum UCharacterHelpFunctionLibrary::GetCharacterCategory(AActor* TargetActor)
{
	if (IsValid(TargetActor))
	{
		auto BasePropertyEntity = Cast<IEntityBaseProperty>(TargetActor);
		if (BasePropertyEntity != nullptr)
		{
			return BasePropertyEntity->GetCharacterCategory();
		}
	}
	return ECharacterCategoryTypeEnum::VE_None;
}

EClassTypeEnum UCharacterHelpFunctionLibrary::GetCharacterClassType(AActor* TargetActor)
{
	if (IsValid(TargetActor))
	{
		auto BasePropertyEntity = Cast<IEntityBaseProperty>(TargetActor);
		if (BasePropertyEntity != nullptr)
		{
			FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(BasePropertyEntity->GetCharacterCategory(), BasePropertyEntity->GetCharacterID());
			if (CharacterInfo)
			{
				return CharacterInfo->Class;
			}
		}
	}
	return EClassTypeEnum::VE_ClassTypeMax;
}

FName UCharacterHelpFunctionLibrary::GetCharacterID(AActor* TargetActor)
{
	if (IsValid(TargetActor))
	{
		auto BasePropertyEntity = Cast<IEntityBaseProperty>(TargetActor);
		if (BasePropertyEntity != nullptr)
		{
			return BasePropertyEntity->GetCharacterID();
		}
	}
	return FName();
}

UCharacterBattleInfo* UCharacterHelpFunctionLibrary::GetBattleInfo(AActor* InTargetActor)
{
	UBattleEntityComponent* TargetActor_BattleEntityComp = GetBattleEntityComponent(InTargetActor);
	if (TargetActor_BattleEntityComp)
	{
		return TargetActor_BattleEntityComp->GetBattleInfo();
	}
	return nullptr;
}


bool UCharacterHelpFunctionLibrary::IsDead(AActor* InTargetActor)
{
	UBattleEntityComponent* BattleEntityComponent = GetBattleEntityComponent(InTargetActor);
	if (BattleEntityComponent)
	{
		return BattleEntityComponent->IsDead();
	}
	return false;
}


bool UCharacterHelpFunctionLibrary::IsRemoteEntity(AActor* InTargetActor)
{
	if (IsValid(InTargetActor))
	{
		auto InTargetActor_BaseProperty = Cast<IEntityBaseProperty>(InTargetActor);
		if (InTargetActor_BaseProperty)
		{
			if (InTargetActor_BaseProperty->GetEntityType() == EEntityTypeEnum::VE_RemoteEntity)
			{
				return true;
			}
		}
	}

	return false;
}


bool UCharacterHelpFunctionLibrary::LaunchCharacter(AActor* DeadActor, AActor* FromActor, bool bXYOverride, bool bZOverride)
{
	URGameInstance*			GameInstance = RGAMEINSTANCE(GEngine);
	checkf(GameInstance != nullptr, TEXT("[%s]GameInstance is Empty"), __FUNCTION__);

	if (false == IsValid(DeadActor) || false == IsValid(FromActor))
		return false;

	if (URenewal_BattleFunctionLibrary::IsBossActor(DeadActor))
		return false;

	ACharacter* DeadCharacter = Cast< ACharacter >(DeadActor);
	if (false == IsValid(DeadCharacter))
		return false;

	FVector launchDir = DeadActor->GetActorLocation() - FromActor->GetActorLocation();
	launchDir.Normalize();
	launchDir = launchDir + FVector(0, 0, 1);

	DeadCharacter->LaunchCharacter(launchDir * GameInstance->GameEnvironmentVar.DeadLaunchPower, bXYOverride, bZOverride);
	return true;
}

void UCharacterHelpFunctionLibrary::SetOriginalMaterial(AActor* TargetActor)
{
	USkeletalMeshComponent* SkeletalMeshComponent = TargetActor->FindComponentByClass<USkeletalMeshComponent>();
	int32 MaterialSlotNum = SkeletalMeshComponent->GetNumMaterials();

	UEntityRenderComponent* RenderComp = GetEntityRenderComponent(TargetActor);
	if (RenderComp)
	{
		for (size_t i = 0; i < RenderComp->OriginalMaterialList.Num(); ++i)
		{
			if (i >= MaterialSlotNum)
				break;

			SkeletalMeshComponent->SetMaterial(i, RenderComp->OriginalMaterialList[i]);
		}
	}
}

bool UCharacterHelpFunctionLibrary::IsPossibleToSpawn(AActor* InTargetActor)
{
	bool SpawnPossible = false;
	if (IsValid(InTargetActor))
	{
		auto BasePropertyEntity = Cast<IEntityBaseProperty>(InTargetActor);
		if (BasePropertyEntity != nullptr)
		{
			FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(BasePropertyEntity->GetCharacterCategory(), BasePropertyEntity->GetCharacterID());
			if (CharacterInfo)
			{
				SpawnPossible = (CharacterInfo->SpawnType != ESpawnTypeEnum::VE_SpawnNone);
			}
		}
	}

	return SpawnPossible;
}



bool UCharacterHelpFunctionLibrary::CastSlotSkill(AActor* InTargetActor, ESkillCategoryTypeEnum InSkillCategoryType /*= ESkillCategoryTypeEnum::VE_ActiveSkill03*/)
{
	if (!IsValid(InTargetActor))
		return false;

	auto SkillEntComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(InTargetActor);
	auto BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(InTargetActor);
	bool IsCastSkill = false;
	
	if (IsValid(SkillEntComp) && IsValid(BattleEntComp))
	{
		UCharacterBattleInfo* BattleInfo = BattleEntComp->GetBattleInfo();
		float AttackRange = BattleInfo->GetCharacterFinalProperty< float >(ECharacterPropertyTypeEnum::VE_AttackRange);

		UISkill_V2* ActiveSkill = SkillEntComp->GetSkillByCategory_V2(InSkillCategoryType);
		if (ActiveSkill)
		{
			AActor* CurTarget = BattleEntComp->GetCurrentAttackTarget();
			if (!IsValid(CurTarget))
			{
				BattleEntComp->SetCurrentAttackTarget(URenewal_BattleFunctionLibrary::FindEnemy(InTargetActor, InTargetActor->GetActorLocation(), ESearchShapeTypeEnum::VE_Circle, AttackRange, EEnemySearchTargetTypeEnum::VE_Close));
				CurTarget = BattleEntComp->GetCurrentAttackTarget();
			}

			if (CurTarget)
			{
				FVector Location = UMapFunctionLibrary::GetNavAgentLocation(CurTarget);
				IsCastSkill = SkillEntComp->CastInputSkill(ActiveSkill->GetSkillID(), InTargetActor, CurTarget, Location);
			}
			else
			{
				UE_LOG(LogGamePlay, Log, TEXT("target empty"));
				IsCastSkill = SkillEntComp->CastInputSkill(ActiveSkill->GetSkillID(), InTargetActor, nullptr);
			}
		}
	}

	return IsCastSkill;
}


