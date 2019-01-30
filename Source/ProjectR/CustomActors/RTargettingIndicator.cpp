// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RTargettingIndicator.h"

#include "UtilFunctionIntegrated.h"


// Sets default values
ARTargettingIndicator::ARTargettingIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UParticleSystem> PSFinder(TEXT("ParticleSystem'/Game/FX/Battle/FX_3_PointFire.FX_3_PointFire'"));	
	RootComponent = ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	
	if (PSFinder.Succeeded())
	{
		ParticleSystemComponent->SetTemplate(PSFinder.Object);
		ParticleSystemComponent->SetAutoActivate(true);
	}
}

// Called when the game starts or when spawned
void ARTargettingIndicator::BeginPlay()
{
	Super::BeginPlay();

	if (ParticleSystemComponent)
	{
		ParticleSystemComponent->OnSystemFinished.Clear();
		ParticleSystemComponent->OnSystemFinished.AddDynamic(this, &ARTargettingIndicator::DestroythisActor);
	}
	
}

// Called every frame
void ARTargettingIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentTarget.IsValid() || !ParticleSystemComponent || !SourceActor.IsValid())
		return;

	if (IsPendingKill())
		return;

	if (SourceActor->IsPendingKill() || !SourceActor->IsValidLowLevel())
		return;
	
	float HalfHeight = SourceActor->GetSimpleCollisionHalfHeight();
	//1600

	FVector2D Leader2D = FVector2D(SourceActor->GetActorLocation().X, SourceActor->GetActorLocation().Y);
	FVector2D Target2D = FVector2D(CurrentTarget->GetActorLocation().X, CurrentTarget->GetActorLocation().Y);
	FVector2D Dir = Target2D - Leader2D;
	float Dist = Dir.Size();

	Dir.Normalize();
	float fCos = FVector2D::DotProduct(Dir, FVector2D(1.f, 0.f));
	float fAngle = 0.f;
	if (Dir.Y > 0)
		fAngle = acosf(fCos);
	else
		fAngle = 2 * PI - acosf(fCos);

	fAngle = FMath::RadiansToDegrees(fAngle + PI);

	FRotator Rot = FRotator(0.f, fAngle, 0.f);

	ParticleSystemComponent->SetVisibility(true);
	ParticleSystemComponent->SetVectorParameter(FName(TEXT("ScaleX")), FVector(Dist / 1700, 0.25, 0.25));
	ParticleSystemComponent->SetWorldLocationAndRotation(SourceActor->GetActorLocation() + FVector(0.f, 0.f, HalfHeight), Rot);

}

void ARTargettingIndicator::DestroythisActor(UParticleSystemComponent* Component)
{
 	if(IsValidLowLevel())
 		Destroy();
}

