// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "GrindReturnMaterial.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API  FGrindReturnMaterial : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GrindReturnMaterial)
	FName ItemCodeName1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GrindReturnMaterial)
	int32 ItemCodeName1Amt;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GrindReturnMaterial)
	FName ItemCodeName2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GrindReturnMaterial)
	int32 ItemCodeName2Amt;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GrindReturnMaterial)
	FName ItemCodeName3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GrindReturnMaterial)
	int32 ItemCodeName3Amt;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GrindReturnMaterial)
	FName ItemCodeName4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GrindReturnMaterial)
	int32 ItemCodeName4Amt;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GrindReturnMaterial)
	FName ItemCodeName5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GrindReturnMaterial)
	int32 ItemCodeName5Amt;

	
};
