// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "UC_Hero.generated.h"

class URGameInstance;
class URenewal_BaseHeroIcon;

/**
 * BP: UC_Hero
 * embedded in UP_Mediator_HeroManagement (BP: UP_Hero_Manager)
 *
 * represents currently equipped items by EquipPosition
 */
UCLASS()
class PROJECTR_API UUC_Hero : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;
	void Init();

	void UpdateSelectedHeroUDs(const TArray<FString>& InSpiritUDs);

	TArray<URenewal_BaseHeroIcon*> UC_HeroIcons;
	TArray<FString>                SelectedSpiritUDs;
};
