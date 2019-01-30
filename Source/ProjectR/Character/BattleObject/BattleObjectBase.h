#pragma once
#include "Character/BattleBaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "SharedConstants/SharedGlobalConstants.h"

#include "BattleObjectBase.generated.h"

/**
*
*/
UCLASS()
class PROJECTR_API ABattleObjectBase : public ABattleBaseCharacter
{
	GENERATED_BODY()

	friend class UObjectBattleEntityComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObject)
		EBattleObjectTypeEnum	ElementType = EBattleObjectTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObject)
		EContinentTypeEnum	ContinentType = EContinentTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObject)
		USkeletalMeshComponent*	BodyMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObject)
		USkeletalMeshComponent*	HeadMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObject)
		UParticleSystemComponent*	OrbParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BattleObject)
		UAnimationAsset*	DeathAnim = nullptr;

public:
	ABattleObjectBase(const FObjectInitializer& ObjectInitializer);

	void OnConstruction(const FTransform& Transform) override;
	void Tick(float DeltaSecond) override;
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnDead();

	UFUNCTION()
		void SetOrbColor();
};