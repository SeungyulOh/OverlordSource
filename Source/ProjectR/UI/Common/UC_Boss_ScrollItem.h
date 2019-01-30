// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Control/RScrollItem.h"
#include "UC_Icon_TowerBoss.h"
#include "HeroBaseScrollItem.h"
#include "UC_Boss_ScrollItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Boss_ScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void		NativeConstruct() override;
	void		InvalidateData() override;

	UFUNCTION()
		virtual void SetSelected(bool bSelected) override;

	UFUNCTION()
		virtual void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Boss_ScrollItem)
	UUC_Icon_TowerBoss* UC_Icon_TowerBoss = nullptr;
	FString HeroUD;
	ESelectType SelectState = ESelectType::VE_None;
};
