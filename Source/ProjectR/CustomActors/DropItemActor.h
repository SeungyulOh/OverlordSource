#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropItemActor.generated.h"

class USkeletalMeshComponent;
class UAnimationAsset;
class UParticleSystemComponent;

enum class EDropItemAnimState : uint8
{
	State_None,
	State_Drop,
	State_Idle,
};

UCLASS()
class PROJECTR_API ADropItemActor: public AActor
{
	GENERATED_BODY()
public:
	ADropItemActor();

public:
	virtual void Tick(float DeltaTime) override;
	void SetDropItemData(USkeletalMesh* InTargetSkel, UAnimationAsset* InAnimAsset_Drop, UAnimationAsset* InAnimAsset_Idle, int32 InGrade, float InLifeTime = 30.0f);

private:
	void StateDrop();
	void StateIdle(const float& DeltaTime);
public:
	UPROPERTY()
	USkeletalMeshComponent*			SkeletalMeshComponent = nullptr;

	UPROPERTY()
	UParticleSystemComponent*		ParticleComponent = nullptr;

	UPROPERTY()
	UAnimationAsset*				AnimAsset_Drop = nullptr;

	UPROPERTY()
	UAnimationAsset*				AnimAsset_Idle = nullptr;

	UPROPERTY()
	float							fLifeTime = 0.0f;

public:
// 	UPROPERTY()
	EDropItemAnimState				eDropItemAnimState = EDropItemAnimState::State_None;
};
