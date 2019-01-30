#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "BattleObjectBase.h"
#include "UObjectGlobals.h"
#include "GlobalIntegrated.h"

ABattleObjectBase::ABattleObjectBase(const FObjectInitializer& ObjectInitializer) :	Super(ObjectInitializer)
{
	BodyMeshComponent = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, FName(TEXT("SkeletalMesh")));
	BodyMeshComponent->SetupAttachment(RootComponent);
	OrbParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OrbParticle"));
}

void ABattleObjectBase::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogBattle, Log, TEXT("ABattleBaseObject::UserConstructionScript"));

	SetCharacterCategory(ECharacterCategoryTypeEnum::VE_Object);
	Super::OnConstruction(Transform);
}

void ABattleObjectBase::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond); 
	FVector temp;
	//OrbParticle->GetVectorParameter(FName("a"), temp);
	//UE_LOG(LogClass, Log, TEXT("x : %f, y : %f, z : %f"), temp.X, temp.Y, temp.Z);
	OrbParticle->SetVectorParameter(FName("a"), FVector(0, 0, 0));
}

void ABattleObjectBase::BeginPlay()
{
	Super::BeginPlay();
	TArray<UAnimSequence*> anim;
	//this->GetComponents(anim);
	SetOrbColor();
}

void ABattleObjectBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
}

void ABattleObjectBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ABattleObjectBase::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ABattleObjectBase::OnDead()
{
	
}

void ABattleObjectBase::SetOrbColor()
{
	OrbParticle->SetVectorParameter(FName("a"), FVector(0, 0, 0));
}
