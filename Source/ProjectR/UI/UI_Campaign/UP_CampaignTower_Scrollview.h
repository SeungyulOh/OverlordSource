// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UP_CampaignTower_Scrollview.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_CampaignTower_Scrollview : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;


	void Invalidate_CurrentFloor();
	void MoveScroll_ToTargetFloor(int32 InTargetFloor);
	
	
public:
	UPROPERTY()
	class URTowerScrollView* TowerScrollView;

	UPROPERTY()
	UCanvasPanel* CanvasPanel_Message;
	
};
