// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PaperSprite.h"
#include "ItemEquipTypeTableInfo.generated.h"

/**
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FItemEquipTypeTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemGradeTableInfo)
	FName UIStringKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemGradeTableInfo)
	TSoftObjectPtr< UPaperSprite >		IconImg = nullptr;
};
