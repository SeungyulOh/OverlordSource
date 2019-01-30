// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "SharedConstants/GlobalConstants.h"
#include "GigantComponent.generated.h"

class URaidObjectSocketMgr;

/**
 * 
 */
UCLASS(ClassGroup = (Boss), meta = (BlueprintSpawnableComponent))
class PROJECTR_API UGigantComponent : public UBossBaseComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GigantComponent)
	float						UpHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GigantComponent)
	float						UpRadius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GigantComponent)
	float						UpTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GigantComponent)
	float						ThrowEachDelayTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GigantComponent)
	float						ThrowDelayTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GigantComponent)
	float						ThrowTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GigantComponent)
	float						FallSpawnMinTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GigantComponent)
	float						FallSpawnMaxTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GigantComponent)
	float						FallDelayTime = 5.0f;


	// for modifing searching pivot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GigantComponent)
	float						PivotZOffset = 0.0f;




	// Sets default values for this component's properties
	UGigantComponent();

	// Called when the game starts
	void BeginPlay() override;
		
	void SetOwner(AActor* character) override;
	bool IsUsePathfinding() override { return false; }	

	FVector GetGigantDir();
	void ChangeGigantDir();

	bool	IsMoveEnable() override { return false; }

	virtual bool CanAttachSlot(EAbnormalSlotTypeEnum SlotType) override;

protected:
	FVector			FrontVector = FVector::ForwardVector;
	TArray<AActor*>		OverlapList;
};
