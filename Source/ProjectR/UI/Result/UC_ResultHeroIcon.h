// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "Table/CharacterTableInfo.h"

#include "UC_ResultHeroIcon.generated.h"


#define dEMPTYSLOT 15
#define dTICKTIMER 0.5f
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_ResultHeroIcon : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetIcon(int32 InIndex);

	UPROPERTY()
	URenewal_BaseHeroIcon*	HeroIcon			=	nullptr;
	UPROPERTY()
	UTextBlock*				Text_Level			=	nullptr;

	UPROPERTY()
	int32					SlotIndex			=	dEMPTYSLOT;
// 	UPROPERTY()
// 	FHeroSlotInfo			HeroSlot;

	FCharacterCostGrowth*	CostGrowthInfo = nullptr;
	FHERO*					Hero = nullptr;
	float					TickTime = 0.0f;
	float					HeroExpPercent = 0.0f;
	float					HeroExpMaxPercent = 0.0f;
};