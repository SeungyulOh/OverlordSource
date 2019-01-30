// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AsyncLoader.h"

#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"


URAsyncLoader* URAsyncLoader::InstancePtr = nullptr;

URAsyncLoader::URAsyncLoader()
{	
}

URAsyncLoader* URAsyncLoader::GetInstancePtr()
{
	if (InstancePtr == nullptr)
	{
		InstancePtr = NewObject< URAsyncLoader >((UObject*)GetTransientPackage(), URAsyncLoader::StaticClass());
		InstancePtr->AddToRoot();
		InstancePtr->FxQueue.Empty();
		InstancePtr->CacheBGImage();
	}
	
	return InstancePtr;
}

void URAsyncLoader::ReleaseSingleton()
{
	if (InstancePtr)
	{
		InstancePtr->RemoveFromRoot();
	}
	InstancePtr = nullptr;
}

void URAsyncLoader::LoadAsyncLobbyCharacter(TArray<FSoftObjectPath> Assets, FStreamableDelegate Callback)
{
	TArray<FSoftObjectPath> AsyncAsset;
	AsyncAsset.Reset(Assets.Num());
	for (FSoftObjectPath& objectPath : Assets)
	{
		if (!mAssets.Contains(objectPath) && !objectPath.IsNull() && !objectPath.IsSubobject())
		{
			AsyncAsset.AddUnique(objectPath);
			mAssets.AddUnique(objectPath);
		}
	}
	if (AsyncAsset.Num() > 0)
	{
		GetInstancePtr()->_StreamableManager.RequestAsyncLoad(AsyncAsset, Callback);
	}
	else
	{
		Callback.ExecuteIfBound();
	}
}

void URAsyncLoader::LoadAsyncResource(TArray<FSoftObjectPath> Assets, FStreamableDelegate Callback)
{
	TArray<FSoftObjectPath> AsyncAsset;
	AsyncAsset.Reset(Assets.Num());
	for (FSoftObjectPath& objectPath : Assets)
	{
		if (!mAssets.Contains(objectPath) && !objectPath.IsNull() && !objectPath.IsSubobject())
		{
			AsyncAsset.AddUnique(objectPath);
			mAssets.AddUnique(objectPath);
		}
	}
	if (AsyncAsset.Num() > 0)
	{
		GetInstancePtr()->_StreamableManager.RequestAsyncLoad(AsyncAsset, Callback);
	}
	else
	{
		Callback.ExecuteIfBound();
	}
}

void URAsyncLoader::ClearResource()
{
	for (auto& Element : mAssets)
	{
		GetInstancePtr()->_StreamableManager.Unload(Element);
	}
	mAssets.Empty();
}

void URAsyncLoader::LoadFx()
{
	if (FxQueue.IsEmpty())
		return;

	FSpawnFxInfo fxInfo;
	if (FxQueue.Peek(fxInfo))
	{
		if (fxInfo.FXKey.IsValid())
		{
			FResourceFXTableInfo* info = UTableManager::GetInstancePtr()->GetResourceFXRow(fxInfo.FXKey);
			//info

			/*this is added only for test play*/
			ATestGameMode* TestGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(GEngine)->GetWorld()));
			if (TestGameMode)
			{
				if (info && !info->Particle.IsValid())
					info->Particle.LoadSynchronous();
			}
			/*this should be deleted when we ship this game*/
			
			
			if (info && info->Particle.IsValid() && info->Particle.IsNull() == false)
			{
				if (info->Particle.ToSoftObjectPath().IsValid())
				{
					if (fxInfo.TargetActor.IsValid())	// || fxInfo.TargetActor->IsPendingKill() || fxInfo.TargetActor->GetWorld() == nullptr)
					{
						UParticleSystemComponent* SpawnEffect = nullptr;
						if (fxInfo.SocketName.IsNone())
						{
							SpawnEffect = UGameplayStatics::SpawnEmitterAtLocation(fxInfo.TargetActor->GetWorld(), info->Particle.Get(), fxInfo.SpawnLocation, fxInfo.SpawnRotator);
						}
						else
						{
							USkeletalMeshComponent* TargetActor_SkeletalMeshComp = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(fxInfo.TargetActor.Get());
							if (IsValid(TargetActor_SkeletalMeshComp))
							{
								SpawnEffect = UGameplayStatics::SpawnEmitterAttached(info->Particle.Get(), TargetActor_SkeletalMeshComp, fxInfo.SocketName, fxInfo.SpawnLocation, fxInfo.SpawnRotator, EAttachLocation::KeepRelativeOffset);
								if (IsValid(SpawnEffect))
								{
									//UE_LOG(LogBattle, Log, TEXT("SpawnFX %s"), FXKey);
									if (info->UseParentScale)
									{
										FVector SocketScale = TargetActor_SkeletalMeshComp->GetSocketTransform(fxInfo.SocketName).GetScale3D();
										SpawnEffect->SetWorldScale3D(SocketScale);
									}
									else
									{
										SpawnEffect->SetWorldScale3D(FVector(1, 1, 1));
									}
								}
							}
						}
						fxInfo.Result = SpawnEffect;
					}
				}

				if (info->Sound.IsValid())
				{
					if (fxInfo.SpawnLocation.IsNearlyZero())
					{
						if(fxInfo.TargetActor.IsValid())
							fxInfo.SpawnLocation = fxInfo.TargetActor->GetActorLocation();
					}
					UGameplayStatics::PlaySoundAtLocation(RGAMEINSTANCE(GEngine)->GetWorld(), info->Sound.Get(), fxInfo.SpawnLocation);
				}
			}
		}
		FxQueue.Pop();
	}
}

void URAsyncLoader::AddLoadFx(FSpawnFxInfo info)
{
	FxQueue.Enqueue(info);
}

void URAsyncLoader::ClearFxQueue()
{
	FxQueue.Empty();
}

void URAsyncLoader::CacheBGImage()
{
	HeroPortraitBGArray.Empty(5);

	UPaperSprite* PortraitBGTex = nullptr;
	PortraitBGTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/HeroPortraitBG/HeroPortraitBG1_Sprite.HeroPortraitBG1_Sprite'")));
	if (PortraitBGTex)
		HeroPortraitBGArray.Emplace(PortraitBGTex);
	PortraitBGTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/HeroPortraitBG/HeroPortraitBG2_Sprite.HeroPortraitBG2_Sprite'")));
	if (PortraitBGTex)
		HeroPortraitBGArray.Emplace(PortraitBGTex);
	PortraitBGTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/HeroPortraitBG/HeroPortraitBG3_Sprite.HeroPortraitBG3_Sprite'")));
	if (PortraitBGTex)
		HeroPortraitBGArray.Emplace(PortraitBGTex);
	PortraitBGTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/HeroPortraitBG/HeroPortraitBG4_Sprite.HeroPortraitBG4_Sprite'")));
	if (PortraitBGTex)
		HeroPortraitBGArray.Emplace(PortraitBGTex);
	PortraitBGTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/HeroPortraitBG/HeroPortraitBG5_Sprite.HeroPortraitBG5_Sprite'")));
	if (PortraitBGTex)
		HeroPortraitBGArray.Emplace(PortraitBGTex);


	//
	PortraitBGArray.Empty(5);

	PortraitBGTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/HeroPortraitBG/PortraitBG1_Sprite.PortraitBG1_Sprite'")));
	if (PortraitBGTex)
		PortraitBGArray.Emplace(PortraitBGTex);
	PortraitBGTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/HeroPortraitBG/PortraitBG2_Sprite.PortraitBG2_Sprite'")));
	if (PortraitBGTex)
		PortraitBGArray.Emplace(PortraitBGTex);
	PortraitBGTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/HeroPortraitBG/PortraitBG3_Sprite.PortraitBG3_Sprite'")));
	if (PortraitBGTex)
		PortraitBGArray.Emplace(PortraitBGTex);
	PortraitBGTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/HeroPortraitBG/PortraitBG4_Sprite.PortraitBG4_Sprite'")));
	if (PortraitBGTex)
		PortraitBGArray.Emplace(PortraitBGTex);
	PortraitBGTex = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, TEXT("PaperSprite'/Game/UMG/Image/Atlas/HeroPortraitBG/PortraitBG5_Sprite.PortraitBG5_Sprite'")));
	if (PortraitBGTex)
		PortraitBGArray.Emplace(PortraitBGTex);



}