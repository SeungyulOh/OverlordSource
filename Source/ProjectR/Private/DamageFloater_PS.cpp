// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "DamageFloater_PS.h"
#include "Paper2DClasses.h"

#include "Global/TableManager.h"
#include "Table/ResourceParticleTableInfo.h"

#include "UtilFunctionIntegrated.h"
#include "ProjectRGameMode.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/ParticleModuleRequired.h"
#include "Particles/Event/ParticleModuleEventReceiverBase.h"
#include "Particles/Spawn/ParticleModuleSpawn.h"

const int32 MaxCount = 5;
const int32 LifeTime = 1.f;

const int32 White = 0;
const int32 White_Dark = 1;
const int32 Yello = 2;
const int32 Yello_Dark = 3;
const int32 Red = 4;
const int32 Red_Dark = 5;
const int32 Green = 6;
const int32 Green_Dark = 7;

const int32 NumberInterval = 30;

// Sets default values
ADamageFloater_PS::ADamageFloater_PS()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = RootComp = CreateDefaultSubobject<USceneComponent>(FName(TEXT("RootComp")));

	DamagePSC = CreateDefaultSubobject<UParticleSystemComponent>(FName(TEXT("ParticleSystemComponent")));
	DamagePSC->SetupAttachment(RootComp);
	DamagePSC->bAutoActivate = true;
}

// Called when the game starts or when spawned
void ADamageFloater_PS::BeginPlay()
{
	Super::BeginPlay();

	FResourceParticleTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetResourceParticleRow(TEXT("Floater"));
	if (TableInfo)
	{
		if (TableInfo->Asset.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(TableInfo->Asset.Get());
			if (PS)
			{
				DamagePSC->SetTemplate(PS);
			}
		}
#ifdef WITH_EDITOR
		else
		{
			UParticleSystem* PS = TableInfo->Asset.LoadSynchronous();
			if (PS)
			{
				DamagePSC->SetTemplate(PS);
			}
		}
#endif
	}

	URGameInstance* RGameInst = RGAMEINSTANCE(this);

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGameInst->GetWorld(), 0);
	FVector RightVector = PCM->ViewTarget.Target->GetActorRightVector();
	

	int32 NumberCount = 0;

	if (FinalDamage / 10000 > 0)
		NumberCount = MaxCount;
	else if (FinalDamage / 1000 > 0)
		NumberCount = MaxCount - 1;
	else if (FinalDamage / 100 > 0)
		NumberCount = MaxCount - 2;
	else if (FinalDamage / 10 > 0)
		NumberCount = MaxCount - 3;
	else if (FinalDamage != 0)
		NumberCount = MaxCount - 4;
	else
		NumberCount = 0;

	for (size_t i = 0; i < MaxCount; ++i)
	{
		if (i + NumberCount < MaxCount)
			continue;

		int32 iNum = FMath::Pow(10, MaxCount - i - 1);

		int32 iClippedDamage = FinalDamage % (iNum * 10);
		DamageNumberArray.Emplace(iClippedDamage / iNum);
	}


// 	if (DamageResultEnum == EBattleDamageResultEnum::VE_Hit)
// 	else if (DamageResultEnum == EBattleDamageResultEnum::VE_Critical)
// 	else if (DamageResultEnum == EBattleDamageResultEnum::VE_ReceiveHit || DamageResultEnum == EBattleDamageResultEnum::VE_ReceiveCritical)
// 	else if (DamageResultEnum == EBattleDamageResultEnum::VE_Heal)
		

	for (size_t i = 1; i <= MaxCount; ++i)
	{
		if (i <= NumberCount)
		{
			FParticleEmitterInstance* PEI = DamagePSC->EmitterInstances[i - 1];
			UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(PEI->CurrentMaterial, PEI->Component);

			FVector LeftStart = RightVector * NumberInterval * -0.5f * NumberCount;
			FVector NewOffSet = LeftStart + RightVector * NumberInterval * (i - 1);
			UParticleLODLevel* LODLevel = PEI->SpriteTemplate->GetCurrentLODLevel(PEI);
			if (LODLevel)
			{
				UParticleModuleRequired* RM = LODLevel->RequiredModule;
				RM->EmitterOrigin = NewOffSet;
			}

			UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAttached(SmokePS, RootComp, NAME_None, NewOffSet);
			if(IsValid(PSC))
			{
				PSC->OnSystemFinished.RemoveDynamic(this, &ADamageFloater_PS::DestroyThisActor);
				PSC->OnSystemFinished.AddDynamic(this, &ADamageFloater_PS::DestroyThisActor);
			}
			
		
			int32 DamageNum = 0;
			if (DamageNumberArray.IsValidIndex(i - 1))
				DamageNum = DamageNumberArray[i - 1];

			if (NumberArray.IsValidIndex(DamageNum))
			{
				if (MID)
				{
					MID->SetTextureParameterValue(FName(TEXT("DiffuseTexture")), NumberArray[DamageNum]);
					DamagePSC->SetMaterial(i - 1, MID);
				}
			}
			continue;
		}
		else
		{
			FString EmitterStr = TEXT("Damage") + FString::FromInt(i);
			DamagePSC->SetEmitterEnable(FName(*EmitterStr), false);
		}
	}

	if (TextTexture)
	{
		int32 ElementIdx = DamagePSC->EmitterInstances.Num() - 1;
		FParticleEmitterInstance* PEI = DamagePSC->EmitterInstances[ElementIdx];
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(PEI->CurrentMaterial, PEI->Component);
		if (MID)
		{
			MID->SetTextureParameterValue(FName(TEXT("DiffuseTexture")), TextTexture);
			DamagePSC->SetMaterial(ElementIdx, MID);
		}

		if (IsValid(DamagePSC))
		{
			DamagePSC->OnSystemFinished.RemoveDynamic(this, &ADamageFloater_PS::DestroyThisActor);
			DamagePSC->OnSystemFinished.AddDynamic(this, &ADamageFloater_PS::DestroyThisActor);
		}
	}
	else
		DamagePSC->SetEmitterEnable(FName(TEXT("Text")), false);


	

	if(OwnerActor.IsValid() && FromActor.IsValid())
	{
		if (IsValid(FromActor.Get()))
		{
			InitialPowerDirection = OwnerActor.Get()->GetActorLocation() - FromActor.Get()->GetActorLocation();
			InitialPowerDirection.Normalize();
		}
	}
	InitialUpPowerIntensity = InitialPowerIntensity = FMath::Pow(base, NumberCount) * InitialPowerOffSet;
	decelerationIntensity = Coefficient * InitialUpPowerIntensity;
	
	
}

// Called every frame
void ADamageFloater_PS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPendingKill())
		return;
		
	InitialUpPowerIntensity -= decelerationIntensity * DeltaTime;
	FVector TargetLocation = GetActorLocation() + InitialPowerDirection * DeltaTime * InitialPowerIntensity + FVector(0.f,0.f,1.f) * InitialUpPowerIntensity * DeltaTime;
	SetActorLocation(TargetLocation);

}

void ADamageFloater_PS::Initialize(AActor* InFromActor, AActor* InOwnerActor, float Damage, EBattleDamageResultEnum DamageResultType)
{
	FromActor = InFromActor;
	OwnerActor = InOwnerActor;
	FinalDamage = FMath::Abs<float>(Damage);
	DamageResultEnum = DamageResultType;

	AProjectRGameMode* RGameMode = UUtilFunctionLibrary::GetGamePlayMode();
	if (RGameMode && RGameMode->DamageFloaterMgr)
	{
		int32 MyGroupKey = Cast<IEntityBaseProperty>(InOwnerActor)->GetGroupKey();
		bool isEnemy = MyGroupKey == GroupKey_SINGLE_ENEMY ? true : false;

		NumberArray = RGameMode->DamageFloaterMgr->GetCorrespondingNumberArray(DamageResultType, isEnemy);

		SmokePS = RGameMode->DamageFloaterMgr->GetCorrespondingPS(DamageResultType, isEnemy);

		if (FinalDamage == 0)
			DamageResultType = isEnemy == true ? EBattleDamageResultEnum::VE_Miss : EBattleDamageResultEnum::VE_Dodge;

		if (DamageResultType == EBattleDamageResultEnum::VE_Critical || DamageResultType == EBattleDamageResultEnum::VE_Miss || DamageResultType==EBattleDamageResultEnum::VE_Dodge)
			TextTexture = RGameMode->DamageFloaterMgr->GetCorrespondingTextTexture(DamageResultType);
	}

	
}

void ADamageFloater_PS::DestroyThisActor(UParticleSystemComponent* PSC)
{
	if (IsValidLowLevel() && !IsPendingKill() && PSC->GetActiveParticleNum() == 0)
		Destroy();
}

UDamageFloaterManager::UDamageFloaterManager()
{
	
}

void UDamageFloaterManager::Init()
{
	UTableManager* TableManager = UTableManager::GetInstancePtr();
	if (!TableManager)
		return;

	WhiteSpriteArray.Empty();
	for (int32 i = 0; i < 10; ++i)
	{
		TArray<FStringFormatArg> Args;
		Args.Empty();
		Args.Push(FString::FromInt(i));
		FString Path1 = FString::Format(TEXT("White{0}"), Args);

		FResourceTexture2DTableInfo* TableInfo = TableManager->GetResourceDamageFloaterTex2DRow(FName(*Path1));
		if (TableInfo)
		{
			if (TableInfo->Asset.IsValid())
			{
				UTexture2D* Tex = Cast<UTexture2D>(TableInfo->Asset.Get());
				if (Tex)
				{
					WhiteSpriteArray.Emplace(Tex);
				}
			}
#ifdef WITH_EDITOR
			else
			{
				UTexture2D* Tex = TableInfo->Asset.LoadSynchronous();
				if (Tex)
				{
					WhiteSpriteArray.Emplace(Tex);
				}
			}
#endif
		}
	}

	YelloSpriteArray.Empty();
	for (int32 i = 0; i < 10; ++i)
	{
		TArray<FStringFormatArg> Args;
		Args.Empty();
		Args.Push(FString::FromInt(i));
		FString Path1 = FString::Format(TEXT("Yello{0}"), Args);

		FResourceTexture2DTableInfo* TableInfo = TableManager->GetResourceDamageFloaterTex2DRow(FName(*Path1));
		if (TableInfo)
		{
			if (TableInfo->Asset.IsValid())
			{
				UTexture2D* Tex = Cast<UTexture2D>(TableInfo->Asset.Get());
				if (Tex)
				{
					YelloSpriteArray.Emplace(Tex);
				}
			}
#ifdef WITH_EDITOR
			else
			{
				UTexture2D* Tex = TableInfo->Asset.LoadSynchronous();
				if (Tex)
				{
					YelloSpriteArray.Emplace(Tex);
				}
			}
#endif
		}
	}

	RedSpriteArray.Empty();
	for (int32 i = 0; i < 10; ++i)
	{
		TArray<FStringFormatArg> Args;
		Args.Empty();
		Args.Push(FString::FromInt(i));
		FString Path1 = FString::Format(TEXT("Red{0}"), Args);

		FResourceTexture2DTableInfo* TableInfo = TableManager->GetResourceDamageFloaterTex2DRow(FName(*Path1));
		if (TableInfo)
		{
			if (TableInfo->Asset.IsValid())
			{
				UTexture2D* Tex = Cast<UTexture2D>(TableInfo->Asset.Get());
				if (Tex)
				{
					RedSpriteArray.Emplace(Tex);
				}
			}
#ifdef WITH_EDITOR
			else
			{
				UTexture2D* Tex = TableInfo->Asset.LoadSynchronous();
				if (Tex)
				{
					RedSpriteArray.Emplace(Tex);
				}
			}
#endif
		}
	}

	GreenSpriteArray.Empty();
	for (int32 i = 0; i < 10; ++i)
	{
		TArray<FStringFormatArg> Args;
		Args.Empty();
		Args.Push(FString::FromInt(i));
		FString Path1 = FString::Format(TEXT("Green{0}"), Args);

		FResourceTexture2DTableInfo* TableInfo = TableManager->GetResourceDamageFloaterTex2DRow(FName(*Path1));
		if (TableInfo)
		{
			if (TableInfo->Asset.IsValid())
			{
				UTexture2D* Tex = Cast<UTexture2D>(TableInfo->Asset.Get());
				if (Tex)
				{
					GreenSpriteArray.Emplace(Tex);
				}
			}
#ifdef WITH_EDITOR
			else
			{
				UTexture2D* Tex = TableInfo->Asset.LoadSynchronous();
				if (Tex)
				{
					GreenSpriteArray.Emplace(Tex);
				}
			}
#endif
		}
	}

	FResourceTexture2DTableInfo* TableInfo = TableManager->GetResourceDamageFloaterTex2DRow(TEXT("Miss"));
	if (TableInfo)
	{
		if (TableInfo->Asset.IsValid())
		{
			UTexture2D* Tex = Cast<UTexture2D>(TableInfo->Asset.Get());
			if (Tex)
			{
				MissSprite = Tex;
			}
		}
#ifdef WITH_EDITOR
		else
		{
			UTexture2D* Tex = TableInfo->Asset.LoadSynchronous();
			if (Tex)
			{
				MissSprite = Tex;
			}
		}
#endif
	}

	FResourceTexture2DTableInfo* TableInfo1 = TableManager->GetResourceDamageFloaterTex2DRow(TEXT("Critical"));
	if (TableInfo1)
	{
		if (TableInfo1->Asset.IsValid())
		{
			UTexture2D* Tex = Cast<UTexture2D>(TableInfo1->Asset.Get());
			if (Tex)
			{
				CritSprite = Tex;
			}
		}
#ifdef WITH_EDITOR
		else
		{
			UTexture2D* Tex = TableInfo1->Asset.LoadSynchronous();
			if (Tex)
			{
				CritSprite = Tex;
			}
		}
#endif
	}

	FResourceParticleTableInfo* TableInfo2 = TableManager->GetResourceParticleRow(TEXT("Floater0"));
	if (TableInfo2)
	{
		if (TableInfo2->Asset.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(TableInfo2->Asset.Get());
			if (PS)
			{
				WhitePS = PS;
			}
		}
#ifdef WITH_EDITOR
		else
		{
			UParticleSystem* PS = TableInfo2->Asset.LoadSynchronous();
			if (PS)
			{
				WhitePS = PS;
			}
		}
#endif
	}

	TableInfo2 = TableManager->GetResourceParticleRow(TEXT("Floater1"));
	if (TableInfo2)
	{
		if (TableInfo2->Asset.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(TableInfo2->Asset.Get());
			if (PS)
			{
				YelloPS = PS;
			}
		}
#ifdef WITH_EDITOR
		else
		{
			UParticleSystem* PS = TableInfo2->Asset.LoadSynchronous();
			if (PS)
			{
				YelloPS = PS;
			}
		}
#endif
	}

	TableInfo2 = TableManager->GetResourceParticleRow(TEXT("Floater2"));
	if (TableInfo2)
	{
		if (TableInfo2->Asset.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(TableInfo2->Asset.Get());
			if (PS)
			{
				RedPS = PS;
			}
		}
#ifdef WITH_EDITOR
		else
		{
			UParticleSystem* PS = TableInfo2->Asset.LoadSynchronous();
			if (PS)
			{
				RedPS = PS;
			}
		}
#endif
	}

	TableInfo2 = TableManager->GetResourceParticleRow(TEXT("Floater3"));
	if (TableInfo2)
	{
		if (TableInfo2->Asset.IsValid())
		{
			UParticleSystem* PS = Cast<UParticleSystem>(TableInfo2->Asset.Get());
			if (PS)
			{
				GreenPS = PS;
			}
		}
#ifdef WITH_EDITOR
		else
		{
			UParticleSystem* PS = TableInfo2->Asset.LoadSynchronous();
			if (PS)
			{
				GreenPS = PS;
			}
		}
#endif
	}
}

TArray<UTexture2D*>& UDamageFloaterManager::GetCorrespondingNumberArray(EBattleDamageResultEnum DamageResultType, bool isEnemy)
{
	if (DamageResultType == EBattleDamageResultEnum::VE_Hit)
		return WhiteSpriteArray;
	else if (DamageResultType == EBattleDamageResultEnum::VE_Critical)
		return YelloSpriteArray;
	else if (DamageResultType == EBattleDamageResultEnum::VE_ReceiveHit || DamageResultType == EBattleDamageResultEnum::VE_ReceiveCritical)
	{
		if(isEnemy)
			return WhiteSpriteArray;
		else
			return RedSpriteArray;
	}
	else if (DamageResultType == EBattleDamageResultEnum::VE_Heal)
		return GreenSpriteArray;
	else
		return WhiteSpriteArray;
}

UTexture2D* UDamageFloaterManager::GetCorrespondingTextTexture(EBattleDamageResultEnum DamageResultType)
{
	if (DamageResultType == EBattleDamageResultEnum::VE_Miss || DamageResultType == EBattleDamageResultEnum::VE_Dodge)
		return MissSprite;
	else if (DamageResultType == EBattleDamageResultEnum::VE_Critical)
		return CritSprite;
	else
		return nullptr;
}

UParticleSystem* UDamageFloaterManager::GetCorrespondingPS(EBattleDamageResultEnum DamageResultType, bool isEnemy)
{
	if (DamageResultType == EBattleDamageResultEnum::VE_Hit)
		return WhitePS;
	else if (DamageResultType == EBattleDamageResultEnum::VE_Critical)
		return YelloPS;
	else if (DamageResultType == EBattleDamageResultEnum::VE_ReceiveHit || DamageResultType == EBattleDamageResultEnum::VE_ReceiveCritical)
	{
		if (isEnemy)
			return WhitePS;
		else
			return RedPS;
	}
	else if (DamageResultType == EBattleDamageResultEnum::VE_Heal)
		return GreenPS;
	else
		return nullptr;
}
