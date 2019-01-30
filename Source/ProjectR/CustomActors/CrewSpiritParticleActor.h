#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrewSpiritParticleActor.generated.h"

enum class ECharacterRoleEnum : uint8;

UCLASS()
class PROJECTR_API ACrewSpiritParticleActor : public AActor
{
	GENERATED_BODY()
public:
	ACrewSpiritParticleActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetParticleData(UParticleSystem* InPS, USkeletalMeshComponent* InTargetSkel, ECharacterRoleEnum InRole);
public:
	UPROPERTY()
	class UParticleSystemComponent*		ParticleComponent = nullptr;

	UPROPERTY()
	class USkeletalMeshComponent*		TargetSkeletalMeshComp = nullptr;
	
public:
	UPROPERTY()
	ECharacterRoleEnum		eRole;//= ECharacterRoleEnum::VE_None;

	bool bAlone = false;
};