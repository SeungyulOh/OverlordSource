// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ObjectPoolManager.h"

#include "GlobalIntegrated.h"


// #include "Utils/ObjectPool.h"
// #include "Utils/WidgetObjectPool.h"
// #include "Utils/ComponentObjectPool.h"
// #include "Utils/MapNodeObjectPool.h"
#include "UtilFunctionIntegrated.h"

#include "Character/Components//FunctionalComponents/PathFindingComponent.h"
#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Components/SkillCastingDecalComponent.h"

 


void UObjectPoolManager::Initialize(/*URGameInstance* InRGameInstance*/)
{
#ifndef CHECK_NEXT_MOVE_VALIDATION
	return;
#endif

	//Set RGameInstance
	//RGameInstance = InRGameInstance;

	//SkillAreaMeshComponent
	/*auto StaticMeshTableInfo = UTableManager::GetInstancePtr()->GetResourceStaticMeshRow(FName("SkillPickAreaMesh"));
	if (StaticMeshTableInfo != nullptr)
	{
		this->RegisterTypes< UComponentObjectPool >(EObjectPoolTypeEnum::VE_SkillArea, USkillAreaIndicatorMeshComponent::StaticClass());
	}*/

	this->RegisterTypes<UMapNodeObjectPool>(EObjectPoolTypeEnum::VE_GridDistanceInfo, UGridDistanceInfo::StaticClass());
	//this->RegisterTypes<UComponentObjectPool>(EObjectPoolTypeEnum::VE_SkillCastingDecal, USkillCastingDecalComponent::StaticClass());
}

template<typename T>
void UObjectPoolManager::RegisterTypes(EObjectPoolTypeEnum PoolType, UClass* ObjectType)
{
	if (ObjectPools.Contains(PoolType) == false)
	{
		UObjectPool* ObjectPool = nullptr;
		ObjectPool = NewObject<T>((UObject*)GetTransientPackage(), T::StaticClass());
		if (IsValid(ObjectPool))
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			ObjectPool->SetType(PoolType, ObjectType);
			ObjectPool->Initialize();
			ObjectPools.Emplace(PoolType, ObjectPool);
		}
	}	
}

UObject* UObjectPoolManager::GetUnusedObject(EObjectPoolTypeEnum PoolType)
{
	if (ObjectPools.Contains(PoolType) == false)
		return nullptr;

	auto Object = ObjectPools[PoolType]->GetUnusedObject();
	return Object;
}


void UObjectPoolManager::ReturnObject(UObject* TargetObject, EObjectPoolTypeEnum PoolType)
{
	if (ObjectPools.Contains(PoolType) == false)
		return;

	auto ObjectPool = ObjectPools[PoolType];
	if (IsValid(ObjectPool))
	{
		ObjectPool->ReturnObject(TargetObject);
	}
}

//Common Method
UObjectPool* UObjectPoolManager::GetObjectPoolByType(EObjectPoolTypeEnum PoolType)
{
	if (ObjectPools.Contains(PoolType) == false)
		return nullptr;

	return ObjectPools[PoolType];
}

