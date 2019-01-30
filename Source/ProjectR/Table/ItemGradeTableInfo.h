// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemGradeTableInfo.generated.h"

/**
 * Moved from Xmls/Etc/GameData - ItemGrades
 * client settings regarding item grades
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FItemGradeTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemGradeTableInfo)
	FName UIStringKey;

	// TODO: to FSlateColor
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemGradeTableInfo)
	FColor Color;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemGradeTableInfo)
	TSoftObjectPtr< UTexture2D >		IconImg = nullptr;
};
