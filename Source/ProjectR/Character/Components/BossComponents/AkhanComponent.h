// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "SharedConstants/GlobalConstants.h"
#include "AkhanComponent.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = (Boss), meta = (BlueprintSpawnableComponent))
class PROJECTR_API UAkhanComponent : public UBossBaseComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UAkhanComponent)
	ECharacterSituationState	CurSituationState = ECharacterSituationState::VE_Ground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UAkhanComponent)
	float		Height = 420.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UAkhanComponent)
	float		StartHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UAkhanComponent)
	float		FallTime = 1.0f;

	// Sets default values for this component's properties
	UAkhanComponent();

	// Called when the game starts
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
	void SetOwner(AActor* character) override;

	void ChangeSituation(ECharacterSituationState InState, float InTotalTime);

	virtual bool CanAttachSlot(EAbnormalSlotTypeEnum SlotType) override;


private:
	float			CurAniTime = 0.0f;
	float			TotalAniTime = 0.0f;
	float			CurStartHeight = 0.0f;
	bool			bFallStart = false;
};
