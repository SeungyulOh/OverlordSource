// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factories/Factory.h"
#include "XmlImportFactory.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTREDITOR_API UXmlImportFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	virtual bool FactoryCanImport(const FString& Filename) override;
	
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn) override;
};
