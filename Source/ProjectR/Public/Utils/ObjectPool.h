// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UI/UIDefs.h"
#include "Global/ObjectPoolManager.h"
#include "Global/RGameInstance.h"
#include "ObjectPool.generated.h"

/**
 * 
 */

//UCLASS templates are not supported.
UCLASS()
class PROJECTR_API UObjectPool : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void			Initialize(int32 Size = 50);
	virtual void			ResizePool(int32 Size = 5);
	virtual UObject*		GetUnusedObject();
	virtual void			ReturnObject(UObject* TargetObject);
	void					SetType(EObjectPoolTypeEnum InObjectPoolType, UClass* TargetType);
	

protected:
	int32								PoolSize = 0;
	
	UPROPERTY()
	UClass*								ObjectType = nullptr;
	
	EObjectPoolTypeEnum					ObjectPoolType;

	UPROPERTY()
	TArray< UObject* >	UnusedPool;
	UPROPERTY()
	TArray< UObject* > 	UsedPool;
};