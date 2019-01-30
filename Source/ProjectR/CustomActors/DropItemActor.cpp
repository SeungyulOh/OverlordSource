
#include "ProjectR.h"
#include "DropItemActor.h"
#include "Global/TableManager.h"
#include "Utils/UtilFunctionLibrary.h"
#include "ContentGameMode/Modes/StageGameMode.h"

ADropItemActor::ADropItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComponent->SetupAttachment(RootComponent);
}

void ADropItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (eDropItemAnimState)
	{
	case EDropItemAnimState::State_Drop:	StateDrop();	break;
	case EDropItemAnimState::State_Idle:	StateIdle(DeltaTime);	break;
	default:	break;
	}
}

void ADropItemActor::StateDrop()
{
	if (SkeletalMeshComponent)
	{
		if (!SkeletalMeshComponent->IsPlaying())
		{
			const bool bLooping = true;
			SkeletalMeshComponent->PlayAnimation(AnimAsset_Idle, bLooping);
			eDropItemAnimState = EDropItemAnimState::State_Idle;
		}
	}
}

void ADropItemActor::StateIdle(const float& DeltaTime)
{
	fLifeTime -= DeltaTime;
	if (fLifeTime <= 0.0f)
	{
		eDropItemAnimState = EDropItemAnimState::State_None;
		AStageGameMode* pStageGameMode = Cast<AStageGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
		if (pStageGameMode)
		{
			if (pStageGameMode->DropItemActors.Contains(this))
				pStageGameMode->DropItemActors.Remove(this);
		}

		this->Destroy();
	}
}

void ADropItemActor::SetDropItemData(USkeletalMesh* InTargetSkel, UAnimationAsset* InAnimAsset_Drop, UAnimationAsset* InAnimAsset_Idle, int32 InGrade, float InLifeTime)
{
	SkeletalMeshComponent->SetSkeletalMesh(InTargetSkel);
	AnimAsset_Drop = InAnimAsset_Drop;
	AnimAsset_Idle = InAnimAsset_Idle;
	fLifeTime = InLifeTime;

	if (SkeletalMeshComponent)
	{
		const bool bLooping = false;
		SkeletalMeshComponent->PlayAnimation(AnimAsset_Drop, bLooping);
		eDropItemAnimState = EDropItemAnimState::State_Drop;
	}

	if(ParticleComponent)
	{
		FDropItemParticleInfo* DropParticleInfo = UTableManager::GetInstancePtr()->GetDropItemParticleRow(InGrade);
		UParticleSystem* ps = UUtilFunctionLibrary::GetParticleSystemFromAssetID(DropParticleInfo->Particle);
		ParticleComponent->SetTemplate(ps);
		ParticleComponent->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("Item_Root"));
	}
}