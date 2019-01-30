 // Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AutoSpawner.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"

#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"

#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"


#include "UtilFunctionIntegrated.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "GameInfo/PlayStateInfo.h"
#include "GlobalIntegrated.h"




#include "CustomStructures/SpawnStructures/StageSpawnInfo.h"
#include "CustomStructures/SpawnStructures/SpawnerTransformInfo.h"
#include "UtilFunctionIntegrated.h"
#include "Object/BaseCameraActor.h"

const int MaxHeroSpawnCount = 5;

//#define drawspawn
UAutoSpawner::UAutoSpawner()
{

}

bool UAutoSpawner::Init()
{
	int32 NumSpawnGroup = static_cast<int32>(ESpawnGroup::VE_Max);
	SpawnPositionCheck.SetNum(NumSpawnGroup);
	for (int32 i = 0; i < NumSpawnGroup; ++i)
	{
		SpawnPositionCheck[i].SpawnCheck.SetNumZeroed(MaxHeroSpawnCount);
		/*for (int32 j = 0; j < MaxHeroSpawnCount; ++j)
		{
			SpawnPositionCheck[i].SpawnCheck[j] = false;
		}*/
	}

	return true;
}

bool UAutoSpawner::SetSelectedCharacterProperty()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	checkf(RGameInstance != nullptr, TEXT("[%s]URGameInstance is Empty"), __FUNCTION__);

	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);

	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameWorld);
	if (IsValid(BasePlayerController) == false)
		return false;

	SelectedCharacter = BasePlayerController->GetSelectedCharacter();
	if (IsValid(SelectedCharacter) == false)
		return false;

	auto BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SelectedCharacter);
	if (IsValid(BattleEntityComp) == false)
		return false;

	SelectedCharacterUnitRadius = BattleEntityComp->GetBoundingRadius();
	
	auto BattleInfo = UCharacterHelpFunctionLibrary::GetBattleInfo(SelectedCharacter);
	if (IsValid(BattleInfo) == false)
		return false;


	SelectedCharacterAttackType = BattleInfo->CharacterProperty.AttackType;
	SelectedCharacterAttackRange = BattleInfo->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_AttackRange);
	SelectedCharacterSlotIndex = RGameInstance->PlayStateInfo->GetSelectedSlotIndex();

	return true;
}

bool UAutoSpawner::SetCurrentAttackTargetProperty()
{
	auto BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(SelectedCharacter);
	if (IsValid(BattleEntityComp) == false)
		return false;

	CurrentAttackTarget = BattleEntityComp->GetCurrentAttackTarget();
	if (IsValid(CurrentAttackTarget) == true)
	{
		auto CurrentAttackTargetBattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CurrentAttackTarget);
		if (IsValid(CurrentAttackTargetBattleEntityComp) == false)
			return false;

		EnemyRadius = CurrentAttackTargetBattleEntityComp->GetBoundingRadius();
	}

	return true;
}

bool UAutoSpawner::SetSpawnCharacterProperty(const int32 SlotIndex)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("[%s]URGameInstance is Empty"), __FUNCTION__);
	checkf(RGameInstance->PlayStateInfo != nullptr, TEXT("[%s]RGameInstance->PlayStateInfo is Empty"), __FUNCTION__);

	auto SlotInfo = RGameInstance->PlayStateInfo->GetSelectedCharacterSlot(SlotIndex);
	if (SlotInfo.HeroIDName.IsNone() == true)
		return false;

	auto CharacterTableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, SlotInfo.HeroIDName);
	if (CharacterTableInfo == nullptr)
		return false;

	SpawnCharacterSlotIndex = SlotIndex;
	SpawnCharacterAttackType = CharacterTableInfo->AttackType;
	SpawnCharacterAttackRange = CharacterTableInfo->AttackRange;
	SpawnCharacterUnitRadius = CharacterTableInfo->UnitRadius;

	return true;
}

bool UAutoSpawner::AutoSpawnCharacters(FTransform SpawnTransform)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("[%s]URGameInstance is Empty"), __FUNCTION__);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	/*Init();

	if (SetSelectedCharacterProperty() == false)
		return false;

	
	SetCurrentAttackTargetProperty();

	switch (SelectedCharacterAttackType)
	{
	case EAttackTypeEnum::VE_Melee:
		SpawnPositionCheck[static_cast<int32>(ESpawnGroup::VE_Group1)].SpawnCheck[0] = true;
		break;

	case EAttackTypeEnum::VE_Magic:
	case EAttackTypeEnum::VE_Range:
		SpawnPositionCheck[static_cast<int32>(ESpawnGroup::VE_Group2)].SpawnCheck[0] = true;
		break;
	default:
		return false;
	}*/

	return SpawnCharacters(SpawnTransform);
}

bool UAutoSpawner::SpawnCharacters(FTransform SpawnTransform)
{
	return false;
}

bool UAutoSpawner::GetValidSpawnPointByGroup(const int32 SlotIndex, FVector& ValidSpawnPos)
{
	if (SetSpawnCharacterProperty(SlotIndex) == false)
		return false;

	ESpawnGroup SpawnGroup;
	switch (SpawnCharacterAttackType)
	{
	case EAttackTypeEnum::VE_Melee:
		SpawnGroup = ESpawnGroup::VE_Group1;
		break;

	case EAttackTypeEnum::VE_Magic:
	case EAttackTypeEnum::VE_Range:
		SpawnGroup = ESpawnGroup::VE_Group2;
		break;	
	default:
		return false;
	}

	return GetValidSpawnPosition(SpawnGroup, ValidSpawnPos);
}

bool UAutoSpawner::GetValidSpawnPosition(ESpawnGroup SpawnGroup, FVector& ValidSpawnPos)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("[%s]URGameInstance is Empty"), __FUNCTION__);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	FVector AttackDir;

	if (IsValid(CurrentAttackTarget) == true)
	{
		auto CurrentAttackTarget_BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CurrentAttackTarget);
		if (IsValid(CurrentAttackTarget_BattleEntityComp) == false)
			return false;

		auto CurrentAttackTargetLocation	= CurrentAttackTarget->GetActorLocation();
		CurrentAttackTargetLocation.Z		= 0;

		auto SelectedCharacterLocation	= SelectedCharacter->GetActorLocation();
		SelectedCharacterLocation.Z		= 0;

		AttackDir = CurrentAttackTargetLocation - SelectedCharacterLocation;
	}
	else
	{
		AttackDir = SelectedCharacter->GetActorForwardVector();
	}

	AttackDir.Z = 0;
	AttackDir.Normalize();
	
	auto RightSpawnDir = FVector::CrossProduct(AttackDir, FVector::UpVector);
	auto LeftSpawnDir = FVector::CrossProduct(FVector::UpVector, AttackDir);

	LeftSpawnDir.Normalize();
	RightSpawnDir.Normalize();

#ifdef drawspawn
	auto AttackSPawnPosLocation = SelectedCharacter->GetActorLocation() + AttackDir * 1000.0f;
	auto RightSpawnPosLocation = SelectedCharacter->GetActorLocation() + LeftSpawnDir * 1000.0f;
	auto LeftSpawnPosLocation = SelectedCharacter->GetActorLocation() + RightSpawnDir * 1000.0f;
	DrawDebugLine(GameWorld, SelectedCharacter->GetActorLocation(), AttackSPawnPosLocation, FColor(255, 0, 0, 1), false, 10.0f, 0.0f, 5.0f);
	DrawDebugLine(GameWorld, SelectedCharacter->GetActorLocation(), RightSpawnPosLocation, FColor(0, 255, 0, 1), false, 10.0f, 0.0f, 5.0f);
	DrawDebugLine(GameWorld, SelectedCharacter->GetActorLocation(), LeftSpawnPosLocation, FColor(0, 0, 255, 1), false, 10.0f, 0.0f, 5.0f);
#endif

	//Height
	float OffsetX = 0.0f;

	//Width
	float OffsetY = 0.0f;
	int32 GroupIndex = static_cast<int32>(SpawnGroup);

	auto SelectedCharacterLocation = SelectedCharacter->GetActorLocation();

	TArray<FVector> SpawnFailPos;
	for (int32 i = 0; i < MaxHeroSpawnCount; ++i)
	{
		if (SpawnPositionCheck[GroupIndex].SpawnCheck[i] == true)
			continue;

		OffsetX = SelectedCharacterUnitRadius * 2 * ((i + 1) / 2);

		if (GetOffSetY(SpawnGroup, OffsetY) == false)
			continue;

		FVector OffSetXApplyDir, OffSetYApplyDir;

		if ((i + 1) % 2 == 1)
			OffSetXApplyDir = RightSpawnDir * OffsetX;
		else
			OffSetXApplyDir = LeftSpawnDir * OffsetX;

		OffSetYApplyDir = AttackDir * OffsetY;

		FVector ProjectionPos;
		auto SpawnPosition = SelectedCharacterLocation + OffSetXApplyDir + OffSetYApplyDir;

		if(UMapFunctionLibrary::CanSpawnPostion(RGameInstance,SpawnPosition) == false)
		{
			SpawnFailPos.Emplace(SpawnPosition);

			continue;
		}

		ValidSpawnPos = SpawnPosition;
		SpawnPositionCheck[GroupIndex].SpawnCheck[i] = true;

		return true;
	}


	for (const auto& SpawnFailPosElem : SpawnFailPos)
	{
		FCellIndex AdjacentCell;
		if (UMapFunctionLibrary::GetAdjacentCellNearestWithTargetPositionOnNavyMesh(AdjacentCell, SpawnFailPosElem, 1, 100) == true)
		{
			ValidSpawnPos = UMapFunctionLibrary::GetWorldPosFromCell(AdjacentCell);

			return true;
		}
	}
	

	return false;
}

bool UAutoSpawner::GetOffSetY(ESpawnGroup SpawnGroup, float& OffSetY)
{
	switch (SelectedCharacterAttackType)
	{
	case EAttackTypeEnum::VE_Melee:
		return GetOffSetYSelectedAttackTypeNormal(SpawnGroup, OffSetY);
		break;

	case EAttackTypeEnum::VE_Magic:
	case EAttackTypeEnum::VE_Range:
		return GetOffSetYSelectedAttackTypeRange(SpawnGroup, OffSetY);
		break;
	//case EAttackTypeEnum::VE_Heal:
	//	return GetOffSetYSelectedAttackTypeHeal(SpawnGroup, OffSetY);
	//	break;
	default:
		break;
	}

	return true;
}

bool UAutoSpawner::GetOffSetYSelectedAttackTypeNormal(ESpawnGroup SpawnGroup, float& OffsetY)
{
	switch (SpawnGroup)
	{
	case ESpawnGroup::VE_Group1:
		OffsetY = 0.0f;
		break;
	case ESpawnGroup::VE_Group2:
		OffsetY = (SelectedCharacterUnitRadius + SelectedCharacterAttackRange) - (SpawnCharacterUnitRadius + SpawnCharacterAttackRange);
		break;
	case ESpawnGroup::VE_Group3:
		OffsetY = -(SelectedCharacterUnitRadius + SpawnCharacterAttackRange + SpawnCharacterUnitRadius);
		break;
	default:
		return false;
	}

	return true;
}

bool UAutoSpawner::GetOffSetYSelectedAttackTypeRange(ESpawnGroup SpawnGroup, float& OffSetY)
{
	switch (SpawnGroup)
	{
	case ESpawnGroup::VE_Group1:
		OffSetY = (SelectedCharacterUnitRadius + SelectedCharacterAttackRange) - (SpawnCharacterUnitRadius + SpawnCharacterAttackRange);
		break;
	case ESpawnGroup::VE_Group2:
		OffSetY = 0.0f;
		break;
	case ESpawnGroup::VE_Group3:
		GetOffSetY_Group2WithGroup3(OffSetY);
		break;
	default:
		return false;
	}
	
	return true;
}

bool UAutoSpawner::ContainSpawnCharacters(FString& HeroKey)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(RGameInstance != nullptr, TEXT("[%s]URGameInstance is Empty"), __FUNCTION__);
	checkf(RGameInstance->PlayStateInfo != nullptr, TEXT("[%s]GameInstance->PlayStateInfo is Empty"), __FUNCTION__);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);

	ABasePlayerController*	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GameWorld);
	if (IsValid(BasePlayerController) == false)
		return false;

	if (IsValid(RGameInstance->PlayStateInfo) == false)
		return false;

	bool bIsAlreadySpawnedNormalAttackCharacter = false;
	UCharacterBattleInfo* BattleInfo = nullptr;
	for (int32 i = 0; i < MaxHeroSpawnCount; ++i)
	{
		if (i == SpawnCharacterSlotIndex || i == SelectedCharacterSlotIndex)
			continue;

		auto HeroSlot = RGameInstance->PlayStateInfo->GetSelectedCharacterSlot(i);
		BattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(HeroSlot.HeroUD);
		if (BattleInfo == nullptr)
		{
			continue;
		}

		if (BattleInfo->FinalCharacterProperty.AttackType == EAttackTypeEnum::VE_Melee)
		{
			bIsAlreadySpawnedNormalAttackCharacter = true;
			HeroKey = HeroSlot.HeroUD;
			break;
		}
		BattleInfo = nullptr;
	}


	return bIsAlreadySpawnedNormalAttackCharacter;
}

bool UAutoSpawner::GetOffSetY_Group2WithGroup3(float& OffsetY)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("[%s]URGameInstance is Empty"), __FUNCTION__);

	FString HeroKey;
	auto bIsAlreadySpawnedNormalAttackCharacter = ContainSpawnCharacters(HeroKey);
	auto CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(HeroKey);

	if (bIsAlreadySpawnedNormalAttackCharacter && CharacterBattleInfo != nullptr)
	{
		auto Group1_HeroSpawnPos = (SelectedCharacterUnitRadius + SelectedCharacterAttackRange) - (SpawnCharacterUnitRadius + SpawnCharacterAttackRange);
		OffsetY = (Group1_HeroSpawnPos - CharacterBattleInfo->FinalCharacterProperty.UnitRadius) - (SpawnCharacterAttackRange + SpawnCharacterUnitRadius);
	}
	else
	{
		OffsetY = -SpawnCharacterUnitRadius - SelectedCharacterUnitRadius;
	}
	CharacterBattleInfo = nullptr;
	return true;
}

bool UAutoSpawner::GetOffSetYSelectedAttackTypeHeal(ESpawnGroup SpawnGroup, float& OffSetY)
{
	switch (SpawnGroup)
	{
	case ESpawnGroup::VE_Group1:
		OffSetY = SelectedCharacterAttackRange + SelectedCharacterUnitRadius + SpawnCharacterUnitRadius;
		break;
	case ESpawnGroup::VE_Group2:
		GetOffSetY_Group3WithGroup2(OffSetY);
		break;
	case ESpawnGroup::VE_Group3:
		OffSetY = 0;
		break;
	default:
		return false;
	}

	return true;
}

bool UAutoSpawner::GetOffSetY_Group3WithGroup2(float& OffsetY)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("[%s]URGameInstance is Empty"), __FUNCTION__);

	FString HeroKey;
	auto bIsAlreadySpawnedNormalAttackCharacter = ContainSpawnCharacters(HeroKey);
	auto CharacterBattleInfo = URenewal_BattleFunctionLibrary::MakeCharacteBattleInfoFromInven(HeroKey);

	if (bIsAlreadySpawnedNormalAttackCharacter && CharacterBattleInfo != nullptr)
	{
		auto Group1_HeroSpawnPos = (SelectedCharacterUnitRadius + SelectedCharacterAttackRange) + (SpawnCharacterUnitRadius);
		OffsetY = (Group1_HeroSpawnPos + CharacterBattleInfo->FinalCharacterProperty.UnitRadius + CharacterBattleInfo->FinalCharacterProperty.AttackRange) 
		- (SpawnCharacterUnitRadius + SpawnCharacterAttackRange);
	}
	else
	{
		OffsetY = SpawnCharacterUnitRadius + SelectedCharacterUnitRadius;
	}
	CharacterBattleInfo = nullptr;
	return true;
}