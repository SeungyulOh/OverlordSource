// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "JSONString.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UJSONString : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName UniqueName;

	UPROPERTY()
	FString ImportPath;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = JSONString)
	FString		JsonString;	

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Instanced, Category = ImportSettings)
	class UAssetImportData* AssetImportData;
#endif	// WITH_EDITORONLY_DATA

	void PostInitProperties() override;
};
