// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BGMActor.h"
#include "Table/ResourceSoundTableInfo.h"

#include "UtilFunctionIntegrated.h"

// Sets default values
ABGMActor::ABGMActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	BGMComponent = CreateDefaultSubobject<UAudioComponent>(FName(TEXT("BGM")));
	if (IsValid(BGMComponent))
	{
		BGMComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);		
	}
}

// Called when the game starts or when spawned
void ABGMActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABGMActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ABGMActor::CheckBGMComponent(int32 BGMIndex)
{
	if (BGMComponent && BGMComponent->Sound == nullptr)
	{
		BGMComponent->bAutoDestroy = true;
		BGMComponent->bIsUISound = true;
		FString TableStr = TEXT("BGM_");
		TableStr += FString::FromInt(BGMIndex);
		
		FResourceSoundTableInfo* ResourceSoundTableInfo = UTableManager::GetInstancePtr()->GetResourceSoundRow(FName(*TableStr));
		if (ResourceSoundTableInfo)
		{
			BGMComponent->SetSound(UUtilFunctionLibrary::GetSoundCueFromAssetID(ResourceSoundTableInfo->Sound));
		}

		//FResourceSoundTableInfo* ResourceSoundTableInfo = UTableManager::GetInstancePtr()->GetResourceSoundRow(FName(TEXT("BGM_List")));
		//if (ResourceSoundTableInfo)
		//{
		//	BGMComponent->SetSound(UUtilFunctionLibrary::GetSoundCueFromAssetID(ResourceSoundTableInfo->Sound));
		//}
	}
}

void ABGMActor::PlayBGMIndex(int32 BGMIndex)
{
	CheckBGMComponent(BGMIndex);

	if (IsValid(BGMComponent))
	{
		//BGMComponent->SetIntParameter(FName(TEXT("BGM_Index")), BGMIndex);
		BGMComponent->Play();
	}
}

void ABGMActor::PauseBGM()
{
	if (IsValid(BGMComponent))
	{
		BGMComponent->SetPaused(true);
	}
}

void ABGMActor::StopBGM()
{
	//CheckBGMComponent();

	if (IsValid(BGMComponent))	
		BGMComponent->Stop();	
}

void ABGMActor::ResumeBGM()
{
	//CheckBGMComponent();

	if (IsValid(BGMComponent))
		BGMComponent->SetPaused(false);
}

void ABGMActor::FadeInBGM(float fTime)
{
	//CheckBGMComponent();

	if (IsValid(BGMComponent))
		BGMComponent->FadeIn(fTime, 1.0f);
}

void ABGMActor::FadeOutBGM(float fTime)
{
	//CheckBGMComponent();

	if (IsValid(BGMComponent))
		BGMComponent->FadeOut(fTime, 0.0f);	
}

void ABGMActor::SetVolume(float InVolume)
{
	//CheckBGMComponent();

	if (IsValid(BGMComponent))
		BGMComponent->SetVolumeMultiplier(InVolume);
}