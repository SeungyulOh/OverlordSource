// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Control/RScrollView.h"
#include "UI/Control/RScrollItem.h"
#include "RTowerScrollView.generated.h"



UCLASS()
class PROJECTR_API URTowerScrollItem : public URScrollItem
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
public:
	class UUP_Campaign_Tower* UP_CampaignTower;
};
/**
 * 
 */
UCLASS()
class PROJECTR_API URTowerScrollView : public URScrollView
{
	GENERATED_BODY()
	
public:
	void Custom_Init(int InCreateCount);
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SetScrollUpdate(URScrollItem* CurItem, FVector2D diff);

	FORCEINLINE void ResetCurScrollOffSet() { CurScrollOffset = 0; }

private:
	float InitialVelocity = 0.f;
	int32 TargetOffSet = 0.f;
	
	
};
