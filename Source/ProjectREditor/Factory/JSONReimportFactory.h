// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factory/JSONImportFactory.h"
#include "UnrealEd.h"
#include "JSONReimportFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREDITOR_API UJSONReimportFactory : public UJSONImportFactory, public FReimportHandler
{
	GENERATED_UCLASS_BODY()

	// Begin FReimportHandler interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	virtual int32 GetPriority() const override;
	// End FReimportHandler interface
};
