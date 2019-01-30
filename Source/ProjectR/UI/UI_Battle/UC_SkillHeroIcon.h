// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "CustomStructures/HeroSlotInfo.h"

#include "UC_SkillHeroIcon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_SkillHeroIcon : public URBaseWidget
{
	GENERATED_BODY()

public:
	void	NativeConstruct() override;
	void	SetHero(FName id);
	void	SetHero(FHeroSlotInfo info);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillHeroIcon)
	UTextBlock*				Text_HeroName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillHeroIcon)
	UTextBlock*				Text_Level = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillHeroIcon)
	UImage*					Image_Hero = nullptr;

};
