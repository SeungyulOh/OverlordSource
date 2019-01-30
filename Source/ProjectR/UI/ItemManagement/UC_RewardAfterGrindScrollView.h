// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Control/RScrollView.h"
#include "Network/PacketDataStructures.h"
#include "UC_RewardAfterGrindScrollView.generated.h"

class UUC_Currency_Icon;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_RewardAfterGrindScrollView : public URScrollView
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	void InitScrollView();


	void SetReward(TArray<FREWARD>& Rewards);
	void SetItemDataintoScrollItem();

private:
	TArray<FREWARD> GrindRewards;
	
	
	
};
