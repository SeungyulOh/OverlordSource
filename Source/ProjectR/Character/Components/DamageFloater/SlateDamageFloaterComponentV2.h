// Fill out your copyright notice in the Description page of Project Settings.
//#if WITH_KOKOMO

#pragma once

#include "Components/ActorComponent.h"
#include "SDamageFloaterV2.h"
#include "SharedConstants/GlobalConstants.h"
#include "SlateDamageFloaterComponentV2.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API USlateDamageFloaterComponentV2 : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlateDamageFloaterComponentV2();

	// Called when the game starts
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void AddDamageFloater(int32 damagecount, int32 addPercent, EBattleDamageResultEnum damageType, float inScale, bool bShowText = true);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
	void SetDamageFloaterVisible(bool InVisible);

private:
	static int32 DamageFloaterCanvasRefCount;
	static TSharedPtr<SDamageFloaterV2> DamageFloaterCanvas;
};

//#endif
