// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Global/BattleRecordManager.h"
#include "UI/UIDefs.h"

#include "UC_Result_Hero.generated.h"

class UTextBlock;
class URenewal_BaseHeroIcon;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Result_Hero : public URBaseWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = UC_Result_Hero)
	void UpdateInfo(const FHERO_ICON_INFO& HeroIcon, FBattleRecord BattleRecord);

	UFUNCTION()
	void OnUpdateHeroIcon();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UC_Result_Hero)
	FHERO_ICON_INFO			ResultHeroIconInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UC_Result_Hero)
	UTextBlock*				TextBlock_Kill = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UC_Result_Hero)
	UTextBlock*				TextBlock_Attack = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UC_Result_Hero)
	UTextBlock*				TextBlock_Damage = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UC_Result_Hero)
	UTextBlock*				TextBlock_Heal = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UC_Result_Hero)
	UTextBlock*				TextBlock_Skill = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UC_Result_Hero)
	URenewal_BaseHeroIcon*	UC_Icon_Hero;

	
	
};
