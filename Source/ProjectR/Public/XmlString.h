// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "XmlFile.h"
#include "XmlString.generated.h"

class FXmlNode;
/**
 * 
 */
UCLASS()
class PROJECTR_API UXmlString : public UObject
{
	GENERATED_BODY()

public:
	void		LoadXml();
	FXmlNode*	GetRootNode();

	virtual void BeginDestroy() override;

	void PostInitProperties() override;

	void		UnloadXml();

public:
	UPROPERTY()
	FString ImportPath;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = XmlString)
	FString		XmlString;

	FXmlFile	XmlFile;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Instanced, Category = ImportSettings)
	class UAssetImportData* AssetImportData;
#endif	// WITH_EDITORONLY_DATA

};
