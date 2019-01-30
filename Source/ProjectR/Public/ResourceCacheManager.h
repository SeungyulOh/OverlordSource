// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StreamableManager.h"
#include "ResourceCacheManager.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FResourceCacheTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FResourceCacheTable")
	TSoftObjectPtr<UObject> cacheobj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FResourceCacheTable")
	TSoftClassPtr<UClass> cacheclass;


	

};

/**
 * 
 */
UCLASS()
class PROJECTR_API UResourceCacheManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Clear();
	void cache_HeroShop(FStreamableDelegate Callback);
	void cache_TutorialMap(FStreamableDelegate Callback);


	void		SetAssetResource(TArray<FSoftObjectPath>& InAssets, FName& InHeroID, bool InNpc = false);
	void		SetAssetResource_FromTables(TArray<FSoftObjectPath>& InAssets);

	void RecursiveCacheResource(FStreamableDelegate Callback);
	UFUNCTION()
	void Callback_AsyncLoaded();
	
public:
	DECLARE_DELEGATE(FOnFinishedAsyncLoaded)
	FOnFinishedAsyncLoaded OnAsyncLoadedCompleted;


	UPROPERTY()
	TArray<UObject*>	CachedObjectArray;
	UPROPERTY()
	TArray<UClass*>		CachedClassArray;
	UPROPERTY()
	TArray<FSoftObjectPath> AssetsQueue;

private:
	int32 iPartition = 5;
	
};
