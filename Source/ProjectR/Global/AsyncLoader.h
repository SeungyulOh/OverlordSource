// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "AsyncLoader.generated.h"

USTRUCT()
struct PROJECTR_API FSpawnFxInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FName FXKey;
	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor;
	UPROPERTY()
	FVector SpawnLocation;
	UPROPERTY()
	FRotator SpawnRotator; 
	UPROPERTY()
	FName SocketName;
	UPROPERTY()
	TWeakObjectPtr<UParticleSystemComponent> Result;
};

UCLASS(BlueprintType)
class PROJECTR_API URAsyncLoader : public UObject
{
	GENERATED_BODY()
		
public:
	UFUNCTION(BlueprintCallable, Category = URAsyncLoader)
	static URAsyncLoader* GetInstancePtr();
	UFUNCTION(BlueprintCallable, Category = TableManager)
	static void	ReleaseSingleton();

	//void SpawnFX(FName FXKey, AActor* TargetActor, FVector SpawnLocation, FRotator SpawnRotator, FName SocketName);
	void LoadAsyncLobbyCharacter(TArray<FSoftObjectPath> Assets, FStreamableDelegate Callback);
	void LoadAsyncResource(TArray<FSoftObjectPath> Assets, FStreamableDelegate Callback);
	void ClearResource();
	void LoadFx();
	void AddLoadFx(FSpawnFxInfo info);
	void ClearFxQueue();
	void CacheBGImage();

	FORCEINLINE TArray<FSoftObjectPath>& GetAssetPath() { return mAssets; }


public:
	///////Resource 
	UPROPERTY()
	TArray<UPaperSprite*> HeroPortraitBGArray;

	UPROPERTY()
	TArray<UPaperSprite*> PortraitBGArray;

private:
	TQueue<FSpawnFxInfo>		FxQueue;

	URAsyncLoader();

	static URAsyncLoader*	InstancePtr;
	TArray<FSoftObjectPath> mAssets;
	TArray<FSoftObjectPath> mLobbyAssets;
	FStreamableManager	    _StreamableManager;
};
