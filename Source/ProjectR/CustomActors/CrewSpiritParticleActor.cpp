

#include "ProjectR.h"
#include "CrewSpiritParticleActor.h"
#include "SharedConstants/SharedGlobalConstants.h"
#include "SharedConstants/GlobalConstants.h"
#include "UtilFunctionIntegrated.h"
#include "Global/RGameInstance.h"
#include "CustomStructures/GameEnvironmentVar.h"

ACrewSpiritParticleActor::ACrewSpiritParticleActor()
	: eRole(ECharacterRoleEnum::VE_None)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComponent->SetupAttachment(RootComponent);
}

void ACrewSpiritParticleActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACrewSpiritParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ParticleComponent && TargetSkeletalMeshComp && eRole != ECharacterRoleEnum::VE_None)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		float SpiritBackDistance = RGameInstance->GameEnvironmentVar.SpiritBackDistance;
		float SpiritLeftRightDistance = RGameInstance->GameEnvironmentVar.SpiritLeftRightDistance;
		float SpiritHeightDistance = RGameInstance->GameEnvironmentVar.SpiritHeightDistance;
		float SpiritSpeed = RGameInstance->GameEnvironmentVar.SpiritSpeed;

		if (bAlone)
			SpiritLeftRightDistance = 0.0f;
		FVector TargetLoc;
		FQuat WorldRot;
		TargetSkeletalMeshComp->GetSocketWorldLocationAndRotation(SOCKET_ROOT, TargetLoc, WorldRot);

		AActor* OwnerActor = TargetSkeletalMeshComp->GetOwner();
		if (!IsValid(OwnerActor))
			return;

		FVector BackwardVec = OwnerActor->GetActorForwardVector();
		FVector RightVec = OwnerActor->GetActorRightVector();
		BackwardVec *= -1;
		BackwardVec.Normalize(0);
		TargetLoc = TargetLoc + (BackwardVec * SpiritBackDistance);

		if(eRole == ECharacterRoleEnum::VE_Crew1)
		{
			FVector LeftVec = RightVec * -1;
			TargetLoc = TargetLoc + (LeftVec * SpiritLeftRightDistance);
		}
		else if(eRole == ECharacterRoleEnum::VE_Crew2)
		{
			TargetLoc = TargetLoc + (RightVec * SpiritLeftRightDistance);
		}
		UCapsuleComponent* ActorCapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(OwnerActor);
		if (ActorCapsuleComponent)
		{
			TargetLoc.Z += (ActorCapsuleComponent->GetScaledCapsuleHalfHeight() + SpiritHeightDistance);
		}

		FVector CurrentLoc = GetActorLocation();
		float fDistance = FVector::Dist(CurrentLoc, TargetLoc);

		if(50.0f <= fDistance)
		{
			FVector Dir = TargetLoc - CurrentLoc;
			Dir.Normalize(0);
			
			const float fSpeedConstant = 2.0f;
			
			float fSpeedCorrection = SpiritSpeed + ((100.0f < fDistance) ? fDistance * fSpeedConstant : 0);

			FVector TargetLocation = CurrentLoc + Dir * fSpeedCorrection * DeltaTime;

			FTransform transform;
			transform.SetTranslation(TargetLocation);
			ParticleComponent->SetWorldTransform(transform);
		}
	}
}

void ACrewSpiritParticleActor::SetParticleData(UParticleSystem* InPS, USkeletalMeshComponent* InTargetSkel, ECharacterRoleEnum InRole)
{
	ParticleComponent->SetTemplate(InPS);
	TargetSkeletalMeshComp = InTargetSkel;
	eRole = InRole;
}