// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ObjectPool.h"
#include "GlobalIntegrated.h"

void UObjectPool::Initialize(int32 Size)
{
	PoolSize = Size;

	UnusedPool.Empty();
	UnusedPool.Reserve(PoolSize);
	for (int32 i = 0; i < PoolSize; ++i)
	{
		auto Object = NewObject<UObject>((UObject*)GetTransientPackage(), ObjectType);
		if(IsValid(Object))
		{
			UnusedPool.Emplace(Object);
		}
	}
}

void UObjectPool::ResizePool(int32 Size)
{
	PoolSize += Size;

	for (int32 i = 0; i < Size; ++i)
	{
		auto Object = NewObject<UObject>((UObject*)GetTransientPackage(), ObjectType);
		if (IsValid(Object))
		{
			UnusedPool.Emplace(Object);
		}
	}
}

void UObjectPool::SetType(EObjectPoolTypeEnum InObjectPoolType, UClass* TargetType)
{
	ObjectPoolType	= InObjectPoolType;
	ObjectType		= TargetType;
}

UObject* UObjectPool::GetUnusedObject( )
 {
	 if (UnusedPool.Num() <= 0)
		 return nullptr;

	 //UObject* UnusedObject = UnusedPool.Last();

	 UObject* UnusedObject = UnusedPool[0];
	 if (IsValid(UnusedObject))
	 {
		 auto RemoveObjectNumber = UnusedPool.Remove(UnusedObject);
		 UsedPool.Emplace(UnusedObject);

		 return UnusedObject;
	 }

	 return nullptr;
}

void UObjectPool::ReturnObject(UObject* TargetObject)
{
	if (IsValid(TargetObject))
	{
		auto RemoveObjectNumber = UsedPool.Remove(TargetObject);
		UnusedPool.Emplace(TargetObject);
	}
}