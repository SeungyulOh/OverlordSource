// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "UI/UIDefs.h"
#include "ObjectPoolManager.generated.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class EObjectPoolTypeEnum : uint8
{
	VE_SkillArea			UMETA(DisplayName = "SkillAreamMesh"),
	VE_GridDistanceInfo		UMETA(DisplayName = "GridDistanceInfo"),
	VE_SkillCastingDecal	UMETA(DisplayName = "SkillCastingDecal"),
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//class URGameInstance;
class UObjectPool;
/**
 * 
 */
UCLASS()
class PROJECTR_API UObjectPoolManager : public UObject
{ 
	GENERATED_BODY()

public:
	void Initialize(/*URGameInstance* InRGameInstance*/);

	template<typename T>
	void			RegisterTypes(EObjectPoolTypeEnum PoolType, UClass* ObjectType);

	UObject*		GetUnusedObject(EObjectPoolTypeEnum PoolType);

	void			ReturnObject(UObject* TargetObject, EObjectPoolTypeEnum PoolType);

	UObjectPool*	GetObjectPoolByType(EObjectPoolTypeEnum PoolType);

	/*
	UFUNCTION(BlueprintCallable, Category = PoolManager)
	AActor* GetActorObject(EObjectPoolTypeEnum PoolType);
	
	UFUNCTION(BlueprintCallable, Category = PoolManager)
	UUserWidget* GetUserWidgetObject(EObjectPoolTypeEnum PoolType);
	 
	void	ReleaseUserWidgetObject(UUserWidget* InTargetWidget, EObjectPoolTypeEnum PoolType);
	*/
	
public:
	UPROPERTY()
	TMap< EObjectPoolTypeEnum, UObjectPool* >				ObjectPools;


	//UPROPERTY()
	//TMap<UClass, UObjectPool*> ObjectPools;
private:
// 	UPROPERTY()
// 	URGameInstance* RGameInstance = nullptr;
};