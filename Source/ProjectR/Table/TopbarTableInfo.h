// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataEnum.h"
#include "TopbarTableInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FTopbarTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FTopbarTableInfo)
	TArray<FName> PropertyKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FTopbarTableInfo)
	EStringTableType	TextType = EStringTableType::VE_UI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FTopbarTableInfo)
	FName				TextKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FTopbarTableInfo)
	int32				BottomBar = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FTopbarTableInfo)
	int32				MiddleBarLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FTopbarTableInfo)
	int32				MiddleBarRight = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FTopbarTableInfo)
	bool				BackBoardVisible = true;
};