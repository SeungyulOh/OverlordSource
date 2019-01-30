// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CharacterSetCameraActor.h"
#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"

// Sets default values
ACharacterSetCameraActor::ACharacterSetCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterSetCameraActor::BeginPlay()
{
	Super::BeginPlay();
	CameraOriginalTransform = GetTransform();
}

// Called every frame
void ACharacterSetCameraActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
}

void ACharacterSetCameraActor::RemoveUISpawnCharacter()
{
	if (UISpawnCharacterPtr.IsValid())
	{
		AActor* Character = UISpawnCharacterPtr.Get();
		if (Character)
		{
			Character->Destroy();
			Character = nullptr;
			UISpawnCharacterPtr = nullptr;
		}
	}
}

AActor* ACharacterSetCameraActor::CreateUISpawnCharacter(FString& HeroUD)
{
	RemoveUISpawnCharacter();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 	{
// 		return nullptr;
// 	}

// 	UGameObjectManager* ObjectMgr = RGameInstance->GameObjectMgr;
// 	if (!IsValid(ObjectMgr))
// 	{
// 		return nullptr;
// 	}

	//Rotation Front(Special Position)
	FTransform SpawnTransform;
	//SpawnTransform.SetLocation(FVector(0.0, 0.0, 500.0f)); 
	SpawnTransform.SetRotation(FRotator(0.0f, 0.0f, 0.0f).Quaternion());
	SpawnTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	
	this->SetActorRotation(FRotator::ZeroRotator);
	SpawnTransform = SpawnTransform * GetTransform();
	
	int32 UserKID = RGameInstance->HttpClient->GetKID();

	auto HeroInvenData = RGameInstance->RInventory->GetHeroWithUD(HeroUD);
	if (HeroInvenData)
	{
// 		UISpawnCharacterPtr = ObjectMgr->SpawnCharacter(SpawnTransform, HeroInvenData->heroId, ECharacterCategoryTypeEnum::VE_UICharacter, 
// 														UserKID, HeroInvenData->heroUD, GroupKey_SINGLE_MY, PartyID_PC_MY, PLAYER_UK_NONE);

		UISpawnCharacterPtr = UObjectFunctionLibrary::SpawnCharacter(SpawnTransform, HeroInvenData->heroId, ECharacterCategoryTypeEnum::VE_UICharacter,
														UserKID, HeroInvenData->heroUD, GroupKey_SINGLE_MY, PartyID_PC_MY, PLAYER_UK_NONE);
	}
	
	return UISpawnCharacterPtr.Get();
}

void ACharacterSetCameraActor::SetCameraAndUICharacterToOriginalTrasnformation()
{
	this->SetActorTransform(CameraOriginalTransform);
	
	if(UISpawnCharacterPtr.IsValid())
	{
		UISpawnCharacterPtr.Get()->SetActorRotation(FQuat(0.0f, 0.0f, 0.0f, 0));
	}
}