// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "UI/UIDefs.h"
#include "HeroScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UHeroScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UHeroScrollItemData)
	FHERO_ICON_INFO			HeroIconInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UHeroScrollItemData)
	bool				IsEmplacedImgShow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UHeroScrollItemData)
	bool				IsLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UHeroScrollItemData)
	bool				bUsed = false;

};
