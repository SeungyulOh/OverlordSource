// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChatStringAttributeTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FChatStringAttributeTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChatStringAttribute)
		int32 font_size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChatStringAttribute)
		FColor color;
};
