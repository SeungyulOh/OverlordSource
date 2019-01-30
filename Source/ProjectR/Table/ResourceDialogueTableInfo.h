// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "SharedConstants/GlobalConstants.h"
#include "ResourceDialogueTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FResourceDialogueTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ResourceDialogueTableInfo)
	FName		xml;

};