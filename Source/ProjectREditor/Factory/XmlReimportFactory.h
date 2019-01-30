// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factory/XmlImportFactory.h"
#include "UnrealEd.h"
#include "XmlReimportFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREDITOR_API UXmlReimportFactory : public UXmlImportFactory, public FReimportHandler
{
	GENERATED_UCLASS_BODY()

	// Begin FReimportHandler interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	virtual int32 GetPriority() const override;
	// End FReimportHandler interface
};
