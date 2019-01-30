// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_HeroInfoStat.generated.h"

class UCharacterBattleInfo;
class UTextBlock;
class ULocalizingTextBlock;
class UPanelWidget;

/**
 * Widget for displaying one hero stat. A component of UC_HeroInfoStats.
 */
UCLASS()
class PROJECTR_API UUC_HeroInfoStat : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroInfoStat")
	ECharacterPropertyTypeEnum CharacterPropertyType;

	void Refresh(UCharacterBattleInfo* CharacterBattleInfo);

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroInfoStat|Ref")
	ULocalizingTextBlock* Text_StatName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroInfoStat|Ref")
	UTextBlock* Text_Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroInfoStat|Ref")
	UTextBlock* Text_DetailValue;
};
